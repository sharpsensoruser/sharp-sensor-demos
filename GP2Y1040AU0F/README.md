# README
Sharp GP2Y1040AU0F VCSEL Laser Dust Sensor Application Guide

## Overview
**GP2Y1040AU0F** is the brand new flagship model in Sharp's lineup of air quality sensors. 
Designed for use in commercial air purifiers, GP2Y1040AU0F is a VCSEL laser-based particle counting sensor with built-in fan.
The sensor outputs particle counts in the form of number concentrations for different ranges of particle sizes as well as
the corresponding dust density values such as PM2.5 and PM10.
<img align="right" src="https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1040au0f_img.png"></img>

### Features
* VCSEL laser (Vertical Cavity Surface Emitting Laser) light scattering method
* Built-in MCU and fan with automatic cleaning function
* Real-time output of mass concentration (dust density) in units of ug/m3
* Real-time output of number concentration (# of particles per cm3)
* High accuracy of plus or minus 10% based on TSI DustTrak II reference monitor
* Detect particle sizes down to 0.3um (microns)
* Supports both UART and I2C interfaces for input and output

### Applications
* Air purifier / conditioner
* Air quality monitor
* HVAC / ventilation
* Smart home sensor units
* Environmental monitoring

## Key concepts and terminology
![key concepts](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1040au0f_keyconcepts.png)

## Sensor placement and orientation
The Sharp GP2Y1040AU0F sensor has a rectangular hole on one side for air intake, as well as air outlets where the internal fan is located. Due to the path of airflow within the sensor, Sharp recommends that the sensor be placed in an upright position using either of the two orientations shown in the picture below.

![recommended sensor orientation](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1040au0f_placement.png)

Even though GP2Y1040AU0F has an automatic cleaning function (by using high-speed fan rotation), the sensor compartment cannot be opened up for manual cleaning. Therefore, Sharp recommends that your application should try to prevent large dust or other coarse particles from entering the sensor in the first place. For example, install a coarse mesh filter in front of the sensor to capture such particles.
