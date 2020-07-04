#define LARGE_JSON_BUFFERS 1
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <SPI.h>
#include <Wire.h>
#include <Thing.h>
#include <WebThingAdapter.h>

#include <ESP8266WiFi.h>

#include "arduino_secrets.h"
//TO DO: please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;     // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (WPA)- seriously, don't use WEP

Adafruit_BME680 bme; // I2C

WebThingAdapter *adapter;

const char *bme680Types[] = {"TemperatureSensor", nullptr};
// Creating the Webthing
// "Exmpbme680": unique name for every connected "Webthing",
// "Example BME680 Weather Sensor": name in Webthings,
// bme680Types: as above
ThingDevice weather("ExmpBME680", "Example BME680 Weather Sensor", bme680Types);
// Set temperature property
ThingProperty weatherTemp("temperature", "", NUMBER, "TemperatureProperty");
// Set humidity as level-property
ThingProperty weatherHum("humidity", "", NUMBER, "LevelProperty");
ThingProperty weatherPres("pressure", "", NUMBER, nullptr);
ThingProperty weatherGas("air-quality", "", NUMBER, nullptr);

void setup() {
  Serial.begin(115200);

  //connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  // unique name for adapter (here: same as device above) and localIP
  adapter = new WebThingAdapter("ExmpBME680", WiFi.localIP());

  // Set unit for temperature 
  weatherTemp.unit = "degree celsius";

  // Set title to "Humidity"
  weatherHum.title = "Humidity";
  // Set unit for humidity (%)
  weatherHum.unit = "percent";
  // Set humidity as read only
  // (otherweise you may change the values in the gateway interface)
  weatherHum.readOnly = "true";


  // Set title to "Pressure"
  weatherPres.title = "Pressure";
  // Set unit for pressure to hPa
  weatherPres.unit = " hPa";
  // Set pressure to read only
  weatherPres.readOnly = "true";

  // Set title to "Env. Air Quality"
  weatherGas.title = "Env. Air Quality";
  // Set unit for pressure to KOhms
  weatherGas.unit = " KOhms";
  // Set environmental gas to read only
  weatherGas.readOnly = "true";

  weather.addProperty(&weatherTemp);
  weather.addProperty(&weatherPres);
  weather.addProperty(&weatherHum);
  weather.addProperty(&weatherGas);

  adapter->addDevice(&weather);
  adapter->begin();

}

void loop() {
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading : (");
    return;
  }


  float temp = bme.temperature;
  float hum = bme.humidity;
  float pres = bme.pressure / 100.0F;
  float gas = bme.gas_resistance / 1000.0F;


  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(pres);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(hum);
  Serial.println(" % ");

  Serial.print("Gas = ");
  Serial.print(gas);
  Serial.println(" KOhms");

  Serial.println();

  ThingPropertyValue value;
  value.number = pres;
  weatherPres.setValue(value);
  value.number = temp;
  weatherTemp.setValue(value);
  value.number = hum;
  weatherHum.setValue(value);
  value.number = gas;
  weatherGas.setValue(value);

  adapter->update();

  //uncomment if energy-saving is important
  //Serial.println("Light Slepp 5 min");
  //wifi_set_sleep_type(LIGHT_SLEEP_T);
  //delay(300000);
  delay(2000);
}
