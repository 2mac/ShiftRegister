/*
 *  ShiftRegister - A shift register library for Arduino
 *  Copyright (C) 2015-2016 David McMackins II
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

#ifndef ARDUINO_SHIFTREG_H
#define ARDUINO_SHIFTREG_H

#include "Arduino.h"

class ShiftRegister
{
public:
  const int data, clk, latch, clr, out, oe;

  /**
   * Constructor which initializes pin states.
   * @param ser Arduino pin connected to SER on the register.
   * @param srclk Arduino pin connected to SRCLK on the register.
   * @param rclk Arduino pin connected to RCLK on the register.
   * @param srclr Arduino pin connected to SRCLR on the register.
   * @param qhprime Arduino pin connected to the QH` pin of the last register.
   * @param oe Arduino pin connected to the OE pin of the register.
   */
  ShiftRegister(int ser=-1, int srclk=-1, int rclk=-1, int srclr=-1,
		int qhprime=-1, int oe=-1);

  /**
   * Shifts a bit onto the register.
   * @param state Value of the bit to be pushed.
   */
  void
  pushBit(boolean state) const;

  /**
   * Clears all data stored on the register.
   */
  void
  clear() const;

  /**
   * Writes the stored data to the register's output pins.
   */
  void
  show() const;

  /**
   * Shifts a byte onto the register.
   * @param byte The byte to be pushed, with the first bit in the highest value
   * position.
   */
  void
  pushByte(uint8_t byte) const;

  /**
   * Reads the last bit from the register.
   * @return true if the last bit is HIGH.
   */
  boolean
  readBit() const;

  /**
   * Reads a byte from the register.
   * @param pushBack Whether to push each bit read back onto the register.
   * @return The byte read, with the first bit in the highest value position.
   */
  uint8_t
  readByte(boolean pushBack=false) const;

  /**
   * Sets the state of the register's output.
   * @param state true to allow output.
   */
  void
  setOutputEnabled(boolean state) const;

protected:
  void
  initPin(int pin, int mode) const;

  void
  setPin(int pin, boolean state) const;
};

template<size_t num_bytes>
class StateBasedRegister : protected ShiftRegister
{
public:
  StateBasedRegister(const ShiftRegister& reg)
    : ShiftRegister(reg.data, reg.clk, reg.latch, reg.clr, reg.out, reg.oe)
  {
    this->clearBytes();
  }

  StateBasedRegister(int ser, int srclk, int rclk, int srclr=-1,
		     int qhprime=-1, int oe=-1)
    : ShiftRegister(ser, srclk, rclk, srclr, qhprime, oe)
  {
    this->clearBytes();
  }

  void
  clear()
  {
    if (this->clr >= 0)
      {
	ShiftRegister::clear();
      }
    else
      {
	for (size_t i = 0; i < num_bytes; ++i)
	  this->pushByte(0);
      }

    this->clearBytes();
  }

  void
  write(int pin, boolean state)
  {
    if (pin > num_bytes * 8 || pin < 0)
      return;

    size_t i = pin / 8;
    uint8_t mask = pow(2, pin % 8);

    if (state)
      this->bytes[i] |= mask;
    else
      this->bytes[i] &= 0xFF - mask;

    this->flushBytes();
  }

protected:
  uint8_t bytes[num_bytes];

  void
  clearBytes()
  {
    for (size_t i = 0; i < num_bytes; ++i)
      this->bytes[i] = 0;

    this->clear();
  }

  void
  flushBytes()
  {
    for (size_t i = num_bytes; i > 0; --i)
      this->pushByte(this->bytes[i - 1]);
  }
};

#endif
