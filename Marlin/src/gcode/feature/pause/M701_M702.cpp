/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(FILAMENT_LOAD_UNLOAD_GCODES)

#include "../../gcode.h"
#include "../../../MarlinCore.h"
#include "../../../module/motion.h"
#include "../../../module/temperature.h"
#include "../../../feature/pause.h"

#if EXTRUDERS > 1
  #include "../../../module/tool_change.h"
#endif

#if HAS_LCD_MENU
  #include "../../../lcd/ultralcd.h"
#endif

#if ENABLED(PRUSA_MMU2)
  #include "../../../feature/mmu2/mmu2.h"
#endif

#if ENABLED(MIXING_EXTRUDER)
  #include "../../../feature/mixing.h"
#endif

/**
 * M701: Load filament
 *
 *  T<extruder> - Extruder number. Required for mixing extruder.
 *                For non-mixing, current extruder if omitted.
 *  Z<distance> - Move the Z axis by this distance
 *  L<distance> - Extrude distance for insertion (positive value) (manual reload)
 *
 *  Default values are used for omitted arguments.
 */
void GcodeSuite::M701() {
  xyz_pos_t park_point = NOZZLE_PARK_POINT;

  #if ENABLED(NO_MOTION_BEFORE_HOMING)
    // Don't raise Z if the machine isn't homed
    if (axes_need_homing()) park_point.z = 0;
  #endif

  #ifdef WIFI_MANAGER
  wifi_manager.isBusy(true);
  #endif

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  // Z axis lift
  if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

  // Show initial "wait for load" message
  #if HAS_LCD_MENU
    lcd_advanced_pause_show_message(PAUSE_MESSAGE_LOAD, PAUSE_MODE_LOAD_FILAMENT, target_extruder);
  #endif

  // Lift Z axis
  if (park_point.z > 0)
    do_blocking_move_to_z(_MIN(current_position.z + park_point.z, Z_MAX_POS), feedRate_t(NOZZLE_PARK_Z_FEEDRATE));

  // Load filament
  wtvar_click_cancel_heat = true;		
	wtvar_click_cancel_emove = true;
  constexpr float     purge_length = M701_PURGE_LENGTH,
                  slow_load_length = FILAMENT_CHANGE_SLOW_LOAD_LENGTH;
      const float fast_load_length = ABS(parser.seen('L') ? parser.value_axis_units(E_AXIS)
                                                          : fc_settings[active_extruder].load_length);
  load_filament(
    slow_load_length, fast_load_length, purge_length,
    FILAMENT_CHANGE_ALERT_BEEPS,
    true,                                           // show_lcd
    thermalManager.still_heating(target_extruder),  // pause_for_user
    PAUSE_MODE_LOAD_FILAMENT                        // pause_mode
    #if ENABLED(DUAL_X_CARRIAGE)
      , target_extruder                             // Dual X target
    #endif
  );

  wtvar_click_cancel_heat = false;	
	wtvar_click_cancel_emove = false;

  // Restore Z axis
  if (park_point.z > 0)
    do_blocking_move_to_z(_MAX(current_position.z - park_point.z, 0), feedRate_t(NOZZLE_PARK_Z_FEEDRATE));

  // Show status screen
  #if HAS_LCD_MENU
    lcd_advanced_pause_show_message(PAUSE_MESSAGE_STATUS);
  #endif

  thermalManager.setTargetHotend(0, 0);
  
  #ifdef WIFI_MANAGER
  wifi_manager.isBusy(false);
  #endif
}

/**
 * M702: Unload filament
 *
 *  T<extruder> - Extruder number. Required for mixing extruder.
 *                For non-mixing, if omitted, current extruder
 *                (or ALL extruders with FILAMENT_UNLOAD_ALL_EXTRUDERS).
 *  Z<distance> - Move the Z axis by this distance
 *  U<distance> - Retract distance for removal (manual reload)
 *
 *  Default values are used for omitted arguments.
 */
void GcodeSuite::M702() {
  xyz_pos_t park_point = NOZZLE_PARK_POINT;

  #ifdef WIFI_MANAGER
  wifi_manager.isBusy(true);
  #endif

  #if ENABLED(NO_MOTION_BEFORE_HOMING)
    // Don't raise Z if the machine isn't homed
    if (axes_need_homing()) park_point.z = 0;
  #endif

  const int8_t target_extruder = get_target_extruder_from_command();
  if (target_extruder < 0) return;

  // Z axis lift
  if (parser.seenval('Z')) park_point.z = parser.linearval('Z');

  // Show initial "wait for unload" message
  #if HAS_LCD_MENU
    lcd_advanced_pause_show_message(PAUSE_MESSAGE_UNLOAD, PAUSE_MODE_UNLOAD_FILAMENT, target_extruder);
  #endif

  // Lift Z axis
  if (park_point.z > 0)
    do_blocking_move_to_z(_MIN(current_position.z + park_point.z, Z_MAX_POS), feedRate_t(NOZZLE_PARK_Z_FEEDRATE));

  // Unload filament

  // Unload length
  const float unload_length = -ABS(parser.seen('U') ? parser.value_axis_units(E_AXIS)
                                                    : fc_settings[target_extruder].unload_length);
  wtvar_click_cancel_heat = true;			
	wtvar_click_cancel_emove = true;
  unload_filament(unload_length, true, PAUSE_MODE_UNLOAD_FILAMENT
    #if ALL(FILAMENT_UNLOAD_ALL_EXTRUDERS, MIXING_EXTRUDER)
      , mix_multiplier
    #endif
  );
  wtvar_click_cancel_heat = false;		
	wtvar_click_cancel_emove = false;


  // Restore Z axis
  if (park_point.z > 0)
    do_blocking_move_to_z(_MAX(current_position.z - park_point.z, 0), feedRate_t(NOZZLE_PARK_Z_FEEDRATE));

  // Show status screen
  #if HAS_LCD_MENU
    lcd_advanced_pause_show_message(PAUSE_MESSAGE_STATUS);
  #endif

  thermalManager.setTargetHotend(0, 0);

  #ifdef WIFI_MANAGER
  wifi_manager.isBusy(false);
  #endif
}

#endif // ADVANCED_PAUSE_FEATURE
