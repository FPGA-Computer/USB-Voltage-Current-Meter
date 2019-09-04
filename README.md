USB Meter
=========

This is the repository for my KW203 USB meter mod for.
This mod is provided as is. This is a mod, so no PCB or full schematics.

The KW203 USB meter like most of those in the price range have very lousy
resolution and cannot read low currents.  This mod is replacing the
unknown microcontroller with a STM8S003F3 and a few modification to 
improve on the resolution and accuracy.

Since this is a hack that depends on the existing components: 
3V LDO (+/-3%) and current sensing resistor (??? %), each unit has to be 
calibrated against a multimeter.

Firmware is written for Cosmic C which ST arranged to have for free use 
with their STM8 chips.

Project page: mod, documentations, photos

https://hw-by-design.blogspot.com/2019/08/usb-meter-modding.html
https://hw-by-design.blogspot.com/2019/09/usb-meter-modding-part-2.html

Licensing:
Schematic: Creative Commons 4.0 Attribution-ShareAlike 4.0 International
firmware source code: GPL v3
