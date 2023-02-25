# TINA2S Community Firmware
![image](http://www.weedo.ltd/wp-content/uploads/2023/02/tina2s.jpg)

## Summary
This is the repository that contains the community version firmware for the TINA2S motor and UI module. 

The MCU of this module uses GD32F103RET6. The firmware source code is based on the Marlin 2.0.x. We follow the GPL-3.0 copyright license policy and open source this part of the code.

The network module of TINA2S uses ESP32. The firmware is developed by us. Because it involves the company's confidential technology. We do not provide open source code.

## Hardware Version 
- TINA2 Classic: The motherboard model is R62A/R62AS. The MCU chip uses ATMEGA2560. The open source code address is [TINA2firmware](https://github.com/weedo3d/TINA2firmware).

- TINA2 Upgrade: The motherboard model is R72A/R72B. The MCU chip uses GD32F103RET6.

- TINA2S: The motherboard model is R72H/R72P. The MCU chip uses GD32F103RET6. The stepper motor driver chip uses TMC2208.

## Compile requirements

- Download and install [VSCode](https://code.visualstudio.com/)
- Search and install PlatformIO IDE from store marketplace
- Search and install ST STM32 embedded platform from PIO Home
- Copy framework-arduinoststm32-maple@99.99.99 from /buildroot/lib to PlatformIO library location (user)/.platformio/packages/
- Install the USB driver from /buildroot/driver

## Upload firmware

TINA2S uses a customized bootloader, which requires a customized download program for firmware update.  

The Windows version of the download program WEEDOIAP.exe is located in the /buildroot/upload/Win directory. 

The macos version of the download program WEEDOIAP.app is located in the /buildroot/upload/Mac directory. 

Lanuch the WEEDOIAP.exe, open the firmware.bin file from build directory, choice the com port, then click the update.

![image](http://www.weedo.ltd/wp-content/uploads/2021/04/weedoiap.png)


## The difference between the community version firmware and the official version firmware

The official firmware has the function of upgrading the firmware via TF card. Use the IAP function in the control menu to read the flash.wfm file on the TF card to upgrade the firmware.

The community version firmware does not support the wfm format, so it does not support firmware upgrade via TF card.


## Return to the official vesrion firmware

Use WEEDOIAP.exe to flash TINA2S_Vxxx_Community_to_Official.bin in the /Release directory.
