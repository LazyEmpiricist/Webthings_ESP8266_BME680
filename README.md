# Webthings_ESP8266_BME680
Adding an ESP8266 (Adafruit Huzzah Feather) with BME680 environmental sensor to a local Webthings smart home gateway

Hardware: 
Adafruit Huzzah https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/overview
Adafruit BME680 https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas
(should also work with generic ESP8266 and BME680)

ESP8266s and BME680s offer an easy and cheap way to add environmental sensing capabilities to the Mozilla-IoT Webthings gateway.

Sensor values are updated in near real-time. If not needed, you can add a light-sleep at the end of the loop(). 

This code re-uses some example-lines from Adafruit Industries (https://www.adafruit.com/) 

Issues, comments or pull-request are highly appreciated. 
