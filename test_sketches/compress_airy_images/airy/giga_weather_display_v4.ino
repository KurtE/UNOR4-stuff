//#define ILI488_DISP

#include <SPI.h>

#ifdef ARDUINO_GIGA
#include <WiFi.h>
#include "Arduino_GigaDisplay_GFX.h"
#else
#if defined(ILI9488_DISP)
#include "ILI9488_t3.h"
#else
#include <ST7796_t3.h> // Hardware-specific library
#endif
 // Common optimized Teensy driver
#include <SD.h>          // Or LittleFS / Flash memory arrays

#include <PNGdec.h>      // PNG decoder library by bitbank2

#include "font_Arial.h"
#include "font_ArialBold.h"

#include <XPT2046_Touchscreen.h>
#define CS_PIN  7
//XPT2046_Touchscreen ts(CS_PIN);
#define TIRQ_PIN  2
XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

// XPT2046 typical raw ADC limits (adjust if edges are slightly off)
#define TS_MINX 200
#define TS_MAXX 3800
#define TS_MINY 200
#define TS_MAXY 3800
/**************************************************
* Setup Teensy Wifi                               *
***************************************************/
#include <QNEthernet.h>
#endif
#include <ArduinoJson.h>
#include "forwardDecs.h"

// Allocated document capacity
JsonDocument doc;

#ifndef ARDUINO_GIGA
using namespace qindesign::network;
#endif

const char *server = "api.open-meteo.com";
const char *geocoding_api_server = "geocoding-api.open-meteo.com";
const char *air_quality_server = "air-quality-api.open-meteo.com";

const int port = 80;
constexpr uint32_t kDHCPTimeout = 15000;

#ifdef ARDUINO_GIGA
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // your network key Index number (needed only for WEP)

ZephyrClient client;
#else
EthernetClient client;
#endif

#include "location.h"
#ifndef DEFAULT_CITY
#define DEFAULT_CITY "Los Angeles"
#endif

String weather_city = DEFAULT_CITY;
String weather_time_zone = "";
double weather_latitude = 0;
double weather_longitude = 0;

/**************************************************
* Define Display pins and sd card Chipselect pin  *
***************************************************/
// Pin assignments (adjust to your hardware setup)
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8
#define SD_CS     BUILTIN_SDCARD     // Adjust to your SD CS pin (if using SD card)

#ifdef ARDUINO_GIGA
GigaDisplay_GFX tft;

#else
#if defined(ILI9488_DISP)
ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST);
#else
ST7796_t3 tft = ST7796_t3(TFT_CS, TFT_DC, TFT_RST);
#endif

/**************************************************
* Initialize PNG Libraray                         *
***************************************************/
PNG png;

// Display Offsets for PNG callback position
int16_t pngX = 0;
int16_t pngY = 0;

// File handler for PNGdec
File pngFile;
#endif

/*************************************************/
#ifdef ARDUINO_GIGA
void Serial_printf(const char *format, ...) {
  char buffer[256];
  va_list ap;
  va_start(ap, format);
  int cb_ret = vsnprintf(buffer, sizeof(buffer), format, ap);
  Serial.write(buffer, cb_ret);
}

void tft_printf(const char *format, ...) {
  char buffer[256];
  va_list ap;
  va_start(ap, format);
  int cb_ret = vsnprintf(buffer, sizeof(buffer), format, ap);
  tft.write(buffer, cb_ret);
}

#else
#define Serial_printf Serial.printf
#define tft_printf tft.printf
#endif


/*************************************************/
void setup() {
#ifdef ARDUINO_GIGA
  int status = WL_IDLE_STATUS;

  tft.begin();
  tft.setRotation(1); // Landscape (480x320)
#else
  tft.init(320, 480);
  //tft.invertDisplay(true);  //black display
  tft.setRotation(1); // Landscape (480x320)
  tft.setOrigin(0,0);
#endif
  tft.fillScreen(COLOR_BG);
  tft.setFont(Arial_10);

  Serial.print("Initializing SD card...");

  #if 0
  if (!SD.begin(SD_CS)) {
    Serial.println("initialization failed!");
    return;
  }
  #endif
  Serial.println("initialization done.");

#ifdef ARDUINO_GIGA
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 3 seconds for connection:
    delay(3000);
  }
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);


#else
  if (!Ethernet.begin()) {
    Serial.println("Failed to configure Ethernet using DHCP");
    return;
  }

  Serial_printf("Waiting for local IP...\r\n");
  if (!Ethernet.waitForLocalIP(kDHCPTimeout)) {
    Serial_printf("Failed to get IP address from DHCP\r\n");
    return;
  }

  Serial.print("Local IP: ");
  Serial.println(Ethernet.localIP());
#endif

  appState = FETCH_MAP_CITY_TO_LOCATION;

  ts.begin(SPI1);
}

