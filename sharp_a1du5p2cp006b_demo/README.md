# README
Sharp A1DU5P2CP006B Humidity Sensor Module Application Guide

## Overview
<img align="right" src="https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp006b_img.png"></img>
**A1DU5P2CP006B** is a new **relative humidity** and **temperature** sensor module from Sharp which incorporates the ENS210 sensor IC made by AMS/ScioSense. Typical applications of A1DU5P2CP006B include:

* Air purifiers and air conditioners
* Smart home devices and thermostats
* Room monitoring devices
* Air quality monitors
* Portable / wearable devices

ENS210 is a very compact **I2C** sensor (2.0 x 2.0 x 0.75mm) which has a recommended application circuit consisting of a capacitor and two resistors. Sharp's module design makes the sensor easier to use by incorporating the necessary circuitry and providing a connector-based interface. The module also includes a rectangular cover which shields the sensor IC from light and dust. With the cover removed, you can see the actual ENS210 sensor as shown in the center of the picture below.

![Module with cover removed](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp006b_cover.png)

You should be able to find the Sharp module at a component distributor under part number A1DU5P2CP006B or ENS210.

## A1DU5P2CP006B Characteristics

Sharp A1DU5P2CP006B is designed to be used indoors. Avoid exposing the module to very low or very high humidity conditions over a long period of time. 

![A1DU5P2CP006B parameters](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp006b_parameters.png)

## A1DU5P2CP006B Connector

Sharp A1DU5P2CP006B uses a **4-pin connector** made by [JWT (Jowle Technology Co. Ltd)](http://www.jwt.com.tw/index.asp?web_type=english):

* Module-side connector: [JWT A2001WR2-4P-JT](http://www.jwt.com.tw/pro_pdf/A2001.pdf)

![A1DU5P2CP006B connector pins](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp006b_connectorpins.png)

## Using A1DU5P2CP006B with Arduino

### Arduino Circuit

Connect the A1DU5P2CP006B sensor module to your Arduino board as shown in the figure below. This example uses **Arduino Uno** but it should work the same with Arduino **Mega 2560**. The sensor module requires a supply voltage of 3.3V and its SDA and SCL pins operate on 3.3V logic as well. Therefore, it is necessary to use a logic level converter to convert between 3.3V and the 5V that Arduino uses.

![Arduino circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp006b_circuit.png)

The corresponding [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_a1du5p2cp006b_demo/sharp_a1du5p2cp006b_demo.ino) for the above circuit uses the Arduino [Wire library](https://www.arduino.cc/en/reference/wire) to request the relative humidity and temperature values from the ENS210 sensor over I2C, and then display both values using the Arduino Serial Monitor.

The demo code uses the **Single Shot** mode of ENS210 which takes a single measurement and then returns the sensor to a low-power, standby state. The sensor actually supports a wide range of functions and I2C registers. See the ENS210 datasheet for more details and sample code.

### Example Serial Monitor Output

If you are switching between different Arduino boards, make sure you go to the Arduino IDE **Tools** menu and set **Board** and **Port** appropriately before compiling or uploading the code. The following picture shows example output from the A1DU5P2CP006B sensor module as displayed on the Arduino Serial Monitor. 

![Example serial monitor output](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_a1du5p2cp006b_monitor.png)

For testing purposes, I used a third-party humidity and temperature monitor to validate my Arduino output. You can see from the picture above that the Arduino output has good correlation with the monitor's displayed values. The third-party monitor shown in the picture costs less than US$20 on Amazon and has the following specs:

* Miaomiaoce Thermo-hygrometer
* Miaoce Medical Equipment(Tianjin) Co., Ltd.
* Product model: MHO-C202
* Sensor IC brand: Sensirion

## See also
* [Sharp A1DU5P2CP006B Specification](http://www.socle-tech.com.tw/SHARP_sensorModule_Humidity&Temperature.php)
* [ENS210 Datasheet](https://www.sciosense.com/wp-content/uploads/documents/SC-000897-DS-7-ENS210-Datasheet.pdf)
