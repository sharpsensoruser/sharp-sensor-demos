# README
Application Guide for Foxconn FSTOF2002C0D Time of Flight Sensor Module

## Overview
**FSTOF2002C0D** is the new **module** version of Sharp's **GP2AP02VT00F** Time-of-Flight (**TOF**) sensor which is designed to measure absolute distances from 10cm to 240cm (white card). Sharp GP2AP02VT00F is a very compact I2C sensor which can be used in mid-range applications such as camera auto-focus.

![TOF sensor and module](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/foxconn_fstof2002c0d_tofsensor.png)

The **TOF Module** comprises a PCBA board with two mounting holes, a cover lens for the TOF sensor, and a 6-pin connector to support both UART and I2C interfaces. The FSTOF2002C0D module can be used in a variety of distance ranging applications such as:
* Automatic faucets and flush valves
* User detection for ATMs and kiosks
* Robotics obstacle detection
* Copier and vending machines
* Automatic control of lighting
* Factory automation
* Robotic vacuums

The following table summarizes the different versions of the TOF module that are available.

| TOF Module | Distance Range|TOF Sensor|Connector|Interface|
|------------|---------------|----------|---------|---------|
|FSTOF2002C0U|10cm to 240cm (white card)|Sharp GP2AP02VT00F|4-pin|UART|
|FSTOF2002C0I|10cm to 240cm (white card)|Sharp GP2AP02VT00F|4-pin|I2C|
|FSTOF2002C0D|10cm to 240cm (white card)|Sharp GP2AP02VT00F|6-pin|UART / I2C|
|FSTOF2003C0U|1cm to 30cm (white card)|Sharp GP2AP03VT00F|4-pin|UART|
|FSTOF2003C0I|1cm to 30cm (white card)|Sharp GP2AP03VT00F|4-pin|I2C|

### TOF Sensor Eye Safety Considerations

The Sharp TOF sensor uses a [Class 1 Laser](https://en.wikipedia.org/wiki/Laser_safety#Class_1) which is safe under conditions of normal use. As per the [specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/SHARP_GP2AP02VT00F_Specification.pdf) for Sharp GP2AP02VT00F, the laser output from the TOF sensor is Class 1 laser safety under all reasonably foreseeable including single faults in compliance with IEC 60825-1:2014. The laser output will remain within Class 1 limits as long as the Sharp recommended device settings are used and the operating conditions specified in the Sharp GP2AP02VT00F specification are respected. The laser output power must not be increased by any means and no optics should be used with the intention of focusing the laser beam.

![TOF sensor Class 1 laser safety](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_lasersafety.png)

## TOF Module Specifications

### Module Appearance

The TOF Module is comprised of a PCBA board with MCU and a 6-pin connector. On the opposite side of the connector is the actual TOF sensor which is encased behind a rectangular cover and lens.

![TOF module outline](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/foxconn_fstof2002c0d_appearance.png)

### Module Connector

FSTOF2002C0D uses a **6-pin connector** made by [JCTC (Shenglan Technology Co. Ltd)](http://www.jctc.com.cn/):

* Module-side connector: JCTC 11002W00-6P-S-HF
* Mating connector housing: JCTC 11002H00-6P-HF
* Mating connector terminal: JCTC 11002TOP-2E

![TOF module connector pins](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/foxconn_fstof2002c0d_connectorpins.png)

Note that JCTC connectors may be difficult to source in North America. As an alternative, try the [JST SH series 1.0mm pitch 6-pin connector](http://www.jst-mfg.com/product/pdf/eng/eSH.pdf) which should be compatible with JCTC. JST connectors are easy to find on ebay and you can purchase a set of 10 JST cable assemblies for just a few dollars.

### Field of View (FOV)

The FSTOF2002C0D cover lens gives it a standard Field-of-View (FOV) of **25 degrees**, which is suitable for a wide range of applications such as user or object detection. 

## UART Interface

The UART Specification for FSTOF2002C0D is exactly the same as for the 4-pin connector model FSTOF2002C0U. The same Arduino UART code and Windows applications apply to both models. Refer to the _Using FSTOF2002C0U with Arduino_ section of the application guide below.
* [Application Guide for FSTOF2002C0U Time of Flight Sensor Module](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Foxconn-FSTOF2002C0U-Time-of-Flight-Sensor-Module) (see section _Using FSTOF2002C0U with Arduino_)
* [Arduino demo code for FSTOF2002C0U / FSTOF2002C0D](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/FSTOF2002C0U/foxconn_fstof2002c0u_demo.ino)
* [Windows application #1 - TofDemoShow](https://github.com/sharpsensoruser/sharp-sensor-demos/tree/master/bin)
* [Windows application #2 - FoxconnTofModuleDemo with C#/.NET source code](https://github.com/sharpsensoruser/sharp-sensor-demos/tree/master/FSTOF2002C0U/foxconn_fstof2002c0u_demo_csharp)

## I2C Interface

The I2C Specification for FSTOF2002C0D is almost exactly the same as for the 4-pin connector model FSTOF2003C0I. The only difference is the SDA and SCL pins for FSTOF2002C0D are pins 5 and 6, respectively. The same Arduino I2C code applies to both models. Refer to the _Using FSTOF2003C0I with Arduino_ section of the application guide below.
* [Application Guide for FSTOF2003C0I Time of Flight Sensor Module](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Foxconn-FSTOF2003C0I-Time-of-Flight-Sensor-Module) (see section _Using FSTOF2003C0I with Arduino_)
* [Arduino demo code for FSTOF2003C0I / FSTOF2002C0D](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/FSTOF2003C0I/foxconn_fstof2003c0i_demo.ino)

## See also
* [TOF Module Product Page](http://www.socle-tech.com/Socle_ToF_Module%20.php)
* [FSTOF2002C0D Specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/FSTOF2002C0D%20ToF%20module%20Preliminary%20V1.0.pdf)
* [FSTOF2002C0D Application Note](http://www.socle-tech.com/doc/IC%20Channel%20Product/FSTOF200xC0x%20ToF%20module%20Preliminary%20application%20guide_EN.pdf)
* [GP2AP02VT00F Specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/SHARP_GP2AP02VT00F_Specification.pdf)
