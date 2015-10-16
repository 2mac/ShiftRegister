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

#define SR_DELAY (5)

ShiftRegister::ShiftRegister(int ser, int srclk, int rclk, int srclr, int qh,
			     int oe)
  : data(ser), clk(srclk), latch(rclk), clr(srclr), out(qh), oe(oe)
{
  pinMode(this->data, OUTPUT);
  pinMode(this->clk, OUTPUT);
  pinMode(this->latch, OUTPUT);
  pinMode(this->clr, OUTPUT);
  pinMode(this->out, INPUT);
  pinMode(this->oe, OUTPUT);

  digitalWrite(this->clr, HIGH);
  digitalWrite(this->oe, HIGH);
}

void
ShiftRegister::pushBit(boolean state)
{
  digitalWrite(this->data, state);
  digitalWrite(this->clk, HIGH);
  delayMicroseconds(SR_DELAY);
  digitalWrite(this->clk, LOW);
}

void
ShiftRegister::clear()
{
  digitalWrite(this->clr, LOW);
  delayMicroseconds(SR_DELAY);
  digitalWrite(this->clr, HIGH);
}

void
ShiftRegister::show()
{
  digitalWrite(this->latch, HIGH);
  delayMicroseconds(SR_DELAY);
  digitalWrite(this->latch, LOW);
}

void
ShiftRegister::pushByte(uint8_t byte)
{
  for (uint8_t bit = 128; bit >= 1; bit /= 2)
    this->pushBit(byte & bit);
}

boolean
ShiftRegister::readBit()
{
  return digitalRead(this->out);
}

uint8_t
ShiftRegister::readByte(boolean pushBack)
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
ShiftRegister::setOutputEnabled(boolean state)
{
  digitalWrite(this->oe, !state);
}
