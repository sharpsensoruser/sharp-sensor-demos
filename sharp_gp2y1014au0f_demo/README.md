# README
Sharp GP2Y1014AU0F Dust Sensor Application Guide

## Overview
**GP2Y1014AU0F** is one of the newer models in Sharp's lineup of PM (particulate matter) **dust sensors**. Designed for use in home air purifiers, GP2Y1014AU0F is an **analog output** dust sensor which is similar to the popular **GP2Y1010AU0F** model but with improved accuracy. In terms of Arduino circuit and application source code, these two analog dust sensors are more or less interchangeable. You can usually find GP2Y1014AU0F in stock at various components distributors.
<img align="right" src="https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_img.png"></img>

Besides air purifiers, GP2Y1014AU0F can also be found in smart home applications such as room monitors and portable air quality monitors. Renasys has a publicly available [reference design for a battery-powered, PM2.5 air quality monitor based on GP2Y1014AU0F](https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rl78/quick-solution.html) (search for PM2.5). You can download a complete project which includes circuit diagrams, BOM, and sample source code. EME Systems in California also uses GP2Y1014AU0F in a couple of their [air quality projects](https://www.emesystems.com/projects.html).

## How do Sharp dust sensors work?
Sharp dust sensors operate on the principle of **light scattering**. A photo-detector and LED emitter oppose each other at an angle within the rectangular package of the sensor which has a **dust through hole** on either side. Air containing dust particles flows into the sensor chamber and causes the light from the LED emitter to be scattered towards the photo-detector. The more dust there is in the air within the sensor chamber, the greater the intensity of the scattered light. The dust sensor outputs a **voltage** value which varies according to the intensity of the scattered light which in turn corresponds to the level of dust in the air. The actual **dust density** (or dust or mass concentration) can then be calculated from the output voltage value using a linear relation. 

![dust sensor principles](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_dustsensorprinciples.png)

## Key concepts and terminology
![key concepts](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_keyconcepts.png)

## Comparing GP2Y1010AU0F and GP2Y1014AU0F 

![compare GP2Y1010AU0F and GP2Y1014AU0F](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_comparespec.png)

Both dust sensor models measure the ***total dust density***. This total includes the concentrations of 1 micron particles, 2.5 micron particles, 10 micron particles, etc. In practice though, when the total dust density/concentration reads very high, such as in the presence of cigarette smoke, most of the detected concentration is due to **PM2.5** particles anyways (_PM2.5 means particles with size < 2.5um_). In addition, with these analog output models, there is the further possibility to analyze the output signals in order to distinguish between smoke particles and house dust.

You can find the detailed specifications for both dust sensor models [here](http://www.socle-tech.com.tw/SHARP_sensor_Dust%20Sensor.php).

## Airflow design considerations
The GP2Y1014AU0F dust sensor does not include any built-in fan to supply **airflow** to the sensor (and note that the specification does not indicate any airflow is required). This can be advantageous in terms of providing flexibility in sensor orientation and positioning. But still, it is a good idea to think about whether you need to design in at least some airflow into your application. Providing some airflow (from a fan, natural convection, or wind) will allow the sensor to react quicker to changes in dust concentration. For example, if your application is an air purifier or an air conditioner, you might be able to use the system’s natural airflow for this purpose. Other options include adding a mini 20x20mm or 25x25mm external fan but this will introduce additional design considerations such as fan reliability, noise, and cost. To summarize, while airflow is not strictly required, it can help the GP2Y1014AU0F sensor to react quicker to changes in dust concentration. 

Sharp also advises to use a **coarse mesh filter** in your application design to prevent large dust particles from entering the Sharp sensor and accumulating inside. While a light vacuum can be used to remove large dust pieces which may be attached inside the sensor, your application design may preclude such kind of maintenance access to the sensor.

## GP2Y1014AU0F connector cable
Sharp GP2Y1014AU0F uses the same type of **6-pin connector** as the GP2Y1010AU0F model. Earlier iterations of the GP2Y1010AU0F sensor used the JST (Japan Solderless Terminal) connector **S6B-ZR-SM4A**. But all newer production versions of both sensors now use JCTC connector **11501W90-6P-S-HF** which should be compatible with the old JST connector. What this all means is if you have an old cable harness for GP2Y1010AU0F, the cable should work just fine with GP2Y1014AU0F as well. The following table and figure shows the pin assignments for GP2Y1014AU0F.

![connector pin assignments](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_connectorpins.png)

Below is a picture of GP2Y1014AU0F with its connector cable attached.

![connector with cable](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_connectorcable.png)

## Using GP2Y1014AU0F with Arduino
### LED Pulse Drive Circuit
The [specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_GP2Y1014AU0F.pdf) for GP2Y1014AU0F isn't very clear about how to hook up the sensor. A better example can be found in the [Application Note](http://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y1010au_appl_e.pdf) for the GP2Y1010AU0F model. Both GP2Y1010AU0F and GP2Y1014AU0F require the same circuit consisting of a resistor (150 ohms) and a capacitor (220 uF) designed to pulse the sensor **LED** on and off instead of running it continuously. The LED degrades in intensity over time and pulsing the LED helps to extend its lifetime. The figure below from Sharp's application note (section 6-1) shows an example circuit.

![LED pulse drive circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1010au0f_ledpulsecircuit.png)

The application note further indicates (see below figure) that the LED should be pulsed on once every **10ms**, using a pulse duration or width of **0.32ms**. Once the LED is turned on, the application note says to sample the resulting analog output voltage after **0.28ms** (or 280 microseconds) have elapsed from the moment the LED is switched on.

![LED pulse drive timing](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1010au0f_ledpulsetiming.png)

### Arduino Circuit
The following circuit is derived from the [Application Note](http://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y1010au_appl_e.pdf) for the GP2Y1010AU0F model. This example uses Arduino **Uno** but it should work the same with Arduino **Mega 2560**.

![arduino circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_circuit.png)

The key connections to note are:

* The dust sensor **LED** terminal is connected to Arduino digital **pin 7** which will be used to pulse the sensor LED on or off.
* The dust sensor **Vo** terminal is connected to Arduino analog input **pin A5** which will be used to read the output voltage from the sensor.

### Arduino Sketch (demo source code)

The [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1014au0f_demo/sharp_gp2y1014au0f_demo.ino) shows how to:

* Turn the dust sensor LED on.
* Sample the analog output voltage after **0.28 milliseconds** have elapsed. This gives a value from 0 to 1023. Based on the documentation for the Arduino [analogRead](https://www.arduino.cc/reference/en/language/functions/analog-io/analogread/) function, this operation takes around **100 microseconds** or 0.1 milliseconds to complete. This means that the pulse width generated by this demo program is at least 0.38 milliseconds which exceeds the pulse width recommended in the specification of **0.32 ms**.
* Turn the dust sensor LED off.
* Compute an **average voltage** reading based on the last **100 samples**. Using averaging gives more stable results but note that this is just a simple average that is computed every time 100 samples have been read. Since the LED pulse cycle is **10ms**, this means an average voltage reading is generated about once per second.
* Convert the average voltage reading to a value in **Volts**.
* Dynamically adjust the **Voc** value (the output voltage under zero dust conditions). The Voc value is used for **offset correction**.
* Calibrate the voltage reading by applying offset correction and then calculating the **dust density** in units of ug/m3 using the formula below from Sharp.

![Dust density formula from Sharp](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_dustdensity.png)

You can adjust the **Voc** and **K** values in the source code so that your results correlate better against more accurate reference equipment such as a TSI DustTrak II Monitor.

### Example Serial Monitor output
If you are switching between different Arduino boards, make sure you go to the **Tools** menu and set **Board** and **Port** appropriately before compiling or uploading the code. The following screen capture shows example output from the GP2Y1014AU0F dust sensor as displayed on the Arduino Serial Monitor.

![serial monitor output](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_monitor.png)

## Appendix

### Distinguishing between smoke and house dust

The following excerpt from the Application Note for GP2Y1010AU0F (version 2.0) shows how to distinguish between cigarette smoke and house dust particles. This also applies to GP2Y1014AU0F.

![Smoke versus house dust detection](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_housedust.png)

### Sensor Calibration

The GP2Y1014AU0F sensor outputs a voltage reading which varies linearly with PM/dust density. It is necessary to apply a bit of calibration to convert these output voltages to the desired PM/dust density values in units of ug/m3. There are two aspects to this calibration as described below.

#### Offset Correction

Even in a perfectly clean, zero dust environment, the GP2Y1014AU0F sensor will output a non-zero voltage value which is called **Voc**. This behavior is actually useful because you can easily tell whether the sensor is working or not. The GP2Y1014AU0F [specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_GP2Y1014AU0F.pdf) states that the typical value of Voc is **0.6 Volts**. You can see this offset from a graph of Output voltage (V) versus Dust density (mg/m3) for this sensor (see Y-intercept).

![Output voltage versus dust density](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_graph.png)

Offset correction, therefore, involves determining this Voc value, and subtracting it from the uncorrected voltage reading from the sensor. However, determining Voc manually for every dust sensor on a production line may not be practical. The [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1014au0f_demo/sharp_gp2y1014au0f_demo.ino) shows a simple solution to this problem by hardcoding the typical value of 0.6 Volts initially. Then during runtime, the Voc value is updated dynamically whenever a lower output voltage is sensed.

#### Scalar Coefficient

From the Output voltage versus Dust density graph, you can see that the response of the sensor is linear between roughly 0 ug/m3 and 500 ug/m3. By determining the slope of this graph, you get a **scalar coefficient** value which can be used to calculate **dust density**. In other words, the calibration steps are:

1. Apply offset correction to the output voltage reading.
2. Multiply the voltage by the scalar coefficient to get dust density.

The graph from the specification is just an example. Part of the calibration process then is to determine your own Output voltage versus Dust density graph by using a reference dust monitor. For example, choose a reference type of particulate matter to test, such as cigarette smoke, and then record two or more dust density readings reported by a highly accurate dust monitor such as a TSI DustTrak II Monitor. On a graph, plot the offset-corrected voltages from the Sharp sensor on the Y-axis against the dust density readings (in ug/m3) from the reference monitor on the X-axis. Two points are needed to determine the slope of the graph. If you have more than two points, you can use any number of online linear regression tools to calculate the line of best fit. The slope of the graph is the scalar coefficient for converting voltage to dust density.

Sharp's [documentation](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/docs/Sharp%20GP2Y1010AU0F%20-%20Dust%20Density%20Conversion.pdf) for the GP2Y1010AU0F dust sensor (which is very similar to GP2Y1014AU0F) goes a bit further and expresses the scalar coefficient in the form of a **Sensitivity (K)**, in units of Volts/(100 ug/m3). The resulting dust density conversion/calibration formula from Sharp is as follows.

![Dust density formula from Sharp](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_dustdensity.png)

The [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1014au0f_demo/sharp_gp2y1014au0f_demo.ino) uses a typical value for **K** of **0.5** Volts/(100 ug/m3). In practice, K should be adjusted according to the calibration procedure described above.

## See also
* [Sharp GP2Y1014AU0F Specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_GP2Y1014AU0F.pdf)
* [Sharp GP2Y1010AU0F Application Note](http://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y1010au_appl_e.pdf)
* [Sharp GP2Y1010AU0F - Dust Density Conversion](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/docs/Sharp%20GP2Y1010AU0F%20-%20Dust%20Density%20Conversion.pdf)
* [Sharp Dust Sensors Lineup](http://www.socle-tech.com.tw/SHARP_sensor_Dust%20Sensor.php)
* [PM2.5 Monitor with Portable Battery](https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rl78/quick-solution.html) (search for PM2.5)
* [Investigating the Use of Commodity Dust Sensors for the Embedded Measurement of Particulate Matter](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.448.6904&rep=rep1&type=pdf)
* [Comparative Experimental Evaluation of Dust Sensors for Environmental Monitoring on Construction Sites ](https://www.iaarc.org/publications/fulltext/isarc2014_submission_50.pdf)
* [Measurement of PM2.5 Concentrations in Indoor Air Using Low-Cost Sensors and Arduino Platforms](https://www.ama-science.org/proceedings/getFile/ZwD2BD==)
* [Laboratory Evaluation and Calibration of Three Low-Cost Particle Sensors for Particulate Matter
Measurement](https://www.tandfonline.com/doi/pdf/10.1080/02786826.2015.1100710)
