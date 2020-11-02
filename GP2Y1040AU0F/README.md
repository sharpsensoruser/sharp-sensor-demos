# README
Sharp GP2Y1040AU0F VCSEL Laser Dust Sensor Application Guide

## Overview
**GP2Y1040AU0F** is the brand new flagship model in Sharp's lineup of air quality sensors. 
Designed for use in home air purifiers, GP2Y1040AU0F is a VCSEL laser-based particle counting sensor with built-in fan.
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

Even though GP2Y1040AU0F has an automatic cleaning function (by using high-speed fan rotation), the sensor compartment cannot be opened up for manual cleaning. Therefore, Sharp recommends that your application should try to prevent large dust or other coarse particles from entering the sensor in the first place. For example, install a coarse mesh filter in front of the sensor to capture such incoming particles.

## GP2Y1040AU0F connector cable
Sharp GP2Y1040AU0F uses the 6-pin connector **50801W00-6P-S-HF** made by JCTC (Shenglan Technology Co. Ltd). The connector is located on the side of the sensor that is opposite to that of the air inlet and outlets.

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

## UART Specification
### Active versus passive mode
The GP2Y1040AU0F sensor has two modes of operation: **active** mode and **passive** mode. By default, the sensor is set to active mode which means it will continuously output air quality data records/packets approximately once per second through its TXD transmitting pin. If your application only ever needs to read from the sensor in active mode, you can actually leave the sensor's RXD receiving pin unconnected.

### Reading UART data records from GP2Y1040AU0F
In active mode, GP2Y1040AU0F transmits a UART data record about once every second. Each data record consists of 28 data frames, where each data frame is 1 byte. The first two frames of each data record are the bytes 0xFF and 0xFA. You can look for these bytes in your code to know when is the start of a new data record. Each data record contains the air quality measurement quantities such as **number concentrations** (particle counts) and calculated **mass concentrations**. The following table describes the 28-byte UART data record format.

| Byte Number | Field | Format | Description | Units |
|--------|-------|--------|-------------|-------|
| 0 | Start Frame 1 | 0xFF | Indicates start of data record | - |
| 1 | Start Frame 2 | 0xFA | Indicates start of data record | - |
| 2-3 | NC_03 | unsigned int (16-bit) big-endian | Number concentration of particles with size 0.3um to 10um | 0.1 particles / cm3 |
| 4-5 | NC_05 | unsigned int (16-bit) big-endian | Number concentration of particles with size 0.5um to 10um | 0.1 particles / cm3 |
| 6-7 | NC_1 | unsigned int (16-bit) big-endian | Number concentration of particles with size 1.0um to 10um | 0.1 particles / cm3 |
| 8-9 | NC_25 | unsigned int (16-bit) big-endian | Number concentration of particles with size 2.5um to 10um | 0.1 particles / cm3 |
| 10-11 | NC_4 | unsigned int (16-bit) big-endian | Number concentration of particles with size 4.0um to 10um | 0.1 particles / cm3 |
| 12-13 | Reserved | - | Reserved for future expansion | - |
| 14-15 | PM1_1 | unsigned int (16-bit) big-endian | Mass concentration of particles with size 0.3um to 1.0um (standard particle) | ug/m3 |
| 16-17 | PM25_1 | unsigned int (16-bit) big-endian | Mass concentration of particles with size 0.3um to 2.5um (standard particle) | ug/m3 |
| 18-19 | PM10_1 | unsigned int (16-bit) big-endian | Mass concentration of particles with size 0.3um to 10um (standard particle) | ug/m3 |
| 20-21 | PM1_2 | unsigned int (16-bit) big-endian | Mass concentration of particles with size 0.3um to 1.0um (cigarette smoke) | ug/m3 |
| 22-23 | PM25_2 | unsigned int (16-bit) big-endian | Mass concentration of particles with size 0.3um to 2.5um (cigarette smoke) | ug/m3 |
| 24-25 | PM10_2 | unsigned int (16-bit) big-endian | Mass concentration of particles with size 0.3um to 10um (cigarette smoke) | ug/m3 |
| 26 | Reserved | - | Reserved for future expansion | - |
| 27 | CheckSum | unsigned int (8-bit) | Check sum (8 bit) of previous 27 bytes | - |

The number concentration values are in units of 0.1 particles per cubic centimetre. For example, if the NC_03 value is reported as 0x03E8 (= 1000), then corresponding number concentration is 1000 x 0.1 = 100 particles/cm3.

The PM1_1, PM25_1, and PM10_1 mass concentration values are pre-calibrated by using a *standard particle* such as **KCl** as a model.
The PM1_2, PM25_2, and PM10_2 mass concentration values are pre-calibrated by using *cigarette smoke* as a model. If these models are different from the types of particles you are measuring, you can do the calibration yourself by starting with the number concentrations (which are essentially particle counts) and calculating the corresponding PM values yourself. For example, on a graph you can plot number concentration values on the X-axis and mass concentration values on the Y-axis in units of ug/m3. The Y-axis values can be determined by yourself using your own particle model or by referencing an already calibrated air quality monitor such as TSI DustTrak II 8530.

