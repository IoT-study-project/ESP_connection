# Mongoose OS app 

## Overview

App reads data using ESP32 from L3G4200D: three axis digital output gyroscope and sends via MQTT to AWS ( Amazon Web Server ).

# Already have ESP with installed application?
if yes: 
- connect your ESP to AWS using `mos aws-iot-setup --aws-region YOUR_REGION`
- conncect to WiFi using `mos wifi SSID PASSWORD`
- FINISH🎉 ( check if data have been send to topic 'iot/0/data' )


# How to run on new ESP32?

- first use command `mos build` to build application
- run command `mos flash` to send builded code to your ESP32
- after that go to [Already have ESP with installed application](#already-have-esp-with-installed-application)


