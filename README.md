USB Meter
=========

This is the repository for my KW203 USB meter mod.
This mod is provided as is. This is a mod, so no full schematics.

The KW203 USB meter like most of those in the price range have very lousy
resolution and cannot read low currents.  This mod is replacing the
unknown microcontroller with a STM8S003F3 and a few modification to 
improve on the resolution and accuracy. The modifications and the 
calibration is what makes this a usable test equipment.

This is a hack that depends on the existing questionable components i.e. 
3V LDO (+/-3%) and current sensing resistor (??? %). 

Each unit has to be calibrated against a multimeter with a linear 
regression for reasonable accuracy. The current measurement suffers
from non-linearity at high current due to heating effects on the sense 
resistor and the bad layout traces. Trimppot won't help here.  To manage 
the errors, I use a piecewise linear fit. 

Firmware is written for Cosmic C which ST arranged to have for free use 
with their STM8 chips.

Project page: mod, documentations, photos

https://hw-by-design.blogspot.com/2019/08/usb-meter-modding.html

https://hw-by-design.blogspot.com/2019/09/usb-meter-modding-part-2.html

https://hw-by-design.blogspot.com/2019/10/uc-mod-pcb-part-3.html

Licensing:

Schematic/PCB: Creative Commons 4.0 Attribution-ShareAlike 4.0 International

Firmware: source code: GPL v3

Updates:

New PCB for the adapter board - verified.
