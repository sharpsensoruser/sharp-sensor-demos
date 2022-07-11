# Using the Sharp Microwave Sensor Demo Kit

## Overview

Sharp's **Microwave Sensor Demo Kit** is a very good way to get familiar with the capabilities of the [Microwave Sensor (MWS) Module DC6S4ZN3101](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Sharp-DC6S4ZN3101-Microwave-Motion-Sensor-Module). You can use it for demonstration purposes or as a development tool to help you when you are coding directly against the sensor's UART specification.

![MWS module](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_img.png)

## Demo Kit contents

The Sharp MWS Module is available as a demo kit consisting of:

1. Sharp DC6S4ZN3101 Microwave Sensor Module
2. UART-to-USB converter board which connects the MWS Module to a Windows PC
3. Demo Software (consisting of a PC Waveform Viewer program)

Also required is a **Mini USB Type B cable** for connecting the USB board to your Windows PC. This cable is _not included_ in the demo kit, and your Windows PC must be running **Windows 7 or higher** (Windows XP will not work). If you have a Mac, look into dual-boot options or try using virtual machine software such as _Parallels Desktop_.

![Sharp MWS Demo Kit contents](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_demokita.png)

## Setting up the MWS Demo Kit

Once you've unpacked the demo kit and verified the contents, follow these setup steps:

1. Connect the USB board to your PC using the Mini USB cable. Make sure you are using a fully functional cable and not a _Charge-Only_ cable.
2. Open the **Control Panel** in Windows and go to the **Device Manager**. 
3. Expand **Ports** and you should see a _USB Serial Port (COM7)_ added (or similar).

![Demo Kit setup](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_demokitb.png)

If you don't see the COM port, try installing the USB FTDI drivers: http://www.ftdichip.com/Drivers/VCP.htm

## Starting the MWS Viewer Program

1. On your Windows PC, go to the **Sharp MWS Demo Software** folder.
2. Run the application **MicroWave_sample.exe**.

![Starting the MWS Viewer program](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_demokitc.png)

3. Once the **MWS Viewer** program is running, click the **Setting** button in the top-right corner.

![Click the Setting button](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_demokitd.png)

4. In the Setting dialog, select the last COM port. Then click the **X** to close the dialog.

![Select the COM port](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_demokite.png)

5. Click the **Start** button in the top-right corner of the MWS Viewer. You should start seeing microwave signal waveforms displayed in the graph area. There are two waveforms corresponding to the sensor's **I and Q channels**.

![Click the Start button](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_demokitf.png)

## Using the MWS Viewer Program

* The MWS Module DC6S4ZN3101 produces UART output as well as optional digital Hi/Low output.
* It is of course possible to connect directly to the MWS Module and communicate with it via UART (i.e., instead of using the USB board and a PC). 
* However, the USB board and the MWS Viewer program are provided for demonstration purposes and also to serve as development tools.
* You can use the MWS Viewer program to monitor the microwave signal waveforms, alarms, alarm thresholds, and even issue UART commands.

## Monitoring I and Q Channel Waveforms

* The MWS Module outputs waveform signals using two channels: **I** and **Q**.
* Using the MWS Viewer program, you can monitor the I and Q waveforms as well as communicate with the sensor by using UART commands.
* Based on the principle of the **Doppler Effect**, the I and Q waves will be out of phase depending on whether a person is approaching or moving away from the sensor. So the phase shift can be used to determine direction of motion.
* In addition, depending on whether a person is moving closer or further away, the amplitude of the I and Q signals will change. When the mean amplitude exceeds a threshold long enough, an alarm is triggered. The MWS Module has built-in support for thresholds, alarms, and alarm ON/OFF timers in UART mode.

## Thresholds and Alarms

* The MWS Viewer program displays the I and Q channel waveforms in the graph area.
* It also displays 4 alarms (only 1 alarm is audible) corresponding to 4 thresholds.
* Each threshold corresponds to a mean IQ signal amplitude which in turn corresponds to human motion and distance from the sensor.

![Thresholds and Alarms](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_demokitg.png)

The following table shows the default alarm and threshold settings.

![Default alarm and threshold settings](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_dc6s4zn3101_demokith.png)

## Two Modes

The MWS Module actually has two modes of operation.  The standard mode is UART.

### UART (TX, RX) Mode
* 4 kinds of alarms are triggered when corresponding thresholds are exceeded.
* Each alarm is associated with an ON timer and an OFF timer which determines when the alarm should be raised or turned off. For example, by default, an alarm will be set when the IQ signal mean exceeds the alarm's threshold for a duration of 0.1 seconds (or 100 ms). Likewise, when the IQ signal mean drops below the alarm's threshold for at least 0.1 seconds, the alarm will be turned off. These ON/OFF timer durations are configurable per alarm via UART commands.

### Comparator Mode
* An analog voltage signal (0 to 3.3V) is applied to the **COMP_TH** input terminal.
* This is compared against the IQ signal average.
* The result is output to the **COMP_OUT** terminal as a digital Hi/Low signal.

## See also

* [Application Guide for Sharp DC6S4ZN3101 Microwave Motion Sensor Module](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_dc6s4zn3101_demo/README.md)
* [Sharp Microwave Sensor Demo Application (C#/.NET Windows Forms)](https://github.com/sharpsensoruser/sharp-sensor-demos/tree/master/sharp_dc6s4zn3101_demo_csharp)
* [Sharp Microwave Sensor Module Lineup](http://www.socle-tech.com.tw/SHARP_sensorModule_NoncontactBodyMotionSensorModule.php)
* [Sharp DC6S4ZN3101 Preliminary Datasheet](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_DC6S4XN31XX.pdf)
* [Sharp DC6S4ZN3101 Specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_DC6S4ZN3101.pdf)
* [Sharp DC6S4ZN3102 Specification](http://www.socle-tech.com.tw/doc/IC%20Channel%20Product/SHARP_DC6S4ZN3102.pdf)

