# README
Sharp DC6S4ZN3101 Microwave Motion Sensor Module Application Guide

## Overview
**DC6S4ZN3101** is a new Microwave Sensor (**MWS**) Module from Sharp Corporation. This sensor module transmits and receives microwaves using antennas and digitally outputs a motion signal of a person by using the **Doppler effect**.

![MWS module](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_img.png)

Due to region-specific RF certification requirements, there are actually two different models of the Microwave Sensor Module. The two models have similar performance, identical UART interface, and the same connector. However, there are small differences in circuitry.

| Part Number | Region |
|-------------|-------------|
|DC6S4ZN3101|US / Canada|
|DC6S4ZN3102|EU / China / Japan / Korea|

### Features

Sharp's MWS Module is a compact, next-generation model with the following features:

* Operating frequency of 24GHz-ISM-Band
* Supply voltage is 3.3V
* Detects speed and direction of moving objects or persons
* Wide angle detection area (Field of View)
* Body motion and walking speed up to 5km/h can be detected
* Resists the effects of ambient light and temperature
* Sensor module can be completely enclosed within a resin casing
* Built-in RF circuit, analog signal processing, and microcomputer (MCU)
* Digital UART interface

### Applications
* General motion sensing for home and building security
* Human presence or absence detection
* Automatic door sensors
* User detection for ATMs and kiosks
* Automatic control of lighting
* Washroom facilities automation
* Detecting hand or arm movement

## How does the Sharp Microwave Sensor work?

Sharp's microwave sensor operates using the principles of the **Doppler effect**. The sensor module has up to 4 transmitting and 4 receiving antenna elements for sending out microwave signals and detecting the reflected signals. The frequency, phase shift, and amplitude of the returned signals can be used to determine the speed and direction of motion.

Sharp uses the following example of sending out radio waves to determine the speed of an approaching baseball. As the ball approaches the sensor, the reflected radio waves have a higher frequency than the original transmitted signal. This effect is very much similar to the high pitch of an ambulance siren that you hear as the vehicle approaches you. 

![Speed of baseball example](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_speedball.png)

Although microwave sensors are a relatively new technology, microwaves are actually just a type of **radio frequency (RF)** wave with a very high frequency. Sharp Corporation has a lot of experience with RF technology and has been developing RF products such as TV tuners and LNB devices for many years.

## Compare PIR Sensors with Microwave Sensors

Passive Infra-Red (**PIR**) or pyroelectric sensors are commonly used for motion detection. Instead of directly replacing PIR sensors, you would want to use microwave sensors in applications and scenarios that can take advantage of their strengths, such as good resistance to environmental effects and the ability to be completely hidden within a resin case without the need for a cover or hole.

Microwave sensors are also more sensitive to human motion. They can detect minute body motion or movements at distances of a few meters, leading to the possibility of unique applications beyond security monitoring.

![Compare PIR with MWS](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_compare.png)

## Sharp Microwave Sensor Demo Kit

The Sharp MWS Module is available as a demo kit consisting of:

1. Sharp DC6S4ZN3101 Microwave Sensor Module
2. UART-to-USB converter board which connects the MWS Module to a Windows PC
3. Demo Software (consisting of a PC Waveform Viewer program)

The demo kit is a good way to get familiar with the capabilities of the sensor module. You can use it for demonstration purposes or as a development tool to help you when you are coding directly against the UART specification.

![MWS Viewer program](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_demokitf.png)

See [Using the Sharp Microwave Sensor Demo Kit](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Using-the-Sharp-Microwave-Sensor-Demo-Kit) for more details.

## MWS Module Specifications

### Module Outline

The figure below shows the outline dimensions of the Sharp MWS Module DC6S4ZN3101.

![MWS module outline](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_outline.png)

The module board has two holes for application mounting. However, the back of the module has wiring so if you are installing onto a metal surface, it is necessary to use a resin spacer to prevent contact with the metal surface. The spacer is not required if you are installing onto a resin or ceramic surface. Resin screws are also recommended for application mounting instead of metal screws to avoid interference with the microwave signals. 

### Module Connector

