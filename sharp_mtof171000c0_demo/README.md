# README
Sharp MTOF171000C0 Time-of-Flight Sensor Module Application Guide

## Overview
**MTOF171000C0** is the new **module** version of Sharp's **GP2AP01VT10F** Time-of-Flight (**TOF**) sensor which is designed to measure absolute distances from 2cm to 120cm. GP2AP01VT10F is a very compact I2C sensor commonly used in smartphone applications such as camera auto-focus.

![TOF sensor and module](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_tofsensor.png)

The **TOF Module** is more than simply a breakout board for the TOF sensor. The module incorporates a cover lens, has a connector-based design with mounting holes, and provides a digital **UART** interface in addition to I2C. The MTOF171000C0 module can be used in applications such as:
* Automatic faucets
* User detection for ATMs and kiosks
* Robotics obstacle detection
* Copier and vending machines
* Automatic control of lighting
* Factory automation
* Robotic vacuums

![TOF module with ruler](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_tofsensorb.png)

_MTOF171000C0 is designed and manufactured by Socle Technology Corporation, which is a Foxconn subsidiary and the official supplier of Sharp optoelectronic components and sensors in North America, China, and Taiwan_.

## PSD versus TOF Sensors

Sharp's **DMS** (Distance Measuring Sensor) models such as GP2Y0A41SK0F and GP2Y0A02YK0F use **PSD** (Position Sensitive Detector) technology to determine the distance to an object. This type of sensor emits an **IR** (Infra-Red) beam of light which reflects off a target object and is received back at the sensor's PSD. The reflected light is detected at a different location on the PSD depending on the distance to the target. These locations on the PSD correspond to different current and output voltage values which can then be used to compute the distance.

In contrast, Sharp's GP2AP01VT10F Time-of-Flight sensor uses a **Class 1 laser** with a wavelength of **940nm**. This TOF sensor emits a laser pulse and measures the time it takes for the light to be reflected off a target object and received back at the sensor. The well-known formula, _distance = speed x time_, is then used to compute the absolute distance, where _speed_ equals the **speed-of-light**.

From the comparison table below, you can see that the key advantage of the TOF sensor is high accuracy throughout its sensing range.

![compare PSD with TOF sensor](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_comparepsd.png)

By using a different cover lens, the TOF sensor FOV can be changed as needed. For example, it is possible to configure a narrow FOV of just 7 degrees.

### TOF Sensor Eye Safety Considerations

