#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define LEDPin BUILTIN_LED                                                //Using D4 of the NodeMCU development board as output pin for LED
#define DHTTYPE DHT11                                                     //Using the DHT 11 sensor to sense Temperature and Humidity

const char* ssid = "EduCare";                                             //This is the username of the network to which NodeMCU connects
const char* password = "eDuCaRe.2019@SuBhAm";                             //This is the password of the network to which NodeMCU connects
const char* mqtt_server = "broker.mqtt-dashboard.com";                    //This is the broker for the mqtt_server

WiFiClient espClient;                                                     //This initializes the WiFiClient object named espClient
PubSubClient client(espClient);                                           //This initializes the PubSubClient object named as client

const int DHTPin = 5;                                                     // DHT Sensor - GPIO 5 = D1 on ESP-12E NodeMCU board

DHT dht(DHTPin, DHTTYPE);                                                 //Initializing the DHT Sensor with DHTPin and Sensor used

// Timers auxiliary variables
long now = millis();
long lastMeasure = 0;

void setup_wifi()                                                         //This functions connects the NodeMCU to the router
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);                                             //Connecting to a WiFi network with the given username and password
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("WiFi connected to network ");
  Serial.println(ssid);
  Serial.print("with IP Address: ");
  Serial.println(WiFi.localIP());                                         //WiFi.localIP() gives tge address of the IP to which NodeMCU is connected
}

void callback(char* topic, byte* payload, unsigned int length)           //Callback executes when NodeMCU recieves some message from a topic to which it is subscribed
{
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  String messageTemp = "";
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  Serial.print("Subham's Room LED Bulb status: ");
  if(messageTemp == "ON"){
    digitalWrite(LEDPin, LOW);
    Serial.print("On");
  }
  else if(messageTemp == "OFF"){
    digitalWrite(LEDPin, HIGH);
    Serial.print("Off");
  }
      
  Serial.println();
}

void reconnect() 
{
  while (!client.connected()) {

    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      client.subscribe("SubhamRoom/LEDBulb");
    } else {
      Serial.print("Failed, rc= ");
      Serial.print(client.state());
      Serial.println("Reconnecting..");
    }
  }
}

void setup() 
{
  pinMode(LEDPin, OUTPUT);
  
  dht.begin();
  Serial.begin(115200);
  
  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  now = millis();

  if (now - lastMeasure > 5000)                                         //Taking sensor readings every 5 seconds
  {
    lastMeasure = now;
    float humidity = dht.readHumidity();
    
    float temperatureCelsius = dht.readTemperature();                   // Read temperature as Celsius (the default)
    float temperatureFahrenheit = dht.readTemperature(true);            // Read temperature as Fahrenheit (isFahrenheit = true)

    if (isnan(humidity) || isnan(temperatureCelsius) || isnan(temperatureFahrenheit)) 
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    float hic = dht.computeHeatIndex(temperatureCelsius, humidity, false);    // Computes temperature values in Celsius
    static char temperatureTemp[7];
    dtostrf(hic, 6, 2, temperatureTemp);
    
    static char humidityTemp[7];
    dtostrf(humidity, 6, 2, humidityTemp);

    client.publish("SubhamRoom/Temperature", temperatureTemp);                      // Publishes Temperature values
    client.publish("SubhamRoom/Humidity", humidityTemp);                            // Publishes Humidity values
    
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Temperature: ");
    Serial.print(temperatureCelsius);
    Serial.print(" *C \t(");
    Serial.print(temperatureFahrenheit);
    Serial.println(" *F )");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.println(" *C ");
    Serial.println("================================================================================================================================");
  }
} 
