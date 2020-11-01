# README
Sharp GP2Y1040AU0F VCSEL Laser Dust Sensor Application Guide

## Overview
**GP2Y1040AU0F** is the brand new flagship model in Sharp's lineup of air quality sensors. 
Designed for use in commercial air purifiers, GP2Y1040AU0F is a VCSEL laser-based particle counting sensor with built-in fan.
The sensor outputs particle counts in the form of number concentrations for different ranges of particle sizes as well as
the corresponding dust density values such as PM1, PM2.5 and PM10.
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

## Compare Sharp GP2Y1030AU0F and GP2Y1040AU0F

| Specification | GP2Y1030AU0F | GP2Y1040AU0F |
|---------------|--------------|--------------|
| Light emitting element | LED | VCSEL laser diode |
| Method of operation | Particle counter | Particle counter |
| Minimum detectable dust size | 1um | 0.3um |
| Dust density sensing range | up to 500ug/m3 | up to 1000ug/m3 |
| Sensing of different dust sizes | Yes, counts particles < 2.5um, > 2.5um, and total | Yes, outputs PM1, PM2.5, PM10 as well as particle concentrations for several size ranges |
| Startup time | < 5 sec | < 5 sec |
| Accuracy | +/-15% | +/-10% |
| Interface | UART | UART or I2C |
| User maintenance | Removable panel for cleaning | Auto-cleaning by high-speed fan rotation |

## Sensor placement and orientation
The Sharp GP2Y1040AU0F sensor has a rectangular hole on one side for air intake, as well as air outlets where the internal fan is located. Due to the path of airflow within the sensor, Sharp recommends that the sensor be placed in an upright position using either of the two orientations shown in the picture below.

![recommended sensor orientation](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1040au0f_placement.png)

Even though GP2Y1040AU0F has an automatic cleaning function (by using high-speed fan rotation), the sensor compartment cannot be opened up for manual cleaning. Therefore, Sharp recommends that your application should try to prevent large dust or other coarse particles from entering the sensor in the first place. For example, install a coarse mesh filter in front of the sensor to capture such particles.

## GP2Y1040AU0F connector cable
Sharp GP2Y1040AU0F uses the 6-pin connector **50801W00-6P-S-HF** made by JCTC (Shenglan Technology Co. Ltd). The connector is on the side of the sensor that is opposite to the air inlet and outlets.

![GP2Y1040AU0F connector](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1040au0f_connector.png)

The following table shows the pin assignments for GP2Y1040AU0F.

| Pin number | Pin name | Description |
|------------|----------|-------------|
| 1 | TXD / SDA | 3.3V logic UART / I2C |
| 2 | RXD / SCL | 3.3V logic UART / I2C |
| 3 | SEL | Interface select (for UART: set to floating or 3.3V, for I2C: set to GND) |
| 4 | GND | Ground |
| 5 | Vcc | Supply Voltage 5V +/-10% |
| 6 | GND | Ground |

## UART Example: Arduino sketch

## UART Example: Windows/CSharp application

