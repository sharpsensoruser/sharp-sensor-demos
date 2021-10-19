# Sharp Dust Sensor Connectors

## JST Connector

Sharp dust sensor models which have been in production for a few years, such as **GP2Y1010AU0F** and **GP2Y1014AU0F**, originally used the 6-pin **S6B-ZR-SM4A** connector (1.50 mm pitch ZH series) from [JST (Japan Solderless Terminal Co. Ltd)](http://www.jst.com/index.html), as evidenced by the below excerpt from the original GP2Y1010AU0F specification.

![GP2Y1010AU0F old connector spec](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1010au0f_connector.png)

However, since the beginning of 2018, Sharp has been effecting a running change whereby most production models of Sharp dust sensors have switched to using **JCTC connectors**.

## JCTC Connector

Almost all Sharp dust sensor models currently in production now use the 6-pin **11501W90-6P-S-HF** connector made by [JCTC (Shenglan Technology Co. Ltd)](http://www.jctc.com.cn/). If you happen to have an old JST cable harness (mating connector/plug with cable) for a Sharp dust sensor, it should still work with the new JCTC connectors because JCTC has guaranteed compatibility with JST. The following Sharp dust sensors use the JCTC connector:

* GP2Y1010AU0F - _Popular analog output model_
* GP2Y1014AU0F - _Higher accuracy version of GP2Y1010AU0F_
* GP2Y1026AU0F - _Digital UART model with built-in MCU_
* GP2Y1027AU0F - _Higher accuracy version of GP2Y1026AU0F_
* GP2Y1030AU0F - _Digital UART particle counter model_

Below is an excerpt from the revised GP2Y1014AU0F specification (September 28, 2017) showing the new JCTC connector.

![GP2Y1014AU0F new connector spec](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/sharp_gp2y1014au0f_connector.png)

## JCTC Mating Connector

The JCTC mating connector is part number **11501H00-6P** for the connector housing, and part number **11501TOP-2E** for the connector terminal.

![JCTC mating connector](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/jctc_mating_connector.png)

Below is the specification for the JCTC **connector housing** but note that the diagram shows the 8-pin model.

![JCTC connector housing](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/jctc_connector_housing.png)

Below is the specification for the JCTC **connector terminal**.

![JCTC connector terminal](https://github.com/sharpsensoruser/sharp-sensor-demos/blob/master/images/jctc_connector_terminal.png)

## Where to buy JCTC Connectors?

**Hayakawa Electronics America**
* **Address**: 10 Industrial Park Drive, Oxford, Mississippi 38655, USA
* **Tel**: +1-662-234-1410
* **URL**: http://www.hayakawaus.com/ 

Or, you can use JST connector instead. Just search for JST ZH 1.5mm pitch 6-pin connector on ebay, you should be able to find sets of connectors with wires already attached.

## See also

* [Sharp Dust Sensors Lineup](http://www.socle-tech.com/SHARP_sensor_Dust%20Sensor.php)
