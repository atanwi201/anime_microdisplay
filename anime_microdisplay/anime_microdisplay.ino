#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//sda21 sdc22
// WiFi Configuration
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PSWD";

// URLs
char str[150] ;//this is a variable that will be changed for the link of the "api" //change the longitute and latitude
const char* url_weather = "https://api.open-meteo.com/v1/forecast?latitude=28.4682&longitude=-16.2546&current=temperature_2m,precipitation,is_day&timezone=auto";

HTTPClient http;

String quote = "";
String character = "";
int counter = 0;

void getQuote();

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
  srand(time(NULL));

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connected to:");
  display.println(ssid);
  display.display();
  delay(3000);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);

  if (WiFi.status() == WL_CONNECTED) {
    // Refresh quote every hour (3600 seconds)
    if (counter == 0 || counter >= 3) {
      counter = 0;
      getQuote();
    }

    // Get weather
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
        String time = current["time"].as<String>();
        String temp = current["temperature_2m"].as<String>();
        int isDay = current["is_day"];
        float precip = current["precipitation"];

        // Display everything
        display.setTextSize(1);
        display.print(time + " ");
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

      }
    } else {
      display.println("HTTP error (weather)");
      Serial.println("HTTP error: " + String(httpCode));
    }

    http.end();
  } else {
    display.println("WiFi disconnected");
  }

  display.display();
  counter++;
  Serial.println("Counter: " + String(counter));
  delay(10*1000);  // Update every second
}

void getQuote() {
  int num = rand() % 250 + 1;

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