### Sending UART commands to GP2Y1040AU0F

You can send a UART command to the GP2Y1040AU0F sensor in order to switch from active to passive mode. While in passive mode, the sensor doesn't continually send UART data records. Instead, you can issue a subsequent command to read the air quality data one time. You can also issue other commands such as performing a software reset, or change settings on the sensor such as the number of moving averages to use.

Each UART command consists of 7 data frames (or bytes). The first two bytes are always 0x42 and 0x4D. The following table describes the UART command format.

| Byte Number | Field |  Description |
|-------------|-------|--------------|
| 0 | 0x42 | Indicate start of command record |
| 1 | 0x4D | Indicate start of command record |
| 2 | CMD | Command code |
| 3 | DATAH | Data value to set (hi byte) |
| 4 | DATAL | Data value to set (lo byte) |
| 5 | CSH | CheckSum (hi byte) |
| 6 | CSL | CheckSum (lo byte) |

The CheckSum is 16-bits long and is calculated as: **CheckSum = 0x42 + 0x4D + CMD + DATAH + DATAL**.

The following table describes some example UART commands you can send.

| CMD | DATAH | DATAL | Cmd Name | Description | Example bytes (HEX) |
|-----|-------|-------|--------------|-------------|---------------------|
| 0xE2 | 0x00 | 0x00 | READ | Read data in passive mode | 42 4D E2 00 00 01 71 |
| 0xE1 | 0x00 | 0x00 | MODE | Switch to passive mode | 42 4D E1 00 00 01 70 |
| 0xE1 | 0x00 | 0x01 | MODE | Switch to active mode | 42 4D E1 00 01 01 71 |
| 0xE4 | 0x00 | 0x00 | SLEEP | Go to sleep | 42 4D E4 00 00 01 73 |
| 0xE4 | 0x00 | 0x01 | SLEEP | Wake up | 42 4D E4 00 01 01 74 |
| 0x01 | 0x00 | 0x00 | CLEAN | Start cleaning | 42 4D 01 00 00 00 90 |
| 0x02 | 0x81 | 0x00 | RESET | Software reset | 42 4D 02 81 00 01 12 |
| 0x03 | 0x00 | 0x0A | MAVE | Set number of moving averages to 10 | 42 4D 03 00 0A 00 9C |
| 0x04 | 0x00 | 0x00 | TINT | Set interval time to 0 seconds | 42 4D 04 00 00 00 93 |
| 0x05 | 0x00 | 0x03 | TPREFAN | Set pre-rotation time of fan to 3 seconds | 42 4D 05 00 03 00 97 |
| 0x06 | 0xEC | 0x40 | TINTC | Set interval time for auto-cleaning to 60480[10s] | 42 4D 06 EC 40 01 C1 |
| 0x07 | 0x00 | 0x0A | TCLEAN | Set cleaning time to 10 seconds | 42 4D 07 00 0A 00 A0 |

Commands to change settings on the sensor such as MAVE or TCLEAN require you to put the sensor into passive mode first. Once you send a command such as MAVE to the sensor, you will receive back the exact same command (i.e. you will read back a 7-byte record from the sensor).

### UART Demo: Arduino

In this [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/GP2Y1040AU0F/sharp_gp2y1040au0f_demo_uart.ino), the Arduino hardware serial port is used only for printing output to the Arduino Serial Monitor at 9600 baud. The actual UART communication with the GP2Y1040AU0F sensor is done through a software serial port connected to Arduino digital pins 10 and 11. However, because the Arduino digital pins use 5V while the GP2Y1040AU0F TXD and RXD pins use 3.3V logic, it is necessary to use a 3.3V-to-5V logic level converter in the circuit. The following figure shows the connections using Arduino Uno.

![GP2Y1040AU0F Arduino circuit for UART](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1040au0f_circuit.png)

The following screen capture shows example output from the GP2Y1040AU0F dust sensor as displayed on the Arduino Serial Monitor.

![GP2Y1040AU0F Arduino serial monitor output](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1040au0f_monitor.png)

### UART Demo: Windows/CSharp

This is a [C#/.NET Windows Forms application](https://github.com/sharpsensoruser/sharp-sensor-demos/tree/master/GP2Y1040AU0F/sharp_gp2y1040au0f_demo_csharp) for controlling the Sharp GP2Y1040AU0F Dust Sensor. The Sharp sensor must be connected to a FT232RL serial-to-USB adapter which connects to your Windows PC. The USB adapter must support 3.3V RX/TX and 5V VCC.

The application has various options for logging the UART data to a log file and to the screen.

![Sharp GP2Y1040Au0F CSharp Demo App Circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1040au0f_csharp_circuit.png)

![Sharp GP2Y1040Au0F CSharp Demo App Screen](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1040au0f_csharp_app.png)

## See also
* [Sharp GP2Y1040AU0F Specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/Sensors/Dust%20Sensor/GP2Y1040AU0F_spec.pdf)
* [Sharp Dust Sensors Lineup](http://www.socle-tech.com/SHARP_sensor_Dust%20Sensor.php)
