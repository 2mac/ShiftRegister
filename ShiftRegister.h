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

#ifndef ARDUINO_SHIFTREG_H
#define ARDUINO_SHIFTREG_H

#include "Arduino.h"

class ShiftRegister
{
public:
  /**
   * Constructor which initializes pin states.
   * @param ser Arduino pin connected to SER on the register.
   * @param srclk Arduino pin connected to SRCLK on the register.
   * @param rclk Arduino pin connected to RCLK on the register.
   * @param srclr Arduino pin connected to SRCLR on the register.
   * @param qhprime Arduino pin connected to the QH` pin of the last register.
   * @param oe Arduino pin connected to the OE pin of the register.
   */
  ShiftRegister(int ser, int srclk, int rclk, int srclr, int qhprime, int oe);

  /**
   * Shifts a bit onto the register.
   * @param state Value of the bit to be pushed.
   */
  void
  pushBit(boolean state);

  /**
   * Clears all data stored on the register.
   */
  void
  clear();

  /**
   * Writes the stored data to the register's output pins.
   */
  void
  show();

  /**
   * Shifts a byte onto the register.
   * @param byte The byte to be pushed, with the first bit in the highest value
   * position.
   */
  void
  pushByte(uint8_t byte);

  /**
   * Reads the last bit from the register.
   * @return true if the last bit is HIGH.
   */
  boolean
  readBit();

  /**
   * Reads a byte from the register.
   * @param pushBack Whether to push each bit read back onto the register.
   * @return The byte read, with the first bit in the highest value position.
   */
  uint8_t
  readByte(boolean pushBack);

  /**
   * Sets the state of the register's output.
   * @param state true to allow output.
   */
  void
  setOutputEnabled(boolean state);

private:
  const int data, clk, latch, clr, out, oe;
};

#endif
