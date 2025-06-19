# Board Overview

This is the DT-R008 V3.6.13 v2 series (3.6.10+) board.

The exact PCB model is V3.6.13 with RS485 interface.

It came with the locked chip, then replaced with ESP32-D0WD-V3.


## Signals distribution

| WROOM          | 74HD165D     | 74HC595D    | 74LVC1G04  | BL3085           | JL1101 | connector | Comment                              | Serial Programmer 3.3V |
| :------------- | :----------- | :---------- | :--------- | :--------------- | :----- | :-------- | :----------------------------------- | :--------------------- |
| GPIO15 (pin23) | 15 (CLK_INH) | 12 (RCLK)   | 2 (A)      |                  |        |           | Inhibit clock when High              |                        |
|                | 1 (SH/LD)    |             | 4 (Y = /A) |                  |        |           | Load input when Low                  |                        |
| GPIO14 (pin13) | 2 (CLK)      | 11 (SRCLK)  |            |                  |        |           | Latch input, shift output on Rising  |                        |
| GPIO16 (pin27) | 9 (Q)        |             |            |                  |        |           | spi MISO                             |                        |
| GPIO32 (pin8)  |              | 13 (/OE)    |            |                  |        |           | Remove high-z on outputs when Low    |                        |
|                |              | 10 (/SRCRL) |            |                  |        |           | pulled-up                            |                        |
| GPIO13 (pin16) |              | 14 (SER)    |            |                  |        |           | spi MOSI                             |                        |
| GPIO36 (pin4)  | 6 (H)        |             |            |                  |        | I1        | pulled-up                            |                        |
| GPIO39 (pin5)  | 5 (G)        |             |            |                  |        | I2        | pulled-up                            |                        |
| GPIO35 (pin7)  | 4 (F)        |             |            |                  |        | I3        | pulled-up                            |                        |
| GPIO12 (pin14) | 3 (E)        |             |            |                  |        | I4        | pulled-up (Set Low to flash)         | !                      |
|                | 14 (D)       |             |            |                  |        | I5        | pulled-up                            |                        |
|                | 13 (C)       |             |            |                  |        | I6        | pulled-up                            |                        |
|                | 12 (B)       |             |            |                  |        | I7        | pulled-up                            |                        |
|                | 11 (A)       |             |            |                  |        | I8        | pulled-up                            |                        |
| GPIO34 (pin6)  |              |             |            |                  |        |           | factory button (to GND)              |                        |
|                |              |             |            |                  |        | H1        | GND                                  | GND                    |
| GPIO3 (pin34)  |              |             |            | 1 (RO via diode) |        | H2        | Uart Rx                              | Tx                     |
| GPIO1 (pin35)  |              |             |            | 4 (DI)           |        | H3        | Uart Tx                              | Rx                     |
|                |              |             |            |                  |        | H4        | /RST                                 | RTS                    |
| GPIO0 (pin25)  |              |             |            |                  | PWR    | H5        | (Set Low to enter serial bootloader) | DTR                    |
|                |              |             |            |                  |        | H6        | 3.3V                                 |                        |
| GPIO4 (pin26)  |              |             |            |                  |        | H7        | i2c/CAN_RX                           |                        |
| GPIO5 (pin29)  |              |             |            |                  |        | H8        | i2c/CAN_TX                           |                        |
| GPIO2 (pin24)  |              |             |            |                  |        |           | factory LED                          |                        |
| GPIO33 (pin9)  |              |             |            | 2,3              |        |           | RS485_DE flow control                |                        |
|                |              |             |            | 6 (A)            |        | A/H       | RS485 Rx                             |                        |
|                |              |             |            | 7 (B)            |        | B/L       | RS485 Tx                             |                        |
|                | 14 (D)       |             |            |                  |        | I5        | pulled-up                            |                        |
|                | 13 (C)       |             |            |                  |        | I6        | pulled-up                            |                        |
|                | 12 (B)       |             |            |                  |        | I7        | pulled-up                            |                        |
|                | 11 (A)       |             |            |                  |        | I8        | pulled-up                            |                        |
|                |              | 7 (H)       |            |                  |        | RELAY1    | active High                          |                        |
|                |              | 6 (G)       |            |                  |        | RELAY2    | active High                          |                        |
|                |              | 5 (F)       |            |                  |        | RELAY3    | active High                          |                        |
|                |              | 4 (E)       |            |                  |        | RELAY4    | active High                          |                        |
|                |              | 3 (D)       |            |                  |        | RELAY5    | active High                          |                        |
|                |              | 2 (C)       |            |                  |        | RELAY6    | active High                          |                        |
|                |              | 1 (B)       |            |                  |        | RELAY7    | active High                          |                        |
|                |              | 15 (A)      |            |                  |        | RELAY8    | active High                          |                        |
| GPIO23 (pin37) |              |             |            |                  | MDC    |           |                                      |                        |
| GPIO18 (pin30) |              |             |            |                  | MDIO   |           |                                      |                        |
| GPIO17 (pin28) |              |             |            |                  | clk    |           | clk to JL1101                        |                        |
| GPIO21 (pin33) |              |             |            |                  | TXEN   |           | clk to JL1101                        |                        |
| GPIO19 (pin31) |              |             |            |                  | TXD0   |           | clk to JL1101                        |                        |
| GPIO22 (pin36) |              |             |            |                  | TXD1   |           | clk to JL1101                        |                        |
| GPIO27 (pin12) |              |             |            |                  | CRS    |           | clk to JL1101                        |                        |
| GPIO25 (pin10) |              |             |            |                  | RXD0   |           | clk to JL1101                        |                        |
| GPIO26 (pin11) |              |             |            |                  | RXD1   |           | clk to JL1101                        |                        |


