# Mongoose OS app 

## Overview

App reads data from L3G4200D: three axis digital output gyroscope and sends via MQTT to AWS ( Amazon Web Server ).

# Already have ESP with installed application?
if yes: 
- connect your esp to aws using `mos aws-iot-setup --aws-region YOUR_REGION`
- conncect to WIFI using `mos wifi SSID PASSWORD`
- FINISH🎉 ( check if data have been send to topic 'iot/0/data' )


# How to run?

- first use command `mos build` to build application
- run command `mos flash` to send builded code to your ESP32
- after that go to [Link to Header](#already-have-esp-with-installed-application)


