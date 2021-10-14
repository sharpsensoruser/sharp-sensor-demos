# README
## Sharp Sensor Demos for Arduino

The purpose of this repository is to help Arduino users get started with using sensor components from Sharp Corporation. You may know Sharp as the maker of consumer electronics products such as flatscreen TVs, calculators, and smartphones. But Sharp also manufactures optoelectronic components and sensors which are widely used by other companies to build their own products such as air purifiers, robotic vacuums, automatic faucets, and much more.

For each sensor, you will find a demo program (sketch) for use with Arduino and a corresponding wiki application guide. The guide gives you some background information on the sensor and shows you how to get started. The Arduino programs are demonstrations which show you the basic steps. They aren't intended to be used for production purposes.

Everything in this repository is based on information which is already publicly available through various datasheets, specifications, and documents published by Sharp or their suppliers and distributors. I'm basically just summarizing things to help electronics hobbyists, researchers, and engineers get started more efficiently. I'll also be focusing more on the newer sensor models from Sharp.

## Sensors

| Part Number | Description | Documentation | Source Code |
|-------------|-------------|-------|------|
|GP2Y0A41SK0F|Distance Measuring Sensor|[Datasheet](http://www.socle-tech.com/doc/IC%20Channel%20Product/Sensors/Distance%20Measuring%20Sensor/Analog%20Output/gp2y0a41sk_e.pdf) | [Demo Source Code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y0a41sk0f_demo/sharp_gp2y0a41sk0f_demo.ino)|
|GP2Y0A51SK0F|Distance Measuring Sensor|[Datasheet](http://www.socle-tech.com/doc/IC%20Channel%20Product/Sensors/Distance%20Measuring%20Sensor/Analog%20Output/gp2y0a51sk_e.pdf) | [Demo Source Code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y0a51sk0f_demo/sharp_gp2y0a51sk0f_demo.ino)|
|GP2Y0AF15X|Distance Measuring Sensor|[Application Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Sharp-GP2Y0AF15X-Distance-Measuring-Sensor) | [Demo Source Code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y0af15x_demo/sharp_gp2y0af15x_demo.ino)|
|GP2Y1014AU0F|Air Quality Sensor (PM, Dust)|[Application Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Sharp-GP2Y1014AU0F-Dust-Sensor)|[Demo Source Code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1014au0f_demo/sharp_gp2y1014au0f_demo.ino)|
|GP2Y1023AU0F|Air Quality Sensor (PM, Dust)|[Specification](http://www.socle-tech.com/doc/IC%20Channel%20Product/SHARP_GP2Y1023AU0F.pdf)|[Demo Source Code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/GP2Y1023AU0F/sharp_gp2y1023au0f_demo.ino)|
|GP2Y1026AU0F|Air Quality Sensor (PM, Dust)|[Application Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Sharp-GP2Y1026AU0F-Dust-Sensor)|[Demo Source Code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1026au0f_demo/sharp_gp2y1026au0f_demo.ino)|
|GP2Y1030AU0F|Air Quality Sensor (PM, Dust)|[Application Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Sharp-GP2Y1030AU0F-Dust-Sensor)|[Demo Source Code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_gp2y1030au0f_demo/sharp_gp2y1030au0f_demo.ino)|
|GP2Y1040AU0F|Air Quality Sensor (PM, Dust)|[Application Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Sharp-GP2Y1040AU0F-Dust-Sensor)|[Arduino](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/GP2Y1040AU0F/sharp_gp2y1040au0f_demo_uart.ino) / [CSharp](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/GP2Y1040AU0F/sharp_gp2y1040au0f_demo_csharp)|

## Modules

| Part Number | Description | Documentation | Source Code |
|-------------|-------------|-------|------|
|A1DU5P2CP005B|VOCs Sensor Module|[Application Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Sharp-A1DU5P2CP005B-VOCs-Sensor-Module)|[UART](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_a1du5p2cp005b_demo/sharp_a1du5p2cp005b_demo_uart.ino) / [I2C](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_a1du5p2cp005b_demo/sharp_a1du5p2cp005b_demo_i2c.ino)|
|A1DU5P2CP006B|Humidity Sensor Module|[Application Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Sharp-A1DU5P2CP006B-Humidity-Sensor-Module)|[Demo Source Code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_a1du5p2cp006b_demo/sharp_a1du5p2cp006b_demo.ino)|
|FSTOF2002C0D|Time-of-Flight Sensor Module|[Application Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Foxconn-FSTOF2002C0D-Time-of-Flight-Sensor-Module)|[UART](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/FSTOF2002C0U/foxconn_fstof2002c0u_demo.ino) / [I2C](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/FSTOF2003C0I/foxconn_fstof2003c0i_demo.ino) / [CSharp](https://github.com/sharpsensoruser/sharp-sensor-demos/tree/master/FSTOF2002C0U/foxconn_fstof2002c0u_demo_csharp)|
|FSTOF2002C0U|Time-of-Flight Sensor Module|[Application Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Foxconn-FSTOF2002C0U-Time-of-Flight-Sensor-Module)|[Arduino](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/FSTOF2002C0U/foxconn_fstof2002c0u_demo.ino) / [CSharp](https://github.com/sharpsensoruser/sharp-sensor-demos/tree/master/FSTOF2002C0U/foxconn_fstof2002c0u_demo_csharp)|
|FSTOF2003C0I|Time-of-Flight Sensor Module|[Application Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Foxconn-FSTOF2003C0I-Time-of-Flight-Sensor-Module)|[Demo Source Code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/FSTOF2003C0I/foxconn_fstof2003c0i_demo.ino)|
|MTOF171000C0|Time-of-Flight Sensor Module|[Application Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/wiki/Application-Guide-for-Sharp-MTOF171000C0-Time-of-Flight-Sensor-Module)|[Demo Source Code](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/sharp_mtof171000c0_demo/sharp_mtof171000c0_demo.ino)|

## See also
* [Sharp Dust Sensor Connectors - JST S6B-ZR-SM4A and JCTC 11501W90-6P-S-HF](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/docs/Sharp%20Dust%20Sensor%20Connectors.md)
* [Distance Sensor Selection Guide](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/docs/Distance%20Sensor%20Selection.pdf)