## Connectors

| V-  | V+  | GND |    I8    |    I7    |    I6     |    I5    |    I4     |   I3    |   I2    |    I1    | ETH | B/L | A/H | R7  | R8  |
| :-: | :-: | :-: | :------: | :------: | :-------: | :------: | :-------: | :-----: | :-----: | :------: | :-: | :-: | :-: | :-: | :-: |
|     |     |     |          |          |           |          |           |         |         |          |     |     |     |     |     |
|     |     |     | H8 (i2c) | H7 (i2c) | H6 (3.3V) | H5 (/BL) | H4 (/RST) | H3 (Tx) | H2 (Rx) | H1 (GND) |     |     |     |     |     |
|     |     |     |          |          |           |          |           |         |         |          |     |     |     |     |     |
| R1  |     |     |    R2    |          |           |    R3    |           |         |   R4    |          |     | R5  |     |     | R6  |


# Board flashing

## Entering serial bootloader

- /RST must be Low
- IO0 must be Low
- IO2 must be Low or Floating
- IO12 must be Low or Floating (selects flash voltage 3.3V, otherwise it's 1.8 and flash checksum is reported invalid)
- /RST must be released

## Note on cables

The serial cable <mark>must be 3.3V</mark>.

Use the USB/serial cable with short wires at 3.3V side.

For example FT232RG cable is not ok, whilst the board FT2232H with long usb cable and ~10cm 3.3V cables is fine.
The CP2102 chip is reported more robust.

Otherwise the programming may result in following error:
> A fatal error occurred: Failed to write to target RAM (result was 01070000: Operation timed out)


## Connecting the serial programming cable

- The I4 must be manually wired to GND
- All other signals shall be connected to the full serial port according to the table above, so the esptool automatically controls the lines

Example wiring for FT2232H module board:

| Header | FT2232H | Function               |
| :----- | :------ | :--------------------- |
| H1     | GND     | GND                    |
| H2     | ADBUS0  | Board Rx               |
| H3     | ADBUS1  | Board Tx               |
| H4     | ADBUS2  | Reset                  |
| H5     | ADBUS4  | Bootloader selection   |



### Test the connection

- Read chip id

```
esptool.py --port /dev/ttyUSB0 --chip esp32 chip_id

esptool.py v4.8.1
Serial port /dev/ttyUSB0
Connecting....
Chip is ESP32-D0WD-V3 (revision v3.1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: xx:xx:xx:xx:xx:xx
Uploading stub...
Running stub...
Stub running...
Warning: ESP32 has no Chip ID. Reading MAC instead.
MAC: xx:xx:xx:xx:xx:xx
Hard resetting via RTS pin...
```

- Read fuses (check if the chip is not locked)

```espefuse.py --port /dev/ttyUSB0 summary```

Refer to [espefuse.py](https://docs.espressif.com/projects/esptool/en/latest/esp32/espefuse/index.html) for output.


## Programming

```
esphome run --device /dev/ttyUSB0 dtr008v2io.yaml

INFO ESPHome 2025.5.2
INFO Reading configuration dtr008v2io.yaml...
WARNING GPIO15 is a strapping PIN and should only be used for I/O with care.
Attaching external pullup/down resistors to strapping pins can cause unexpected failures.
See https://esphome.io/guides/faq.html#why-am-i-getting-a-warning-about-strapping-pins
WARNING GPIO12 is a strapping PIN and should only be used for I/O with care.
Attaching external pullup/down resistors to strapping pins can cause unexpected failures.
See https://esphome.io/guides/faq.html#why-am-i-getting-a-warning-about-strapping-pins
WARNING GPIO5 is a strapping PIN and should only be used for I/O with care.
Attaching external pullup/down resistors to strapping pins can cause unexpected failures.
See https://esphome.io/guides/faq.html#why-am-i-getting-a-warning-about-strapping-pins
INFO Generating C++ source...
INFO Updating https://github.com/espressif/esp-protocols.git@mdns-v1.8.2
INFO Compiling app...
Processing dtr008v2io (board: esp32dev; framework: espidf; platform: https://github.com/pioarduino/platform-espressif32.git#51.03.07)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash
 - framework-espidf @ 3.50106.0 (5.1.6) 
 - tool-cmake @ 3.21.3 
 - tool-esptoolpy @ 4.8.1 
 - tool-mklittlefs @ 3.2.0 
 - tool-ninja @ 1.7.1 
 - tool-riscv32-esp-elf-gdb @ 12.1.0+20221002 
 - tool-xtensa-esp-elf-gdb @ 12.1.0+20221002 
 - toolchain-esp32ulp @ 2.35.0-20220830 
 - toolchain-xtensa-esp32 @ 12.2.0+20230208
Reading CMake configuration...
Dependency Graph
|-- ArduinoJson @ 6.18.5
RAM:   [=         ]  10.2% (used 33552 bytes from 327680 bytes)
Flash: [=====     ]  46.9% (used 859957 bytes from 1835008 bytes)
====================================================================== [SUCCESS] Took 4.59 seconds ======================================================================
INFO Successfully compiled program.
esptool.py v4.8.1
Serial port /dev/ttyUSB0
Connecting.....
Chip is ESP32-D0WD-V3 (revision v3.1)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: xx:xx:xx:xx:xx:xx
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Auto-detected Flash size: 16MB
Flash will be erased from 0x00010000 to 0x000e3fff...
Flash will be erased from 0x00001000 to 0x00007fff...
Flash will be erased from 0x00008000 to 0x00008fff...
Flash will be erased from 0x00009000 to 0x0000afff...
Compressed 866528 bytes to 563646...
Wrote 866528 bytes (563646 compressed) at 0x00010000 in 12.9 seconds (effective 536.3 kbit/s)...
Hash of data verified.
Flash params set to 0x0240
SHA digest in image updated
Compressed 26784 bytes to 16788...
Wrote 26784 bytes (16788 compressed) at 0x00001000 in 0.7 seconds (effective 297.5 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 134...
Wrote 3072 bytes (134 compressed) at 0x00008000 in 0.1 seconds (effective 383.4 kbit/s)...
Hash of data verified.
Compressed 8192 bytes to 31...
Wrote 8192 bytes (31 compressed) at 0x00009000 in 0.1 seconds (effective 653.9 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
INFO Successfully uploaded program.
INFO Starting log output from /dev/ttyUSB0 with baud rate 115200
```
If not using latest esptool with esptool.cfg with 'custom_hard_reset_sequence = D0|R1|W0.5|R0',
at this point disconnect the H3 then H4 cable, so the flashed firmware starts:
```
[13:40:55]\x80xets Jul 29 2019 12:21:46
[13:40:55]
[13:40:55]rst:0x1 (POWERON_RESET),boot:0x17 (SPI_FAST_FLASH_BOOT)
[13:40:55]configsip: 0, SPIWP:0xee
[13:40:55]clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
[13:40:55]mode:DIO, clock div:2
[13:40:55]load:0x3fff0030,len:7112
[13:40:55]load:0x40078000,len:15688
[13:40:55]load:0x40080400,len:4
[13:40:55]load:0x40080404,len:3876
[13:40:55]entry 0x4008064c
[13:40:55]I (29) boot: ESP-IDF 5.1.6 2nd stage bootloader
[...]
```


# References

## Board Components
- [WROOM](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32e_esp32-wroom-32ue_datasheet_en.pdf)
- [WROOM32 pinouts](https://lastminuteengineers.com/esp32-wroom-32-pinout-reference/)
- [sn74hc165](https://www.ti.com/lit/ds/symlink/sn74hc165.pdf)
- [sn74hc595](https://www.ti.com/lit/ds/symlink/sn74hc595.pdf)
- [sn74lvc1g04](https://www.ti.com/lit/ds/symlink/sn74lvc1g04.pdf)
- [BL3085B](https://www.alldatasheet.com/datasheet-pdf/download/741735/BELLING/BL3085B.html)

## Flashing
- [FT2232H](https://ftdichip.com/wp-content/uploads/2024/09/DS_FT2232H.pdf)
- [Bootloader manual](https://docs.espressif.com/projects/esptool/en/latest/esp32/advanced-topics/boot-mode-selection.html#manual-bootloader)
- [esptool.py](https://docs.espressif.com/projects/esptool/en/latest/esp32/esptool/index.html)
- [esptool.cfg](https://docs.espressif.com/projects/esptool/en/latest/esp32/esptool/configuration-file.html)
- [espefuse.py](https://docs.espressif.com/projects/esptool/en/latest/esp32/espefuse/index.html)
- [esphome](https://esphome.io/guides/cli.html)

## Links
- https://github.com/serg987/HomeAutomationMisc/tree/master/Dingtian-relay-8ch-ESPHome
- https://templates.blakadder.com/dingtian_DT-R008.html
- https://devices.esphome.io/devices/Dingtian-DT-R008
- https://github.com/kecajtop/dtr0xx_io
- https://www.flamingo-tech.nl/2021/03/21/installing-and-using-esptools/
- https://community.home-assistant.io/t/has-anyone-used-dingtian-8ch-ethernet-relay/248441

