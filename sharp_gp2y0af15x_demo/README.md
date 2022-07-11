# README
Sharp GP2Y0AF15X Distance Measuring Sensor Application Guide

## Overview
**GP2Y0AF15X** is the newest addition to Sharp's lineup of compact, analog output Distance Measuring Sensors (**DMS**) which is rated for short distances from **1.5cm to 15cm**. Typical applications include obstacle/step detection for robot vacuum cleaners, automatic soap dispensers, coffee machines, and consumer/toy robotics.

![Sharp GP2Y0AF15X image](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y0af15x_img.png)

There are actually several models in the GP2Y0AF15 series, all of which are essentially the same sensor but with different mounting conditions and connector orientations. 

![Sharp GP2Y0AF15X series comparison](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y0af15x_series_compare.png)

GP2Y0AF15 series sensors are an alternative to the popular GP2Y0A51SK0F model (2cm to 15cm) from Sharp. Depending on your application, the volume pricing for GP2Y0AF15 series may be low enough to make it also a viable alternative to I2C proximity sensors which usually require much more effort for the optical design and to calibrate and performance tune properly.

## How do Sharp distance measuring sensors work?

Sharp's **DMS** (Distance Measuring Sensor) models such as GP2Y0AF15X use **PSD** (Position Sensitive Detector) technology to determine the distance to an object. This type of sensor emits an **IR** (Infra-Red) beam of light which reflects off a target object and is received back at the sensor's PSD. The reflected light is detected at a different location on the PSD depending on the distance to the target. These locations on the PSD correspond to different current and output voltage values which can then be used to compute the distance.

![Sharp DMS principles of operation](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y0af15x_principles.png)

## GP2Y0AF15X connector cable

Sharp GP2Y0AF15X uses a **3-pin connector** made by [JCTC (Shenglan Technology Co. Ltd)](http://www.jctc.com.cn/):

* Sensor-side connector: JCTC 11501W90-3P-1
* Mating connector housing: JCTC 11501H00-3P

![GP2Y0AF15X connector pins](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y0af15x_connectorpins.png)

**Note:** _Previous models of GP2Y0AF15 series used JST B3B-ZR connectors. So if you have old JST connector cables, they may still work with newer production models of these sensors which now use JCTC connectors. This is because JCTC usually guarantees compatibility with JST for these types of connector changes._
 
## Using GP2Y0AF15X with Arduino

### Arduino Circuit

Connect the GP2Y0AF15X sensor to your Arduino board as shown in the figure below. This example uses **Arduino Uno** but it should work the same with Arduino **Mega 2560**.

**Note:** I haven't done it in my demo, but Sharp recommends that a by-pass capacitor of 10μF or more be connected between Vcc and GND near this sensor to stabilize the power supply line. See the GP2Y0AF15X [specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_GP2Y0AF15X.pdf) Section 7-8.

![Arduino circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y0af15x_circuit.png)

The corresponding [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y0af15x_demo/sharp_gp2y0af15x_demo.ino) for the above circuit shows how to read the output voltage values from the GP2Y0AF15X sensor, convert the voltage to a distance in millimeters, and then display both values using the Arduino Serial Monitor.

![Arduino circuit photo](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y0af15x_circuitphoto.png)

### Sensor timing chart

When power is applied to the GP2Y0AF15X sensor, it is necessary to wait until the first measurement is completed before starting to read the output voltage from the sensor. Below is the timing chart from the GP2Y0AF15X [specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_GP2Y0AF15X.pdf) which shows that it takes at least 16.5ms + 3.7ms + 5.0ms = **25.2ms** before the output becomes stable.

![Timing chart](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y0af15x_timing.png)

### Calculating distance from output voltage

Since the GP2Y0AF15X sensor only outputs a voltage value, it is necessary to perform some calculations/calibration in order to determine the actual distance (in millimeters or mm). The GP2Y0AF15X [specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_GP2Y0AF15X.pdf) provides an example graph which can be used to derive an equation for calculating distance from voltage. The graph plots **Output voltage (V)** on the Y-axis versus **Distance (mm)** on the X-axis.

![Distance graph](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y0af15x_graph.png)

The GP2Y0AF15X sensor is only rated for distances between **1.5cm and 15cm**. Therefore, we can ignore the portions of the graph beyond those distances. The remaining graph looks like a reciprocal function of the form: **y = a/x + b**, where 'a' and 'b' are fixed coefficients that we want to determine, 'x' is the distance in mm, and 'y' is the output voltage in Volts. From the graph, I picked out two points I can use to determine the coefficients: (30mm, 1.84V) and (150mm, 0.39V). This gives me two linear equations in two variables. Using an online calculator, I determined that **a = 54.375** and **b = 0.0275**. These are the values I used in the [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y0af15x_demo/sharp_gp2y0af15x_demo.ino).

The graph from Sharp is just an example based on using a white paper target with a 90% reflectance ratio. For your own application, you'll probably want to take your own measurements and customize the distance calibration as needed. In the demo, I used an equation approach but you could also just store a bunch of points in memory and use interpolation instead to derive the distance.

### Example Serial Monitor Output

If you are switching between different Arduino boards, make sure you go to the Arduino IDE **Tools** menu and set **Board** and **Port** appropriately before compiling or uploading the code. The following screen capture shows example output from the GP2Y0AF15X sensor as displayed on the Arduino Serial Monitor.

![Example serial monitor output](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y0af15x_monitor.png)

## See also
* Sharp GP2Y0AF15 specifications: [GP2Y0AF15Q](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_GP2Y0AF15Q.pdf) | [GP2Y0AF15R](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_GP2Y0AF15R.pdf) | [GP2Y0AF15X](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_GP2Y0AF15X.pdf) | [GP2Y0AF15Y](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_GP2Y0AF15Y.pdf)
* [Distance Sensor Selection Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/docs/Distance%20Sensor%20Selection.pdf)
* [Sharp DMS Product Lineup](http://www.socle-tech.com.tw/SHARP_sensor_Distance%20Measuring%20Sensor.php)
* [Sharp-World DMS Product Lineup](http://www.sharp-world.com/products/device/lineup/selection/opto/haca/diagram.html)
