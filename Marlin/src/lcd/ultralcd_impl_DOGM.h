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
 * ultralcd_impl_DOGM.h
 *
 * Graphics LCD implementation for 128x64 pixel LCDs by STB for ErikZalm/Marlin
 * Demonstrator: http://www.reprap.org/wiki/STB_Electronics
 * License: http://opensource.org/licenses/BSD-3-Clause
 *
 * With the use of:
 * u8glib by Oliver Kraus
 * https://github.com/olikraus/U8glib_Arduino
 * License: http://opensource.org/licenses/BSD-3-Clause
 */

/**
 * Implementation of the LCD display routines for a DOGM128 graphic display.
 * These are common LCD 128x64 pixel graphic displays.
 */

#ifndef ULTRALCD_IMPL_DOGM_H
#define ULTRALCD_IMPL_DOGM_H

#include "../MarlinCore.h"

#include "../u8g/u8g2.h"
#include "../u8g/U8g2lib.h"

#include "ultralcd.h"
#include "dogm_bitmaps.h"
#include "../core/utility.h"
#include "../libs/duration_t.h"

#if ENABLED(AUTO_BED_LEVELING_UBL)
#include "ubl.h"
#endif

// Only Western languages support big / small fonts
#if DISABLED(DISPLAY_CHARSET_ISO10646_1)
#undef USE_BIG_EDIT_FONT
#undef USE_SMALL_INFOFONT
#endif

#define INFO_FONT_HEIGHT 8
#define TALL_FONT_CORRECTION 0 

// DOGM parameters (size in pixels)
#define DOG_CHAR_WIDTH 6
#define DOG_CHAR_HEIGHT 12

#define DOG_CHAR_WIDTH_EDIT DOG_CHAR_WIDTH
#define DOG_CHAR_HEIGHT_EDIT DOG_CHAR_HEIGHT

#define START_COL 0

extern U8G2_UC1701_MINI12864_F_4W_HW_SPI u8g;

void lcd_print(const char c);

char lcd_print_and_count(const char c);

void lcd_print(const char *str);

// Initialize or re-initialize the LCD
void lcd_implementation_init();

// The kill screen is displayed for unrecoverable conditions
void lcd_kill_screen();

extern uint8_t row_y1, row_y2;
uint8_t constexpr row_height = DOG_CHAR_HEIGHT + 2 * (TALL_FONT_CORRECTION);

void lcd_implementation_hotend_status(const uint8_t row, const uint8_t extruder);

// Set the colors for a menu item based on whether it is selected
void lcd_implementation_mark_as_selected(const uint8_t row, const bool isSelected);

// Draw a static line of text in the same idiom as a menu item
void lcd_implementation_drawmenu_static(const uint8_t row, const char *pstr, const bool center = true, const bool invert = false, const char *valstr = NULL);

// Draw a generic menu item
void lcd_implementation_drawmenu_generic(const bool isSelected, const uint8_t row, const char *pstr, const char *pre_char, const char *post_char);

void lcd_implementation_drawmenu_var(const bool isSelected, const uint8_t row, const char *pstr, const char *vstr, const char pre_char, const char post_char);

// Macros for specific types of menu items
#define lcd_implementation_drawmenu_back(sel, row, pstr, dummy) lcd_implementation_drawmenu_generic(sel, row, pstr, LCD_STR_UPLEVEL, LCD_STR_UPLEVEL)
#define lcd_implementation_drawmenu_submenu(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, ">", LCD_STR_ARROW_RIGHT)
#define lcd_implementation_drawmenu_gcode(sel, row, pstr, gcode) lcd_implementation_drawmenu_generic(sel, row, pstr, ">", " ")
#define lcd_implementation_drawmenu_function(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, ">", " ")

#define lcd_implementation_drawmenu_prepage(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, ">", LCD_STR_ARROW_RIGHT)
#define lcd_implementation_drawmenu_nextpage(sel, row, pstr, data) lcd_implementation_drawmenu_generic(sel, row, pstr, ">", LCD_STR_ARROW_RIGHT)
#define lcd_implementation_drawmenu_vfunction(sel, row, pstr, vstr, data) lcd_implementation_drawmenu_var(sel, row, pstr, vstr, '>', ' ')

// Draw a menu item with an editable value
void _drawmenu_setting_edit_generic(const bool isSelected, const uint8_t row, const char *pstr, const char *const data, const bool pgm);

// Macros for edit items
#define lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, data) _drawmenu_setting_edit_generic(sel, row, pstr, data, false)
#define lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, data) _drawmenu_setting_edit_generic(sel, row, pstr, data, true)

#define DRAWMENU_SETTING_EDIT_GENERIC(_src) lcd_implementation_drawmenu_setting_edit_generic(sel, row, pstr, _src)
#define DRAW_BOOL_SETTING(sel, row, pstr, data) lcd_implementation_drawmenu_setting_edit_generic_P(sel, row, pstr, (*(data)) ? MMSG_LCD_ON[wtvar_language] : MMSG_LCD_OFF[wtvar_language])

void lcd_implementation_drawedit(const char *const pstr, const char *const value = NULL);

#if ENABLED(SDSUPPORT)

void lcd_implementation_drawmenu_listfile(const bool isSelected, const uint8_t row, const char *const pstr, const char *filename);

#endif // SDSUPPORT

void wt_single_line_message(const uint8_t row, const char *str);


#endif // __ULTRALCD_IMPL_DOGM_H
