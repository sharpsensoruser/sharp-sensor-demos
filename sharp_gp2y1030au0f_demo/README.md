# README
Sharp GP2Y1030AU0F Dust Sensor Application Guide

## Overview
<img align="right" src="https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1030au0f_img.png"></img>
Designed for use in home air purifiers, **GP2Y1030AU0F** is a digital **UART** sensor featuring a built-in microcomputer (MCU) with the ability to output particle counts for different ranges of dust sizes. Sharp GP2Y1030AU0F incorporates a built-in LED pulse drive circuit, automatic temperature correction, and a removable panel which allows for some internal cleaning.

_**Note**: It may be difficult to find this Sharp model in stock anywhere because Sharp is currently focusing promotion efforts on the newer GP2Y1040AU0F laser particle sensor model._

## How do Sharp dust sensors work?
Sharp dust sensors operate on the principle of **light scattering**. A photo-detector and LED emitter oppose each other at an angle within the rectangular package of the sensor which has a **dust through hole** on either side. Air containing dust particles flows into the sensor chamber and causes the light from the LED emitter to be scattered towards the photo-detector. The more dust there is inside the sensor chamber, the greater the intensity of the scattered light. 
![dust sensor principles](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1030au0f_dustsensorprinciples.png)

### Particle counting method
Sharp dust sensors generally output a voltage value that varies with the intensity of the scattered light. However, GP2Y1030AU0F works differently in that it uses the **particle counting method**. By analyzing the signal of the scattered light and using an internal algorithm, it is possible to count the number of particles that flow through the sensor. In particular, GP2Y1030AU0F can separately count the number of particles that are less than 2.5 microns in size, and the number of particles that are greater than 2.5 microns in size. It is important to note though that GP2Y1030AU0F does not physically separate the incoming particles into two different streams.

## Key concepts and terminology
![key concepts](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1030au0f_keyconcepts.png)

## Comparing GP2Y1026AU0F and GP2Y1030AU0F 
![compare GP2Y1010AU0F and GP2Y1026AU0F](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1030au0f_comparespec.png)

