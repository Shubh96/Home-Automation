# Home-Automation
## Designing Idea of the Product
The idea is to design a system that can be used to remotely control any electrical appliance at you home, be it fans at lights at your home or be it washing machines, oven or any other appliance.
It gives you complete control of your home in your hand. *But, don't confuse it with home automation.* We haven't automated anything at our home. **Automation** means that every appliance will operate smartly like your assistant without your intervention.

## Hardware Requirements
* [Node MCU Development Board](http://www.nodemcu.com/index_en.html) (Contains Integrated ESP8266 Module)
* [DHT11 Sensor](https://akizukidenshi.com/download/ds/aosong/DHT11.pdf) (Digital Humidity and Temperature Sensor Version 11)

## Software Stack
* Arduino IDE version 1.8.3
* Node-Red version 0.18.4
* MQTT Box

## Understanding the Code
### Libraries Used
```
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h" 
```

``` Adafruit_Sensor.h ``` is used to be able to integrate the DHT11 functions into the code.

``` ESP8266WiFi.h ``` is used to provide ESP8266 specific Wi-Fi routines which we are calling to connect to network.

``` PubSubClient.h ``` is used to Publish and Subscribe data to and from the MQTT broker.

``` DHT.h ``` is used to get the temperature and humidity data from the DHT11 Sensor.

After this is done, there are 3 lines in the code which have some Macro definition there to specify the pins which are being used as output and input pins. After that, the *ssid*, *password* of the network to which the Node MCU is connected and *server URl* of the broker that has to be use, i have used [iot.eclipse.org](https://iot.eclipse.org/).

### Objects and intializations

* ```WiFiClient espClient;```: Here, espClient is an object of the WiFiClient class
