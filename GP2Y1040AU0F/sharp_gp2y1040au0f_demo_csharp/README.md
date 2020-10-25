# README
## SharpDustSensorDemo

This is a C#/.NET Windows Forms application for controlling Sharp GP2Y1040AU0F Dust Sensor. The Sharp sensor must be connected to a FT232RL serial-to-USB adapter which connects to your Windows PC. The USB adapter must support 3.3V RX/TX and 5V VCC.

The application has various options for logging the UART data to a log file and to the screen.

This is a demo only, not intended for production use.

The bin\Release folder contains an EXE which you can run without having to build the program.

Most of the relevant source code is in Form1.cs.

## Build Notes

Developed using Visual Studio 2019. 

There is a NuGet package (zip file) in the packages folder which needs to be unzipped into the folder.

## Screenshots

![Sharp GP2Y1040Au0F CSharp Demo App Circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1040au0f_csharp_circuit.png)

![Sharp GP2Y1040Au0F CSharp Demo App Screen](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1040au0f_csharp_app.png)

