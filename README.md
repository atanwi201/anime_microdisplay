# 🍥 anime tiny display
 *A display for you to use as a clock, weather station or just as inspiration for your next project .*

## 🌟 Highlights

- extra motivation while working
- rainy day reminder
- good time keeper


## ℹ️ Overview

In this project, I successfully connected an ESP32 to the internet and retrieved various types of data such as time, weather and quotes from Web APIs like the openweather API and my own "not API" for anime quotes, it also uses a button to change the quote if you find it boring  


### ✍️ Authors

I’m Aryan Tanwani. I created this project to learn about C++ and enhance my knowledge for my career as a software developer (although this is a hardware project). My goal is to inspire my generation to create remarkable things and to build many cool things myself. I’m a developer who’s ready to tackle real challenges.

## 🚀 Usage

*this is a plug-and-play system, so don't worry about using it wrong😉.*
this will give you a new quote every time you look at it being able to inspire you to do great things.

## ⬇️ Installation
>### components:
>-Esp32-s3 Dev Board
>-0.96inch TFT screen


### 1.- ➰ Use this pinout structure
for the button
pin 18 and 3V (one each side)

for the OLED 0,96 inch screen 
pin D21 to SDA
pin D22 to SDC
3V to VCC 
GND to GND
##
### 2.- 📁 Download the files and open arduino IDE

### 3.- ⬇ Install the necesary libraries for configuring the ESP32

Adafruit SSD1306
- Controls the OLED display
Adafruit GFX Library
- Required by SSD1306 (handles text, shapes, etc.)
ArduinoJson
- Used to parse API responses
WiFi (built-in with ESP32 core)
- Connects ESP32 to the internet
HTTPClient (built-in)
- Makes API requests
Wire (built-in)

- Handles SDA/SCL communication with the display

### 4.- 🛜 Change the weather API link and the wifi credentials
The APIs used are:
Weather API:
Open-Meteo API
Quote API:
My custom endpoint (the-system-201.neocities.org)

### 5.- ⬆ Upload the code to the board

## 📦 Requirements

### Software
- Arduino IDE
- ESP32 board support ("esp32 by Espressif Systems")

### Libraries
- Adafruit SSD1306
- Adafruit GFX
- ArduinoJson

### Hardware
- ESP32
- SSD1306 I2C OLED display (128x64)
- push button

### Other
- WiFi & internet connection


## 💭 Feedback and Contributing

if you have any more doubts don't hesitate in leaving it in the [Discussion](https://github.com/atanwi201/anime_microdisplay/discussions).

And if you want to contribute in the project you can for example translate this readme in your native language.
