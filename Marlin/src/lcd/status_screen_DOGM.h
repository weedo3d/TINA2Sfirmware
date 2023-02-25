/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * status_screen_DOGM.h
 *
 * Standard Status Screen for Graphical Display
 */

#ifndef _STATUS_SCREEN_DOGM_H_
#define _STATUS_SCREEN_DOGM_H_

#include "../libs/numtostr.h"
#include "../core/types.h"

void _draw_centered_temp(const int16_t temp, const uint8_t x, const uint8_t y);

void _draw_axis_value(const AxisEnum axis, const char *value, const bool blink);

void lcd_implementation_status_message(const bool blink);

void lcd_implementation_status_screen();

#endif // _STATUS_SCREEN_DOGM_H_
