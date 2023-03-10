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

#include "../../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "../gcode.h"
#include "../../sd/cardreader.h"
#include "../../module/printcounter.h"
#include "../../lcd/ultralcd.h"

#if ENABLED(PARK_HEAD_ON_PAUSE)
  #include "../../feature/pause.h"
  #include "../queue.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../feature/host_actions.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/powerloss.h"
#endif

#include "../../MarlinCore.h" // for startOrResumeJob

#include "../../wtlib/WTCMD.h"

/**
 * M24: Start or Resume SD Print
 */
void GcodeSuite::M24() {

  #if ENABLED(POWER_LOSS_RECOVERY)
    if (parser.seenval('S')) card.setIndex(parser.value_long());
    if (parser.seenval('T')) print_job_timer.resume(parser.value_long());
  #endif

  #if ENABLED(PARK_HEAD_ON_PAUSE)
    if (did_pause_print) {
      resume_print(); // will call print_job_timer.start()
      return;
    }
  #endif

  if (card.isFileOpen()) {
    card.startFileprint();            // SD card will now be read for commands
    startOrResumeJob();               // Start (or resume) the print job timer
    #if ENABLED(POWER_LOSS_RECOVERY)
      recovery.prepare();
    #endif

	wt_machineStatus = WT_STATUS::PRINTING;
	#ifdef WIFI_MANAGER
    wifi_manager.isBusy(true);
	#endif
  // host_action_start();

  }

  #if ENABLED(HOST_ACTION_COMMANDS)
    #ifdef ACTION_ON_RESUME
      host_action_resume();
    #endif
    #if ENABLED(HOST_PROMPT_SUPPORT)
      host_prompt_open(PROMPT_INFO, PSTR("Resuming SD"), DISMISS_STR);
    #endif
  #endif

  lcd_reset_status();
}

/**
 * M25: Pause SD Print
 */
void GcodeSuite::M25() {

  lcd_sdcard_pause();
  
}

#endif // SDSUPPORT
