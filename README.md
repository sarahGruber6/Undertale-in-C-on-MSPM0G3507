# Undertale-in-C-on-MSPM0G3507

# DAC runs on PB0 through PB4

# Two buttons initialized and connected to PB6 and PB12

# Joystick is very confusing. Connecting x to PB19 and y to PA22 works, however it is NOT initialized to this, and the ports it is initialized to don't work. I'm not sure why this happens, but... whatever.
# There is joystick button code, however my joystick was broken so I'm unsure if it works.

# LCD connections:
# For ST7735
# LED-   (pin 16) TFT, to ground
# LED+   (pin 15) TFT, to +3.3 V
# SD_CS  (pin 14) SDC, to chip select
# MOSI   (pin 13) SDC, to MOSI
# MISO   (pin 12) SDC, to MISO
# SCK    (pin 11) SDC, to serial clock
# CS     (pin 10) TFT, to PB6  SPI1 CS0
# SCL    (pin 9)  TFT, to PB9  SPI1 SCLK
# SDA    (pin 8)  TFT, to PB8  MOSI SPI1 PICO
# A0     (pin 7)  TFT, to PA13 Data/Command, high for data, low for command
# RESET  (pin 6)  TFT, to PB15 reset (GPIO), low to reset
# NC     (pins 3,4,5)
# VCC    (pin 2)       to +3.3 V
# GND    (pin 1)       to ground

# if you have any questions or need help feel free to message me here or on Instagram @germanwaffles
