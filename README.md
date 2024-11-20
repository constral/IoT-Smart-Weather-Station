# IoT Smart Weather Station



## Overview
This project was developed for the IoT course at my university, and involves a breadboard weather station which uses several sensors to monitor the parameters of the ambient environment, such as temperature, humidity, pressure, and earthquake duration (using a 555 timer). The measured signals are handled by an ESP8266 development board, which is connected over WiFi to a Google Sheets document that receives the values recorded by the sensors. These are then neatly plotted in a spreadsheet table, and are able to be viewed as graphs for better visualization.

## Functionality
The ESP8266 simply reads the output of the DHT11 and BMP280 sensors, with the help of the appropiate Arduino libraries.
<br>For the earthquake system, the 555 timer has an astable configuration, outputting square waves for as long as the pendulum makes contact with the ring around it. The ESP8266 takes these waves as input and counts them. The 555’s output pulse period is 100ms, meaning each rising edge will measure 0.1s worth of shaking.
<br>The data gathered by all sensors within an interval of 5 seconds is bundled into a .json structure format and sent over WiFi to a Google Apps script associated to a spreadsheet.

## Parts used
- DHT11:	Temperature and humidity sensor
- BMP280:	Atmospheric pressure and altitude sensor
- KY-002:	Shock sensor for earthquake detection (for the proof-of-concept model, an equivalent pendulum sensor was used).
- 555 timer (and resistors & capacitors used for biasing in the astable configuration)
- Plusivo ESP8266: The centerpiece handling sensor data collection and uploading it via WiFi.

## Notes on equipment
Although the project was intended to use a KY-002 shock sensor for earthquake detection, the one which I ordered was malfunctioning, so I had to improvise a pendulum that would supplant it.
<br>The programming was done using platformio for python, as the Arduino IDE did not support the ESP8266 Plusivo clone which was used in the project.