void loop() {
  switch (appState) {
    case FETCH_MAP_CITY_TO_LOCATION:
      Serial.println("\n[0/3] Map City to Location...");
      if (sendMapCityRequest()) {
        appState = READ_MAP_CITY_TO_LOCATION;
      } else {
        Serial.println("Connection failed.");
        appState = DONE_APP;
      }
      break;
    case READ_MAP_CITY_TO_LOCATION:
      if (processIncomingStream(printMapCityData)) {
        appState = FETCH_CURRENT;
      }
      break;

    case FETCH_CURRENT:
      Serial.println("\n[1/3] Requesting Current Weather...");
      if (sendCurrentRequest()) {
        appState = READ_CURRENT;
      } else {
        Serial.println("Connection failed.");
        appState = DONE_APP;
      }
      break;

    case READ_CURRENT:
      if (processIncomingStream(printCurrentData)) {
        appState = FETCH_DAILY;  //was FETCH_HOURLY
      }
      break;

    case FETCH_HOURLY:
      Serial.println("\n[2/3] Requesting Hourly Forecast...");
      if (sendHourlyRequest()) {
        appState = READ_HOURLY;
      } else {
        Serial.println("Connection failed.");
        appState = DONE_APP;
      }
      break;

    case READ_HOURLY:
      if (processIncomingStream(printHourlyData)) {
        appState = FETCH_DAILY;
      }
      break;

    case FETCH_DAILY:
      Serial.println("\n[3/3] Requesting Daily Forecast...");
      if (sendDailyRequest()) {
        appState = READ_DAILY;
      } else {
        Serial.println("Connection failed.");
        appState = DONE_APP;
      }
      break;

    case READ_DAILY:
      if (processIncomingStream(printDailyData)) {
        appState = FETCH_AIR_QUALITY;
      }
      break;
    
    case FETCH_AIR_QUALITY:
      Serial.println("\n[4/4] Requesting Air Quality...");
      if (sendAirQualityRequest()) {
        appState = READ_AIR_QUALITY;
      } else {
        Serial.println("Connection failed.");
        appState = DONE_APP;
      }
      break;

    case READ_AIR_QUALITY:
      if (processIncomingStream(printAirQualityData)) {
        drawWeatherDashboard();
        Serial.println("All data successfully fetched!");
        Serial.println("Enter City name:");
        appState = DONE_APP;
      }
      break;
    case DONE_APP:
      break;

    default:
      break;
  }

  if (ts.touched()) {
    TS_Point p = ts.getPoint();

    // Map inverted ADC values to display pixels
    int touchX = map(p.x, 3800, 300, 0, 480);
    int touchY = map(p.y, 3800, 300, 0, 320);

    touchX = constrain(touchX, 0, 480);
    touchY = constrain(touchY, 0, 320);

    // STATE 1: Processing touches on the Main Dashboard
    if (currentScreen == SCREEN_MAIN) {
      int selectedDay = getTouchedForecastCard(touchX, touchY);
      if (selectedDay != -1) {
        Serial_printf("Opening Detail View for Day %d\n", selectedDay);
        showDayDetailScreen(selectedDay);
        delay(300); // Debounce touch
      }
    } 
    // STATE 2: Processing touches on the Detail Screen
    else if (currentScreen == SCREEN_DETAIL) {
      if (isBackButtonClicked(touchX, touchY)) {
        Serial.println("Back Button Pressed! Returning to Main Dashboard...");
        showMainDashboard();
        delay(300); // Debounce touch
      }
    }
  }

  if (Serial.available()) {
    weather_city = Serial.readString();
    weather_city.trim();
    weather_city.replace(' ', '+');
    Serial.print("New City: ");
    Serial.println(weather_city);
    appState = FETCH_MAP_CITY_TO_LOCATION; 
  }
 
}

// Returns card index (0 to 4) if pressed, or -1 if touch is outside cards
int getTouchedForecastCard(int touchX, int touchY) {
  int colWidth = 85;
  int startX = 40;

  int cardY = 185;
  int cardW = colWidth - 8; // 77px wide
  int cardH = 128;         // 128px tall

  // Loop through all 5 cards and check bounding boxes
  for (int i = 0; i < 5; i++) {
    int cardX = (startX + (i * colWidth)) - 5;

    if (touchX >= cardX && touchX <= (cardX + cardW) &&
        touchY >= cardY && touchY <= (cardY + cardH)) {
      return i; // Touched card index
    }
  }

  return -1; // No forecast card touched
}

bool isBackButtonClicked(int touchX, int touchY) {
  // Back button bounds matching: drawRoundRect(20, 260, 100, 40)
  int btnX = 20;
  int btnY = 260;
  int btnW = 100;
  int btnH = 40;

  return (touchX >= btnX && touchX <= (btnX + btnW) &&
          touchY >= btnY && touchY <= (btnY + btnH));
}