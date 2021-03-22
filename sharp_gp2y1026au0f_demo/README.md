# README
Sharp GP2Y1026AU0F Dust Sensor Application Guide

## Overview
<img align="right" src="https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1026au0f_img.png"></img>
Designed for use in home air purifiers, Sharp Corporation dust sensors have proven popular over the years with electronics hobbyists and researchers interested in building air quality and pollution monitors. In particular, Sharp’s **GP2Y1010AU0F** dust sensor model is the subject of many electronics projects due to its low cost and ease of availability through components distributors. Since the introduction of GP2Y1010AU0F, Sharp has released newer models such as **GP2Y1026AU0F** and **GP2Y1030AU0F** which offer improvements in design and function. Sharp GP2Y1026AU0F is a digital UART sensor featuring a built-in LED pulse drive circuit and **microcomputer** (MCU) which handles some of the processing you would normally need to provide yourself such as offset calibration, temperature correction, and averaging of data values to minimize noise.

## How do Sharp dust sensors work?
Sharp dust sensors operate on the principle of **light scattering**. A photo-detector and LED emitter oppose each other at an angle within the rectangular package of the sensor which has a **dust through hole** on either side. Air containing dust particles flows into the sensor chamber and causes the light from the LED emitter to be scattered towards the photo-detector. The more dust there is in the air within the sensor chamber, the greater the intensity of the scattered light. The dust sensor outputs a **voltage** value which varies according to the intensity of the scattered light which in turn corresponds to the level of dust in the air. The actual **dust density** (or dust or mass concentration) can then be calculated from the output voltage value using a linear relation.
![dust sensor principles](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1026au0f_dustsensorprinciples.png)

## Key concepts and terminology
![key concepts](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1026au0f_keyconcepts.png)

## Comparing GP2Y1010AU0F and GP2Y1026AU0F 
![compare GP2Y1010AU0F and GP2Y1026AU0F](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1026au0f_comparespec.png)

Both dust sensor models measure the ***total dust density***. This total includes the concentrations of 1 micron particles, 2.5 micron particles, 10 micron particles, etc. In practice though, when the total dust density or concentration reads very high, such as in the presence of cigarette smoke, most of the detected concentration is due to **PM2.5** particles anyways (_PM2.5 means particles with size less than 2.5um_).

You can find the detailed specifications for both dust sensor models [here](http://www.socle-tech.com/SHARP_sensor_Dust%20Sensor.php).

## Airflow design considerations
The GP2Y1026AU0F dust sensor does not include any built-in fan to supply **airflow** to the sensor (and note that the specification does not indicate any airflow is required). This can be advantageous in terms of providing flexibility in sensor orientation and positioning. But still, it is a good idea to think about whether you need to design in at least some airflow into your application. Providing some airflow (from a fan, natural convection, or wind) will allow the sensor to react quicker to changes in dust concentration. For example, if your application is an air purifier or an air conditioner, you might be able to use the system’s natural airflow for this purpose. Other options include adding a mini 20x20mm or 25x25mm external fan but this will introduce additional design considerations such as fan reliability, noise, and cost. To summarize, while airflow is not strictly required, it can help the GP2Y1026AU0F sensor to react quicker to changes in dust concentration.

Sharp also advises to use a **coarse mesh filter** in your application design to prevent large dust particles from entering the Sharp sensor and accumulating inside. While a light vacuum can be used to remove large dust pieces which may be attached inside the sensor, your application design may preclude such kind of maintenance access to the sensor.

## GP2Y1026AU0F connector cable
Sharp GP2Y1026AU0F uses the same type of **6-pin connector** as the GP2Y1010AU0F model. Earlier iterations of the GP2Y1010AU0F sensor used the JST (Japan Solderless Terminal) connector **S6B-ZR-SM4A**. But all newer production versions of both sensors now use JCTC connector **11501W90-6P-S-HF** which should be compatible with the old JST connector. What this all means is if you have an old cable harness for GP2Y1010AU0F, the cable should work just fine with GP2Y1026AU0F as well. The following table and figure shows the pin assignments for GP2Y1026AU0F.

![connector pin assignments](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1026au0f_connectorpins.png)

## Using GP2Y1026AU0F with Arduino
### Arduino Circuit
GP2Y1026AU0F includes a built-in **LED pulse drive circuit**. This eliminates the need for an external circuit and allows you to connect the Sharp sensor directly to your **Arduino** board. The pulse drive circuit also helps to extend the lifetime of the sensor under continuous operation. Although the Sharp sensor has a **RxD** terminal, you don’t need to send it any commands in order to begin receiving serial UART data from the sensor’s **TxD** terminal. Simply connect the sensor to your Arduino board as shown in the figure below. This example uses Arduino **Uno** but it should work the same with Arduino **Mega 2560**.

![arduino circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1026au0f_circuit.png)

### UART Specification

In the [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1026au0f_demo/sharp_gp2y1026au0f_demo.ino), you will see that the regular hardware serial port is used only for printing output to the Arduino **Serial Monitor** at 9600 baud. The actual UART data from the dust sensor’s **TxD** terminal is received through a **software serial port** connected to Arduino **Pin 10** running at 2400 baud.

Instead of using a software serial port, you can use the regular hardware serial port (Arduino RX0 pin instead of Pin 10) but make sure the sensor TxD terminal is disconnected during uploading to Arduino or else the upload will fail. Another option is to use Serial1 on Arduino Mega 2560 for communicating with the dust sensor.

The GP2Y1026AU0F dust sensor outputs 1 byte every **10ms** through its TxD terminal. According to the specification, it produces a 7-byte data frame as follows:

![data frame](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1026au0f_dataframe.png)

Thus it takes 70ms to output a data frame which is equivalent to **14** frames per second. However, the dust sensor MCU applies **averaging** internally to the data readings in order to reduce noise and so the output voltage values only change about **once per second**. This means that you will see a lot of consecutive frames with an identical set of bytes (and voltage).

Note that the **checksum** is computed by summing the four voltage components and then taking the lower 8 bits of the sum.

### Arduino Sketch (demo source code)

The [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1026au0f_demo/sharp_gp2y1026au0f_demo.ino) shows how to read in a data frame, verify the checksum, and calculate the **output voltage**. A linear calibration **coefficient "a"** is then applied which converts the output voltage into a **dust density** value. The "a" coefficient is set by default to approximate the following graph from the GP2Y1026AU0F [specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/Sensors/Dust%20Sensor/GP2Y1026AU0F%20SPECIFICATION.pdf).

![output voltage vs dust density graph](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1026au0f_graph.png)

For practical applications, you should determine a suitable value for the coefficient yourself by measuring different dust density levels and comparing the results against higher accuracy reference equipment such as a TSI DustTrak II Monitor. For example, on a graph, plot output voltage from the sensor on the Y axis versus dust density on the X axis which you measure simultaneously using a reference dust monitor. Then calculate the coefficient based on the slope of the graph. The coefficient will be different depending on the reference dust monitor used and the type of particulate matter you are testing with.

### Example Serial Monitor output
If you are switching between different Arduino boards, make sure you go to the **Tools** menu and set **Board** and **Port** appropriately before compiling or uploading the code. The following screen capture shows example output from the GP2Y1026AU0F dust sensor as displayed on the Arduino Serial Monitor.

![serial monitor output](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1026au0f_monitor.png)

## See also
* [Sharp GP2Y1026AU0F Specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/Sensors/Dust%20Sensor/GP2Y1026AU0F%20SPECIFICATION.pdf)
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
