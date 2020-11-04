# README
Sharp A1DU5P2CP005B VOCs Sensor Module Application Guide

## Overview
<img align="right" src="https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp005b_img.png"></img> **A1DU5P2CP005B** is a new sensor module from Sharp Corporation for measuring the concentration of harmful substances in the air such as **formaldehyde** (HCHO) and **TVOC** (Total Volatile Organic Compounds). The sensor module incorporates the **iS-MM-VOCs-4** sensor IC made by [MNST (Micro & Nano Sensor Technology Co., Ltd)](http://mnsenstech.com). Typical applications of A1DU5P2CP005B include:

* Air purifiers and air conditioners
* Smart home and room monitoring devices
* Air quality monitors
* Portable / wearable devices

MSNT iS-MM-VOCs-4 is a very compact **MEMS** sensor (3.8 x 3.8 x 1.5mm) which is capable of detecting formaldehyde, CO2, ethanol, and TVOC. In collaboration with MNST, Sharp has released A1DU5P2CP005B as a specific configuration of the sensor in module packaging which can sense 2 of the 4 substances: formaldehyde and TVOC. Other configurations of the sensor are possible and if you are interested in volume production you can contact [Socle Technology Corp.](http://www.socle-tech.com/) which is Sharp's agent in North America, China, and Taiwan.

You should be able to find the Sharp module at a component distributor under part number A1DU5P2CP005B.

## A1DU5P2CP005B Characteristics

Sharp A1DU5P2CP005B is designed to be used indoors. Follow the module [specifications](http://www.socle-tech.com/doc/IC%20Channel%20Product/SHARP_VOCs_A1DU5P2CP005B.pdf) and avoid exposing the sensor to dust, salt, or gases such as SO2, H2S, Cl2 and NH3.

![A1DU5P2CP005B parameters](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp005b_parameters.png)

When the module is first powered on, it goes through a pre-heating initialization period lasting up to 3 minutes.

## Concentration Data Format and Units

Communication with the A1DU5P2CP005B module can be done over **I2C** or **UART** (serial). In either case, the module transmits a **9-byte data record** with the following format.

![A1DU5P2CP005B datarecord](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp005b_datarecord.png)

The module outputs the HCHO and TVOC values using units of **ppb** (parts per billion). However, air quality concentrations are commonly expressed in units of **ug/m3**. The conversion from ppb to ug/m3 depends on atmospheric pressure, temperature, and the molecular weight of the substance involved. According to this [reference](https://cfpub.epa.gov/ncer_abstracts/index.cfm/fuseaction/display.files/fileID/14285), if we assume a standard pressure of 1 atmosphere and a temperature of 25 degrees Celsius, the formula for converting ppb to ug/m3 units is as follows:

**Concentration (ug/m3) = 0.0409 x concentration (ppb) x molecular weight**

In the case of formaldehyde (HCHO), the molecular weight is 30 g/mol. So no problem with formaldehyde.

However, TVOC is a combination of several different VOC substances such as benzene and toluene. So how to determine what molecular weight to use? I contacted the sensor manufacturer MNST directly and put this question to them. They told me that for TVOC, to use a conversion factor of 1.8:

**1 ppb = 1.8 ug/m3**
 
Working backwards using the first formula, I found that this corresponds to a molecular weight of 44 g/mol which is quite close to the molecular weight of propane (44.1 g/mol). So I am guessing that MNST derived this 1.8 factor based on propane although I am not entirely certain because while propane is a VOC as well, it is not commonly found in typical indoor environments.

## A1DU5P2CP005B Connector

Sharp A1DU5P2CP005B uses a **6-pin connector** made by [Zhejiang Xinfuer Electronics Co., Ltd.](http://www.xinfuer.com/):

* Module-side connector: [1.25T-6AWB](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/docs/Sharp%20A1DU5P2CP005B%20-%20Connector%20Specs.pdf)

![A1DU5P2CP005B connector pins](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp005b_connectorpins.png)

## Using A1DU5P2CP005B with Arduino

### Arduino Circuit - I2C Example

Connect the A1DU5P2CP005B sensor module to your Arduino board as shown in the figure below. This example uses **Arduino Uno** but it should work the same with Arduino **Mega 2560**. The sensor module requires a supply voltage of 3.3V and its SDA and SCL pins operate on 3.3V logic as well. Therefore, it is necessary to use a logic level converter to convert between 3.3V and the 5V that Arduino uses.

![Arduino circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp005b_circuit_i2c.png)

The corresponding [I2C demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_a1du5p2cp005b_demo/sharp_a1du5p2cp005b_demo_i2c.ino) for the above circuit uses the Arduino [Wire library](https://www.arduino.cc/en/reference/wire) to request the formaldehyde and TVOC concentration data from the sensor over I2C, and then display both values using the Arduino Serial Monitor.

The **I2C slave address** for the module is **0x51** and the register for requesting the 9-byte data record is **0x61**.

### Arduino Circuit - UART Example

Connect the A1DU5P2CP005B sensor module to your Arduino board as shown in the figure below. This example uses **Arduino Uno** but it should work the same with Arduino **Mega 2560**.

In the circuit, the hardware serial port is used only for printing output concentration values to the Arduino Serial Monitor. A **software serial port** is used for the actual communication with the A1DU5P2CP005B module (Arduino pins 10 and 11) as shown in the [UART demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_a1du5p2cp005b_demo/sharp_a1du5p2cp005b_demo_uart.ino). If you're using Arduino Mega, you can use one of the other hardware serial ports instead such as **Serial1** for communicating with the A1DU5P2CP005B module.

The sensor module requires a supply voltage of 3.3V and its TX and RX pins operate on 3.3V logic as well. Therefore, it is necessary to use a logic level converter to convert between 3.3V and the 5V that Arduino uses.

![Arduino circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp005b_circuit_uart.png)

The [UART demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_a1du5p2cp005b_demo/sharp_a1du5p2cp005b_demo_uart.ino) shows how to request the concentration data from the sensor by sending the UART command **0x61**. A 9-byte data record is transmitted back by the sensor module which is then decoded into concentration values and printed to the Arduino Serial Monitor.

### Example Serial Monitor Output

If you are switching between different Arduino boards, make sure you go to the Arduino IDE **Tools** menu and set **Board** and **Port** appropriately before compiling or uploading the code. The following screen capture shows example output from the A1DU5P2CP005B module as displayed on the Arduino Serial Monitor.

![Example serial monitor output](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp005b_monitor.png)

To actually test with some VOCs, you can open the cap of a permanent marker and wave it around near the sensor.

## See also
* [Sharp A1DU5P2CP005B Specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/SHARP_VOCs_A1DU5P2CP005B.pdf)
* [VOCs Sensor Module Product Page](http://www.socle-tech.com/SHARP_sensorModule_VoC.php)
* [Understanding Units of Measurement](https://cfpub.epa.gov/ncer_abstracts/index.cfm/fuseaction/display.files/fileID/14285)