The Sharp TOF sensor uses a [Class 1 Laser](https://en.wikipedia.org/wiki/Laser_safety#Class_1) which is safe under conditions of normal use. As per the [specification/datasheet](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/Sensors/TOF%20Sensor/GP2AP01VT10F_Specification(ED-17G005)_final_w_conf%20For%20normal.pdf) for GP2AP01VT10F, the laser output from the TOF sensor is Class 1 laser safety under all reasonably foreseeable including single faults in compliance with IEC 60825-1:2014. The laser output will remain within Class 1 limits as long as the Sharp recommended device settings are used and the operating conditions specified in the GP2AP01VT10F datasheet are respected. The laser output power must not be increased by any means and no optics should be used with the intention of focusing the laser beam.

![TOF senser Class 1 laser safety](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_lasersafety.png)

## TOF Module Specifications

### Module Outline

The TOF Module is comprised of a PCBA board with MCU and a 6-pin connector. The PCBA board is affixed to a base that has two screw holes for application mounting purposes. On the opposite side of the base is the actual TOF sensor which is encased behind an oval cover and lens.

![TOF module outline](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_outline.png)

### Module Connector

MTOF171000C0 uses a **6-pin connector** made by [JCTC (Shenglan Technology Co. Ltd)](http://www.jctc.com.cn/):

* Module-side connector: JCTC 11002W00-6P-S-HF
* Mating connector housing: JCTC 11002H00-6P-HF
* Mating connector terminal: JCTC 11002TOP-2E

![TOF module connector pins](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_connectorpins.png)

Pins 5 and 6 are for **I2C** communication which is not described in this guide. By default, only the UART interface is enabled. I2C support must be enabled via firmware update. You can find more information about the I2C interface from the [MTOF171000C0 Application Note](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/ToF%20Module%20MTOF171000C0%20Application%20Notes.pdf).

Note that JCTC connectors may be difficult to source in North America. As an alternative, try the [JST SH series 1.0mm pitch 6-pin connector](http://www.jst-mfg.com/product/pdf/eng/eSH.pdf) which should be compatible with JCTC. JST connectors are easy to find on ebay and you can purchase a set of 10 JST cable assemblies for just a few dollars.

### Field of View (FOV)

The MTOF171000C0 cover lens gives it a Field-of-View (FOV) of **25 degrees**, which is suitable for a wide range of applications such as user or object detection. 

There is also an upcoming model **MTOF171010C0** with a narrow FOV of **7 degrees**. This model is applicable to ranging applications in a more confined space.

### Serial Port Protocol Settings

The following are the serial port settings for communicating with the TOF module through its UART interface.

![TOF module serial port protocol settings](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_serialportsettings.png)

### UART Command and Data Transmission Format

Command and data bytes are transmitted to (or received from) the TOF module using the following record format.

![TOF module command and data format](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_dataformat.png)

Note that the checksum is computed by adding the bytes from CMD to DATn, and then taking the lower 8 bits of the sum.

### UART Command List

The following table shows the available UART command instructions which can be transmitted to the TOF module. For example, to request a distance measurement/reading, send the **CMD_RD_MM** (0xD3) instruction. The TOF module will return a distance in units of MM = millimetres using the Data Transmission Format described in the previous section.

![TOF module command list](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_commands.png)

Here are some examples of sending various command instructions, and the received data.

![TOF module command examples](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_cmdexamples.png)

## Using MTOF171000C0 with Arduino

### Arduino Circuit

Connect the TOF module to your Arduino board as shown in the figure below. This example uses **Arduino Uno** but it should work the same with Arduino **Mega 2560**. Note that the TOF module requires a power supply voltage between **3.0V and 3.6V**. In addition, the TXD and RXD pins of the TOF module operate on 3.3V logic, so it is necessary to use a logic level converter to convert between 3.3V and the 5V that Arduino uses.

In the circuit, the hardware serial port is used only for printing output distances to the Arduino Serial Monitor. A **software serial port** is used for the actual communication with the TOF module (Arduino pins 10 and 11) as shown in the [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_mtof171000c0_demo/sharp_mtof171000c0_demo.ino). If you're using Arduino Mega, you can use one of the other hardware serial ports instead such as **Serial1** for communicating with the TOF module.

![Arduino circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_circuit.png)

The [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_mtof171000c0_demo/sharp_mtof171000c0_demo.ino) shows how to request a distance measurement by sending the **D3** command instruction (CMD_RD_MM) to the TOF module, process the returned data from the module, and then display the distance in millimetre units on the Serial Monitor.

![Arduino circuit photo](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_circuitphoto.png)

### Example Serial Monitor Output

If you are switching between different Arduino boards, make sure you go to the Arduino IDE **Tools** menu and set **Board** and **Port** appropriately before compiling or uploading the code. The following screen capture shows example output from the TOF module as displayed on the Arduino Serial Monitor.

![Example serial monitor output](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_mtof171000c0_monitor.png)

## See also
* [TOF Module Product Page](http://www.socle-tech.com.tw/Socle_ToF_Module%20.php)
* [MTOF171000C0 Specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/MTOF171000C0%20ToF%20module%20specification.pdf)
* [MTOF171000C0 Application Note](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/ToF%20Module%20MTOF171000C0%20Application%20Notes.pdf)
* [GP2AP01VT10F Specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/Sensors/TOF%20Sensor/GP2AP01VT10F_Specification(ED-17G005)_final_w_conf%20For%20normal.pdf)