The MWS Module uses a **6-pin connector** made by [JCTC (Shenglan Technology Co. Ltd)](http://www.jctc.com.cn/). The [preliminary datasheet](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_DC6S4XN31XX.pdf) for the module indicates it uses the following JCTC connector part numbers:

* Module-side connector: JCTC 11002W90-6P-S-5A-HF
* Mating connector housing: JCTC 11002H00-6P-HF

However, the latest [specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_DC6S4ZN3101.pdf) for DC6S4ZN3101 indicates the module uses slightly different JCTC part numbers:

* Module-side connector: JCTC 11002W90-6P-S-R
* Mating connector housing: JCTC 11002H00-6P-2-HF

The figure and table below shows the pin assignments for the MWS Module.

![MWS module connector pins](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_connectorpins.png)

Note that JCTC connectors may be difficult to source in North America. As an alternative, try the [JST SH series 1.0mm pitch 6-pin connector](http://www.jst-mfg.com/product/pdf/eng/eSH.pdf) which should be compatible with JCTC. JST connectors are easy to find on ebay and you can purchase a set of 10 JST cable assemblies for just a few dollars.

### Field of View

The MWS Module has a wide detection area of **140 degrees** in the horizontal plane and **70 degrees** in the vertical/elevation plane. This makes it ideal for applications such as human presence detection.

## UART Specification

### TX Data Format

The MWS Module will begin transmitting **data packets** through its **TX terminal** without any need to send it UART commands (except in the case of I and Q channel waveform values). Each data packet encodes one of the following types of information:
* I and Q channel waveform values (must be enabled via UART command)
* Mean IQ signal value
* Debug output
* Alarm states

Each data packet has a variable-length **Payload**, **Sequence number**, and **Checksum**. 

![UART data packet format](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_uart_datapacket.png)

The **Payload** portion of the data packet further consists of a **Type**, **Length**, and variable-length **Value**. The Value field contains the actual data of importance.

The **Sequence** number in the data packet can be used to check the consistency of Waveform Data packets. Sequence numbers cycle through the values from 0 to 127 for waveform data and should be equal to zero for all other payload types.

The **Checksum** field in a data packet is calculated by XOR'ing all of the bytes in the _Value_ field, and then XOR'ing the result with 0xff. See the [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_dc6s4zn3101_demo/sharp_dc6s4zn3101_demo.ino) for the checksum calculation details.

Four different types of payloads are supported:

#### Type 1 - Channel I and Q Waveform Data

The Channel I and Q Waveform Data is the same information that is displayed in the graph area of the MWS Viewer program from the [Demo Kit](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Using-the-Sharp-Microwave-Sensor-Demo-Kit).

For this type of payload, the _Length_ field is set to **4**, which indicates that the _Value_ is 4 bytes long. The first 2 bytes of the Value field correspond to Channel I, and the last 2 bytes are for Channel Q. Each pair of bytes is encoded as a 16-bit signed short. This means that the Channel I and Q values may be positive or negative.

![UART payload Type 1 format](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_uart_payloadtype1.png)

By default, the MWS Module does not transmit this waveform data and you must enable it with a UART command. Through UART, you can set the frequency of the waveform data to be 100Hz or 500Hz.

#### Type 5 - Signal Mean Value

The **Signal Mean Value** represents the mean IQ signal value over a certain period of time. It is computed using absolute values as follows:

1. Calculate the mean of the Channel I signal over a duration of 0.1 seconds.
2. Calculate the mean of the Channel Q signal over a duration of 0.1 seconds.
3. Calculate the Signal Mean Value as the average of the two mean values.

Format-wise, the _Length_ is *2* and so the _Value_ field will be 2 bytes long. The two bytes are encoded as a 16-bit signed short.

![UART payload Type 5 format](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_uart_payloadtype5.png)

#### Type 7 - Debug Information

**Debug Information** is simply a debug string of characters with _Length_ from 1 to 32 bytes. The string is not _Null_ terminated.

This payload type is used to report error or status messages such as "OK" after a UART command is sent to the MWS Module. It is also used to display the module firmware version information when the UART command **"ver"** is issued.

![UART payload Type 7 format](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_uart_payloadtype7.png)

#### Type 11 - Alarms

The MWS Module is centered around the concept of **threshold detection alarms**. There are four of these alarms which are triggered when the signal mean value exceeds or drops below a certain threshold value for a specific duration of time such as 0.1 seconds. The threshold values and ON/OFF timer durations are configurable per alarm via UART commands.

For **Alarm** payloads, the _Length_ is *2* and so the _Value_ field is 2 bytes long. The first byte encodes the alarm states for **Alarm0** and **Alarm1**. The second byte encodes the alarm states for **Alarm2** and **Alarm3**.

The first 4 bits of the first byte encode the state of Alarm0. This value will be either 0 or 1. The last 4 bits of the first byte encode the state of Alarm1.

Similarly, the first 4 bits of the second byte encode the state of Alarm2. The last 4 bits of the second byte encode the state of Alarm3.

![UART payload Type 11 format](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_uart_payloadtype11.png)

### RX Commands

Sending UART commands to the MWS Module is a lot easier than decoding the TX data formats. The commands are simply text values and do not require you to encode sequence numbers or checksums. The table below describes the supported UART commands.

**NOTE**: After sending a command, you must terminate it properly by writing the value **0x0d** to the serial port.

![UART Commands](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_uart_commands.png)

## Using the MWS Module with Arduino
### Arduino Circuit
You can connect the MWS Module directly to your Arduino board as follows. The circuit figure below shows how to connect to **Arduino Mega 2560**. In this example, the regular hardware serial port is used to display output to the Arduino Serial Monitor. The hardware **Serial1** port (pins RX1 and TX1) is used for the actual communication with the MWS Module.

Alternatively, you can communicate with the MWS Module using a **software serial port** instead of Serial1. The [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_dc6s4zn3101_demo/sharp_dc6s4zn3101_demo.ino) is actually set up to use a software serial port by default (which you can easily change via the provided compilation switch). In this case, connect Arduino **pins 10 and 11** to the MWS Module's TX and RX terminals, respectively. By using a software serial port, the demo source code can work with **Arduino Uno** as well.

Note that the TX and RX pins of the MWS Module operate on 3.3V logic, so it is necessary to use a logic level converter to convert between 3.3V and the 5V that Arduino uses.

According to the module [specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_DC6S4ZN3101.pdf), unused pins should be connected to GND via a 10 kΩ resistor (not shown in figure).

![Arduino Circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_circuit.png)

![Arduino Circuit photo](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_circuitphoto.png)

### Example Serial Monitor output

If you are switching between different Arduino boards, make sure you go to the Arduino IDE **Tools** menu and set **Board** and **Port** appropriately before compiling or uploading the code. The following screen capture shows example output from the MWS Module as displayed on the _Arduino Serial Monitor_.

![Serial Monitor output](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_monitor.png)

### Demo Issues

The [demo source code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_dc6s4zn3101_demo/sharp_dc6s4zn3101_demo.ino) has been coded to Sharp's 2018/06/20 UART specification and MWS Module firmware version 0.27. This firmware version seems to have the issues below:

* Avoid using the "wave 500" mode which sends IQ waveform packets at a frequency of 500 Hz. The Arduino demo cannot keep up and the serial port buffer will overflow.

## See also

* [Using the Sharp Microwave Sensor Demo Kit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/docs/Using%20the%20Sharp%20Microwave%20Sensor%20Demo%20Kit.md)
* [Sharp Microwave Sensor Demo Application (C#/.NET Windows Forms)](https://github.com/sharpsensoruser/sharp-sensor-demos/tree/master/sharp_dc6s4zn3101_demo_csharp)
* [Sharp Microwave Sensor Module Lineup](http://www.socle-tech.com.tw/SHARP_sensorModule_NoncontactBodyMotionSensorModule.php)
* [Sharp DC6S4ZN3101 Preliminary Datasheet](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_DC6S4XN31XX.pdf)
* [Sharp DC6S4ZN3101 Specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_DC6S4ZN3101.pdf)
* [Sharp DC6S4ZN3102 Specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_DC6S4ZN3102.pdf)
