/*
 *  ShiftRegister - A shift register library for Arduino
 *  Copyright (C) 2015 David McMackins II
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, version 3 only.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ShiftRegister.h"

#define SR_DELAY (5) /* documented microsecond delay for popular registers */

ShiftRegister::ShiftRegister(int ser, int srclk, int rclk, int srclr, int qh,
			     int oe)
  : data(ser), clk(srclk), latch(rclk), clr(srclr), out(qh), oe(oe)
{
  this->initPin(this->data, OUTPUT);
  this->initPin(this->clk, OUTPUT);
  this->initPin(this->latch, OUTPUT);
  this->initPin(this->clr, OUTPUT);
  this->initPin(this->out, INPUT);
  this->initPin(this->oe, OUTPUT);

  this->setPin(this->clr, HIGH);
  this->setPin(this->oe, LOW);
}

void
ShiftRegister::pushBit(boolean state) const
{
  this->setPin(this->data, state);
  this->setPin(this->clk, HIGH);
  delayMicroseconds(SR_DELAY);
  this->setPin(this->clk, LOW);
}

void
ShiftRegister::clear() const
{
  this->setPin(this->clr, LOW);
  delayMicroseconds(SR_DELAY);
  this->setPin(this->clr, HIGH);
}

void
ShiftRegister::show() const
{
  this->setPin(this->latch, HIGH);
  delayMicroseconds(SR_DELAY);
  this->setPin(this->latch, LOW);
}

void
ShiftRegister::pushByte(uint8_t byte) const
{
  for (uint8_t bit = 128; bit >= 1; bit /= 2)
    this->pushBit(byte & bit);
}

boolean
ShiftRegister::readBit() const
{
  if (this->out >= 0)
    return digitalRead(this->out);

  return -1;
}

uint8_t
ShiftRegister::readByte(boolean pushBack) const
{
  uint8_t byte = 0;

  for (uint8_t val = 128; val >= 1; val /= 2)
    {
      boolean lastBit = this->readBit();

      if (pushBack)
	pushBit(lastBit);
      else
	pushBit(false);

      if (lastBit)
	byte += val;
    }

  return byte;
}

void
ShiftRegister::setOutputEnabled(boolean state) const
{
  this->setPin(this->oe, !state);
}

void
ShiftRegister::initPin(int pin, int mode) const
{
  if (pin >= 0)
    pinMode(pin, mode);
}

void
ShiftRegister::setPin(int pin, boolean state) const
{
  if (pin >= 0)
    digitalWrite(pin, state);
}
