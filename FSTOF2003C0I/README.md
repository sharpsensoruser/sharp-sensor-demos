# README
Foxconn FSTOF2003C0I Time of Flight Sensor Module Application Guide

## Overview
**FSTOF2003C0I** is the new **module** version of Sharp's **GP2AP03VT00F** Time-of-Flight (**TOF**) sensor which is designed to measure absolute distances from 1cm to 30cm (white card). Sharp GP2AP03VT00F is a very compact I2C sensor designed for short distance ranging applications.

![TOF sensor and module](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/foxconn_fstof2003c0i_tofsensor.png)

The **TOF Module** has an on-board MCU, includes a cover lens for the TOF sensor, and provides a digital **I2C** interface. The FSTOF2003C0I module can be used in a variety of distance ranging applications such as:
* Automatic faucets and dispensers
* Wall and floor detection for robot vacuums

The TOF Module is also available in a UART version instead of I2C. The following table summarizes the different versions of the TOF module that are available.

| TOF Module | Distance Range|TOF Sensor|Interface|
|-------------|-------------|-------------|---------|
|FSTOF2002C0U|10cm to 240cm (white card)|Sharp GP2AP02VT00F|UART|
|FSTOF2002C0I|10cm to 240cm (white card)|Sharp GP2AP02VT00F|I2C|
|FSTOF2003C0U|1cm to 30cm (white card)|Sharp GP2AP03VT00F|UART|
|FSTOF2003C0I|1cm to 30cm (white card)|Sharp GP2AP03VT00F|I2C|

### TOF Sensor Eye Safety Considerations

The Sharp TOF sensor uses a [Class 1 Laser](https://en.wikipedia.org/wiki/Laser_safety#Class_1) which is safe under conditions of normal use. As per the [specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/SHARP_GP2AP03VT00F_Specification.pdf) for Sharp GP2AP03VT00F, the laser output from the TOF sensor is Class 1 laser safety under all reasonably foreseeable including single faults in compliance with IEC 60825-1:2014. The laser output will remain within Class 1 limits as long as the Sharp recommended device settings are used and the operating conditions specified in the Sharp GP2AP03VT00F specification are respected. The laser output power must not be increased by any means and no optics should be used with the intention of focusing the laser beam.

![TOF sensor Class 1 laser safety](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_lasersafety.png)

## TOF Module Specifications

### Module Appearance

The TOF Module is comprised of a PCBA board with MCU and a 4-pin connector. On the opposite side of the connector is the actual TOF sensor which is encased behind a rectangular cover and lens.

![TOF module outline](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/foxconn_fstof2002c0u_appearance.png)

### Module Connector

FSTOF2003C0I uses a **4-pin connector** made by [JCTC (Shenglan Technology Co. Ltd)](http://www.jctc.com.cn/):

* Module-side connector: JCTC 11002W00-4P-S-HF
* Mating connector housing: JCTC 11002H00-4P-HF
* Mating connector terminal: JCTC 11002TOP-2E

![TOF module connector pins](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/foxconn_fstof2003c0i_connectorpins.png)

Note that JCTC connectors may be difficult to source in North America. As an alternative, try the [JST SH series 1.0mm pitch 4-pin connector](http://www.jst-mfg.com/product/pdf/eng/eSH.pdf) which should be compatible with JCTC. JST connectors are easy to find on ebay and you can purchase a set of 10 JST cable assemblies for just a few dollars.

### Field of View (FOV)

The FSTOF2003C0I cover lens gives it a standard Field-of-View (FOV) of **25 degrees**, which is suitable for a wide range of applications such as user or object detection. 

### I2C Data Packet Format

Command codes and data bytes are received from the TOF module using the following data packet format.

| Field Offset (bytes) | Field Length (bytes)|Field Type | Description |
|-------------|-------------|-------------|---------|
|0|1|uint8_t|Command Code|
|1|1|uint8_t|Data Length|
|2|N-2|uint8_t|Raw Data: The data length is different for each command. The computational formula is N = Data Length + 2.|

### I2C Command Codes

The following table shows the available I2C command codes which can be transmitted to the TOF module. For example, to request a distance measurement/reading, send the **0x81** command code. The TOF module will return a distance in units of MM = millimetres using the Data Packet Format described in the previous section.

| Command Code | Description |
|-------------|-------------|
| 0x81 | Measure distance in units of mm |
| 0x82 | Crosstalk calibration |
| 0x83 | Offset calibration |
| 0x84 | Reset TOF module |
| 0x85 | Read factory data |
| 0x86 | Read version information |
| 0x8A | Read debug parameter section 1 |
| 0x8B | Read debug parameter section 2 |
| 0x8C | Read debug parameter section 3 |
| 0x91 | Set the threshold of interrupt output |
| 0x92 | Set the ranging mode |

Here are some examples of sending various command codes, and the received data.

| Command Description| Command Code | Receive Data
|-------------|-------------|-------------|
| Measure a distance of 50cm | 0x81 | 0x81 0x03 0x01 0xF4 0x00
| Read version info | 0x86 | 0x86 0x03 0x03 0x41 0x04

## Using FSTOF2003C0I with Arduino

### Arduino Circuit

Connect the TOF module to your Arduino board as shown in the figure below. This example uses **Arduino Uno** but it should work the same with Arduino **Mega 2560**. Note that the TOF module requires a power supply voltage between **3.0V and 3.6V**. In addition, the SDA and SCL pins of the TOF module operate on **3.3V logic**, so it is necessary to use a logic level converter to convert between 3.3V and the 5V that Arduino uses.

![Arduino circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/foxconn_fstof2003c0i_circuit.png)

The [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/FSTOF2003C0I/foxconn_fstof2003c0i_demo.ino) shows how to request a distance measurement by sending the **0x81** command code to the TOF module, process the returned data from the module, and then display the distance in millimetre units on the Serial Monitor.

### Example Serial Monitor Output

If you are switching between different Arduino boards, make sure you go to the Arduino IDE **Tools** menu and set **Board** and **Port** appropriately before compiling or uploading the code. The following screen capture shows example output from the TOF module as displayed on the Arduino Serial Monitor.

![Example serial monitor output](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/foxconn_fstof2003c0i_monitor.png)

## See also
* [TOF Module Product Page](http://www.socle-tech.com/Socle_ToF_Module%20.php)
* [FSTOF2003C0I Specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/FSTOF2003C0x%20ToF%20module%20Preliminary%20V3_EN.pdf)
* [FSTOF2003C0I Application Note](http://www.socle-tech.com/doc/IC%20Channel%20Product/FSTOF200xC0x%20ToF%20module%20Preliminary%20application%20guide%20.pdf)
* [GP2AP03VT00F Specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/SHARP_GP2AP03VT00F_Specification.pdf)

