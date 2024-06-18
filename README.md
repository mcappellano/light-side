LIGHT SIDE 
READ ME UPDATED: JUNE 18, 2024

Before running code, install ESP32 Drivers
-CP210x USB to UART Bridge VCP Drivers from silicon labs : https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
Downloads -> FILE: CP210x Windows Drivers
  -make sure you dont install the universal driver
  -install this file and run the exe

Also make sure sure you change VSC to board: ESP32 Pico Kit (Espressif)
  -with arduino framework

platformio settings should be: 
platform = espressif32
board = pico32
framework = arduino
monitor_speed = 115200
