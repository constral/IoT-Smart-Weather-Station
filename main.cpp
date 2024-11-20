#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <DHT.h>



const char* ssid = "ssid";
const char* password = "password";
const char* googleScriptUrl = "https://script.google.com/macros/s/AKfycbzjrHmGCgQNVtz9EM3E1Sn6eFkBuJqmGSipxdVIIXLU-g317XEHdfM3d7TaD4Krv4LT/exec";

WiFiClientSecure client;

		

// DHT11 settings
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// BMP280 settings (I2C)
Adafruit_BMP280 bmp;

// 555 timer output
#define PULSE_PIN D5



// Pulse counter variable (volatile since it's used in an interrupt)
volatile unsigned long pulseCount = 0;

// Increment pulse count whenever interrupt is triggered
void IRAM_ATTR handlePulse() {
	pulseCount++;
}





void sendToGoogleSheets(String jsonData) {

	HTTPClient http;

	// Set client to ignore SSL certificate validation (for "testing" purposes)
	client.setInsecure();

	http.begin(client, googleScriptUrl);

	http.addHeader("Content-Type", "application/json");

	int httpResponseCode = http.POST(jsonData);

	if (httpResponseCode > 0) {
		String response = http.getString();
		Serial.println("Data sent successfully: " + response);
	} else {
		Serial.println("Error sending data: " + String(httpResponseCode));
	}

	http.end();
}




void setup() {

	Serial.begin(115200);

// wifi connection

	WiFi.begin(ssid, password);
	Serial.print("connecting to wifi");

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("connected to wifi");



// initialize sensors

	// dht:
	dht.begin();

	// bmp:
	if (!bmp.begin(0x76)) {
		Serial.println("BMP280 initialization failed");
		while (1);
	}

	// clock from earthquake pendulum
	pinMode(PULSE_PIN, INPUT);
	
	// attach interrupt to the pulse pin (trigger on RISING edge)
	attachInterrupt(digitalPinToInterrupt(PULSE_PIN), handlePulse, RISING);
}



void loop() {

	// holder for json message
	String jsonData;

	// sensor readings
	float temperature = dht.readTemperature();
	float humidity = dht.readHumidity();
	float pressure = bmp.readPressure() / 100.0F;	// hPa
	float altitude = bmp.readAltitude(1013.25);	// Altitude in meters


	// Check if readings are valid
	if (!isnan(temperature) && !isnan(humidity) && !isnan(pressure))
	{
		// Capture pulse count (disable interrupts briefly to avoid conflict)
		noInterrupts();
		float countedSeconds = pulseCount * 0.1;
		pulseCount = 0;	// Reset the pulse counter after capturing the value
		interrupts();

		// Prepare JSON object for the request
		jsonData = "{\"temperature\":";
		jsonData += String(temperature);
		jsonData += ", \"humidity\":";
		jsonData += String(humidity);
		jsonData += ", \"pressure\":";
		jsonData += String(pressure);
		jsonData += ", \"altitude\":";
		jsonData += String(altitude);
		jsonData += ", \"pulseCount\":";
		jsonData += String(countedSeconds);
		jsonData += "}";

		Serial.println("DATA: " + jsonData);
	}
	else
	{
		Serial.println("failed to read sensors");
	}


	// if connected to wifi, send data to sheets
	if (WiFi.status() == WL_CONNECTED)
	{
		sendToGoogleSheets(jsonData);
	}
	else
	{
		Serial.println("wifi not connected");
	}

	delay(5000);	// send data every 5 seconds
}
