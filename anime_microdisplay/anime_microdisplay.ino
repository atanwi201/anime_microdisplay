#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//SDA21 SCL22
// WiFi Configuration
const int quotes_len = 550;
const int btn_pin = 18;
const char* ssid = "Your_wifi";
const char* password = "Your_password";
int btn_state;
// URLs
char str[150] ;//this is a variable that will be changed for the link of the "api"
//remember to change the Weather API place
const char* url_weather = "https://api.open-meteo.com/v1/forecast?latitude=28.4682&longitude=-16.2546&current=temperature_2m,precipitation,is_day&timezone=auto";


int counter = 0;
String quote = "";
String character = "";
String currentTime;
String temp;
int isDay;
float precip;
//component functions
void getQuote();
void getWeather();

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting to WiFi...");
  display.display();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nCONNECTED to SSID: " + String(ssid));
  // set randomness
  randomSeed(esp_random());
  Serial.print(time(NULL));
  pinMode(btn_pin, INPUT);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connected to:");
  display.println(ssid);
  display.display();
  delay(3000);
}

void loop() {
  btn_state = digitalRead(btn_pin);

  if(btn_state == 1)
  {
    Serial.print("btn pressed");
    getQuote();
    delay(100);
  }else{
    Serial.print(btn_state);
  }
  display.clearDisplay();
  display.setCursor(0, 0);

  if (WiFi.status() == WL_CONNECTED) {
    // Refresh quote every 30s
    if (counter == 0 || counter >= 30) {
      counter = 0;
      getQuote();
    }
    if (counter == 0 || counter >= 10) {
      getWeather();
    }

        // Display everything
        display.setTextSize(1);
        display.print(currentTime + " ");
        display.println(isDay ? "Day" : "Night");  // Show Day or Night
        display.print(temp);
        display.print("C - ");
              if (precip > 0.0) {
          display.println("Raining...");
        } else {
          display.println("No rain");
        }
        display.println(quote);
        display.println("- " + character);
  } else {
    display.println("WiFi disconnected");
  }

  display.display();
  counter++;
  Serial.println("Counter: " + String(counter));
  delay(1*1000);  // Update every 1' second
}

void getQuote() {
  HTTPClient http;
  int num = random(1, quotes_len + 1);

  snprintf(str, sizeof(str),
           "https://the-system-201.neocities.org/home/quotes/json_full_chunk_%d.json",
           num);

  Serial.println(str);

  http.begin(str);
  int httpCode = http.GET();

  if (httpCode == 200) {

    String payload = http.getString();
    Serial.println(payload);

    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("JSON error: ");
      Serial.println(error.c_str());
      quote = "JSON error";
      character = "";
      return;
    }

    // adjust this depending on real JSON:
    quote = doc[0]["quote"].as<String>();
    character = doc[0]["source"].as<String>();

  } else {
    Serial.print("HTTP error: ");
    Serial.println(httpCode);
    quote = "HTTP failed";
    character = "";
  }

  http.end();
}

void getWeather()
{
  HTTPClient http;
  http.begin(url_weather);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();

      StaticJsonDocument<1024> doc;  // Smaller size is enough for weather
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print("JSON deserialize error (weather): ");
        Serial.println(error.c_str());
        display.println("Weather JSON error");
      } else {
        JsonObject current = doc["current"];
        Serial.print(current);
        currentTime = current["time"].as<String>();
        temp = current["temperature_2m"].as<String>();//because it's a floating number
        isDay = current["is_day"];
        precip = current["precipitation"];
        }
    } else {
      display.println("HTTP error (weather)");
      Serial.println("HTTP error: " + String(httpCode));
    }

    http.end();
}