You can find the detailed specifications for both dust sensor models [here](http://www.socle-tech.com/SHARP_sensor_Dust%20Sensor.php).

## Airflow design requirement
The GP2Y1030AU0F specification indicates that airflow must be provided to the sensor in order for the particle counting to work properly. The sensor will not work if there is only still air. If your application is an air purifier or an air conditioner, you might be able to use the system’s natural airflow for this purpose. Otherwise, you can look at adding a mini 20x20mm or 25x25mm external fan. The GP2Y1030AU0F sensor has a black, plastic side on which the Sharp model number is printed. You can attach a fan to this side of the sensor in order to provide airflow into the **dust through hole** with a speed (at point **b** in the figure below) of 0.5 m/s to 3.0 m/s. Most fans have a CFM (cubic feet per minute) rating but there are some [converters](http://www.comairrotron.com/airflow-unit-conversion) which you may be able to use to convert to units of m/s.

![sensor and fan](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1030au0f_airflow.png)

## Sensor maintenance
GP2Y1030AU0F is the only Sharp dust sensor with a removable panel to allow for internal cleaning. This is a good way to prevent large dust pieces from accumulating inside the sensor permanently and affecting its performance.

![sensor and fan](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1030au0f_panel.png)

Sharp also advises to use a **coarse mesh filter** in your application design to prevent large dust pieces from entering the Sharp sensor in the first place.

## GP2Y1030AU0F connector cable
Sharp GP2Y1030AU0F uses the same type of **6-pin connector** as the popular GP2Y1010AU0F model. Earlier iterations of the GP2Y1010AU0F sensor used the JST (Japan Solderless Terminal) connector **S6B-ZR-SM4A**. But all newer production versions of both sensors now use JCTC connector **11501W90-6P-S-HF** which should be compatible with the old JST connector. What this all means is if you have an old cable harness for GP2Y1010AU0F, the cable should work just fine with GP2Y1030AU0F as well. The following table and figure shows the pin assignments for GP2Y1030AU0F.

![connector pin assignments](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1030au0f_connectorpins.png)

## Using GP2Y1030AU0F with Arduino
### Arduino Circuit
GP2Y1030AU0F includes a built-in **LED pulse drive circuit**. This eliminates the need for an external circuit and allows you to connect the Sharp sensor directly to your **Arduino** board. The pulse drive circuit also helps to extend the lifetime of the sensor under continuous operation. Although the Sharp sensor has a **RxD** terminal, you don’t need to send it any commands in order to begin receiving serial UART data from the sensor’s **TxD** terminal. Simply connect the sensor to your Arduino board as shown in the figure below. This example uses Arduino **Uno** but it should work the same with Arduino **Mega 2560**.

![arduino circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1030au0f_circuit.png)

### UART Specification

In the [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1030au0f_demo/sharp_gp2y1030au0f_demo.ino), you will see that the regular hardware serial port is used only for printing output to the Arduino **Serial Monitor** at 9600 baud. The actual UART data from the dust sensor’s **TxD** terminal is received through a **software serial port** connected to Arduino **Pin 10** running at 2400 baud.

If you are using **Arduino Mega 2560**, which has additional hardware serial ports, you can use **Serial1** instead of the software serial port for communicating with the dust sensor.

The GP2Y1030AU0F dust sensor outputs 1 **data frame** every **10ms** through its TxD terminal. According to the specification, it produces a sequence of 10 data frames as follows:

![data frame](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1030au0f_dataframe.png)

Thus it takes 100ms to output a complete sequence of data frames which is equivalent to **10** sequences per second. However, note that the specification indicates that data is only updated about **once per second**.

The first two data frames are used to identify the start of a sequence. This is followed by data frames containing the raw **particle counts** as follows:

* **PM_A** refers to the count of particles that are less than 2.5 microns in size.
* **PM_B** refers to the count of particles that are greater than 2.5 microns in size.
* **PM_C** refers to the count of all particles regardless of particle size.

### Arduino Sketch (demo source code)

The [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1030au0f_demo/sharp_gp2y1030au0f_demo.ino) shows how to read in a sequence of data frames, verify the **checksum**, and calculate the raw **particle counts**. The code also computes a **moving average** of the last 10 counts in order to produce more stable results. This averaging is recommended by Sharp in their **Application Note** for the GP2Y1030AU0F dust sensor (which is a companion document to the specification). This note also describes the further step of calculating **dust density** (or dust/mass concentration) from the particle counts.

### Dust density formulas
Sharp instructs users to determine their own **formulas** for converting a particle count to dust density by using a **reference dust monitor** (such as a TSI DustTrak II 8530 device), measuring different levels of dust in your own environment, and then plotting the results on a graph as follows:

* Plot particle counts (averaged) from the Sharp GP2Y1030AU0F dust sensor on the X-axis.
* Plot the corresponding dust density (ug/m3) on the Y-axis as measured by the reference dust monitor.

From the graph, derive a formula **y = f(x)** which best fits the plotted points, where **x** is a particle count, and **y** is the corresponding dust density.

![graph to determine formula](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1030au0f_formula.png)

The GP2Y1030AU0F application note lists several examples of formulas for different types of dust particles, different sizes of particles, and various models of reference dust monitors. However, these are just examples and you should conduct your own testing in order to derive formulas suitable for your application and test environment. The [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1030au0f_demo/sharp_gp2y1030au0f_demo.ino) implements some of these formulas, but again, this is just for demonstration purposes and they are not guaranteed to be accurate for your application.

### Example Serial Monitor output
If you are switching between different Arduino boards, make sure you go to the **Tools** menu and set **Board** and **Port** appropriately before compiling or uploading the code. The following screen capture shows example output from the GP2Y1030AU0F dust sensor as displayed on the Arduino Serial Monitor.

![serial monitor output](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1030au0f_monitor.png)

## See also
* [Sharp GP2Y1030AU0F Specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/Sensors/Dust%20Sensor/GP2Y1030AU0F_spec.pdf)
* [Sharp GP2Y1010AU0F Application Note](http://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y1010au_appl_e.pdf)
* [Sharp Dust Sensors Lineup](http://www.socle-tech.com/SHARP_sensor_Dust%20Sensor.php)
* [PM2.5 Monitor with Portable Battery](https://www.renesas.com/us/en/products/microcontrollers-microprocessors/rl78/quick-solution.html) (search for PM2.5)
* [Investigating the Use of Commodity Dust Sensors
for the Embedded Measurement
of Particulate Matter](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.448.6904&rep=rep1&type=pdf)
* [Comparative Experimental Evaluation of Dust Sensors for
Environmental Monitoring on Construction Sites ](https://www.iaarc.org/publications/fulltext/isarc2014_submission_50.pdf)
* [Measurement of PM2.5 Concentrations in Indoor Air Using
Low-Cost Sensors and Arduino Platforms](https://www.ama-science.org/proceedings/getFile/ZwD2BD==)
* [Laboratory Evaluation and Calibration of Three
Low-Cost Particle Sensors for Particulate Matter
Measurement](https://www.tandfonline.com/doi/pdf/10.1080/02786826.2015.1100710)
