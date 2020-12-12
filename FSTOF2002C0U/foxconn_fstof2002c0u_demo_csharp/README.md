# README
## FoxconnTofModuleDemo

This is a C#/.NET Windows Forms application for controlling Foxconn FSTOF2002C0U / FSTOF2003C0U time-of-flight sensor modules. The TOF Module must be connected to a FT232RL serial-to-USB adapter which connects to your Windows PC. The USB adapter must support 3.3V RX/TX and 3V supply voltage.

The application has various options for logging the UART data to a log file and to the screen.

This is a demo only, not intended for production use.

The bin\Release folder contains an EXE which you can run without having to build the program.

Most of the relevant source code is in Form1.cs.

## Build Notes

Developed using Visual Studio 2019. 

There is a NuGet package (zip file) in the packages folder which needs to be unzipped into the folder.

## Screenshots

![Foxconn Tof Module CSharp Demo App Circuit](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/foxconn_fstof2002c0u_csharp_circuit.png)

![Foxconn Tof Module CSharp Demo App Screen](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/foxconn_fstof2002c0u_csharp_app.png)

