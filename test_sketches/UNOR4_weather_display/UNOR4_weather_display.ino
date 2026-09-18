#define USE_COMPRESSED_IMAGE
struct image_info {
  const void *image;
  const uint16_t *palette;
  uint8_t image_top;
  uint8_t image_bottom;
};

//#define ILI488_DISP
//#define XPT_TOUCH  //else FT6236
#define USE_TOUCH
#define USE_KEYBOARD
//#define printForecast
#define orientation 1  // or 1 (landscape)

#include <SPI.h>

#include <ST77XX_renesas.h>  // Hardware-specific library
//#include <Adafruit_GFX.h>
//#include <Adafruit_ST7796S.h>
//#include "customFonts/FreeSans10pt7b.h"
//#include <PNGdec.h>  // PNG decoder library by bitbank2

// Config Touch Pins
#if defined(XPT_TOUCH)
#define CS_PIN 7
#define TOUCH_SPI SPI
#else
#define TOUCH_WIRE Wire
#endif


#include "ST77XX_renesas_font_Arial.h"
#include "ST77XX_renesas_font_ArialBold.h"


#ifdef USE_TOUCH
#if defined(XPT_TOUCH)
#include <XPT2046_Touchscreen.h>
XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - Touch IRQ Pin - interrupt enabled polling
// XPT2046 typical raw ADC limits (adjust if edges are slightly off)
#define TS_MINX 200
#define TS_MAXX 3800
#define TS_MINY 200
#define TS_MAXY 3800
#else
#include <Adafruit_FT6206.h>
Adafruit_FT6206 ts = Adafruit_FT6206();
#endif

#endif


/**************************************************
* Setup Teensy Wifi                               *
***************************************************/
#include <WiFiS3.h>
#define ARDUINOJSON_ENABLE_PROGMEM 0
#include <ArduinoJson.h>
#include "forwardDecs.h"

// Allocated document capacity
JsonDocument doc;

#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
// IPAddress server(93,184,216,34);  // IP address for example.com (no DNS)


WiFiClient client;

const char *server = "api.open-meteo.com";
const char *geocoding_api_server = "geocoding-api.open-meteo.com";
const char *air_quality_server = "air-quality-api.open-meteo.com";

const int port = 80;
constexpr uint32_t kDHCPTimeout = 15000;


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
#define TFT_CS 10
#define TFT_DC 8
#define TFT_RST 9
#define SD_CS BUILTIN_SDCARD  // Adjust to your SD CS pin (if using SD card)

//Adafruit_ST7796S tft(TFT_CS, TFT_DC, TFT_RST);
ST7796_renesas tft(&SPI, TFT_CS, TFT_DC, TFT_RST);


#if defined(USE_KEYBOARD)
#include "ST77XX_renesas_Keypad.h"
// easy way to include fonts but change globally
#define FONT_BUTTON Arial_12  // font for keypad buttons

Keyboard MyKeyboard(&tft, &ts);
#endif  //use keyboard
uint16_t ScreenLeft = 30, ScreenRight = 468, ScreenTop = 302, ScreenBottom = 3;

/**************************************************
* Initialize PNG Libraray                         *
***************************************************/
//PNG png;

/************************************************/

uint32_t g_weather_cycle_time_ms = (uint32_t)(60 * 60 * 1000);  // cycle time in MS 10 * 60 *1000;
uint32_t g_last_cycle_time_ms = 0;

// -------------------------------------------------------------------
// print out WiFi status
// -------------------------------------------------------------------
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void Serial_printf(const char *format, ...) {
  char buffer[256];
  va_list ap;
  va_start(ap, format);
  int cb_ret = vsnprintf(buffer, sizeof(buffer), format, ap);
  Serial.write(buffer, cb_ret);
}

void tft_printf(const char *format, ...) {
  uint8_t buffer[256];
  va_list ap;
  va_start(ap, format);
  int cb_ret = vsnprintf((char *)buffer, sizeof(buffer), format, ap);
  tft.write(buffer, cb_ret);
}


/*************************************************/
void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {}
  tft.begin(320, 480);
  //tft.init(320, 480, 0, 0, ST7796S_RGB);
  tft.invertDisplay(true);       //black display
  tft.setRotation(orientation);  // Landscape (480x320)
                                 //  tft.setOrigin(0,0);
  tft.fillScreen(COLOR_BG);
  //  tft.setFont(Arial_10);

  Serial.println("initialization done.");

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to wifi");

  printWifiStatus();

  appState = (weather_time_zone.length() == 0) ? FETCH_MAP_CITY_TO_LOCATION : FETCH_CURRENT;

#ifdef USE_TOUCH
#if defined(XPT_TOUCH)
  ts.begin(TOUCH_SPI);
  if (orientation == 3) {
    ScreenLeft = 267;
    ScreenRight = 3869;
    ScreenTop = 167;
    ScreenBottom = 3863;
  } else if (orientation == 1) {
    ScreenLeft = 3869;
    ScreenRight = 267;
    ScreenTop = 3863;
    ScreenBottom = 167;
  }
#else
  ts.begin(40, &TOUCH_WIRE);
  if (orientation == 1) {
    ScreenLeft = 3;
    ScreenRight = 478;
    ScreenTop = 302;
    ScreenBottom = 2;
  } else if (orientation == 3) {
    ScreenLeft = 471;
    ScreenRight = 4;
    ScreenTop = 9;
    ScreenBottom = 319;
  }
#endif
#endif

#if defined(USE_KEYBOARD)
  MyKeyboard.init(COLOR_BLACK, COLOR_WHITE, COLOR_BLUE, COLOR_DARKGREY, COLOR_DARKGREY, COLOR_NAVY, COLOR_BLACK, FONT_BUTTON);
  MyKeyboard.setTouchLimits(ScreenLeft, ScreenRight, ScreenTop, ScreenBottom);
  // optional methods
  // max input characters is controlled by in the .h file
  // #define MAX_KEYBOARD_CHARS 18
  // change input display color
  MyKeyboard.setDisplayColor(COLOR_WHITE, COLOR_BLUE);
  // want rounded corners?
  // MyKeyboard.setCornerRadius(3);
  // Set initial instructions
  // MyKeyboard.setInitialText("IP 111.222.333.444");
  // MyKeyboard.hideInput(); // for hidden password input
  // optional to populate the input box
  //  strcpy(MyKeyboard.data, "TEXT");
#endif  //use keyboard
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


#ifdef USE_TOUCH
  int touchX = 0;
  int touchY = 0;
#if defined(XPT_TOUCH)
  if (ts.touched()) {
    TS_Point p = ts.getPoint();

    if (orientation == 1) {
      // Map inverted ADC values to display pixels
      touchX = map(p.x, 3800, 300, 0, 480);
      touchY = map(p.y, 3800, 300, 0, 320);

      touchX = constrain(touchX, 0, 480);
      touchY = constrain(touchY, 0, 320);
    } else {
      TS_Point p = ts.getPoint();

      // Map inverted ADC values to display pixels
      touchX = map(p.x, 0, 3800, 0, 479);
      touchY = map(p.y, 0, 3800, 0, 319);

      touchX = constrain(touchX, 0, 479);
      touchY = constrain(touchY, 0, 319);
    }
#else  //end XPT Touch
  if (ts.touched()) {
    // 1. Get raw reading
    TS_Point p = ts.getPoint();

//    Serial_printf("Touched(%d, %d)", p.x, p.y);

    if (orientation == 1) {
      // 2. Map coordinates for Rotation = 1
      int mappedX = p.y;
      int mappedY = 320 - p.x;

      // 3. Calibrate edges (maps the observed ~0 to ~319 active range to 0-479)
      touchX = map(mappedX, 0, 470, 0, 479);
      touchY = map(mappedY, 0, 310, 0, 319);

      // 4. Constrain to valid display bounds
      touchX = constrain(touchX, 0, 479);
      touchY = constrain(touchY, 0, 319);
    } else {
      // 2. Map coordinates for Rotation = 3
      int mappedX = 480 - p.y;
      int mappedY = p.x;

      // 3. Scale and calibrate active edge boundaries
      touchX = map(mappedX, 10, 480, 0, 479);
      touchY = map(mappedY, 0, 310, 0, 319);

      // 4. Constrain bounds
      touchX = constrain(touchX, 0, 479);
      touchY = constrain(touchY, 0, 319);
    }
//    Serial_printf(" - (%u, %u)\n", touchX, touchY);
#endif
    // STATE 1: Processing touches on the Main Dashboard
    if (currentScreen == SCREEN_MAIN) {
      int selectedDay = getTouchedForecastCard(touchX, touchY);
      if (selectedDay != -1) {
        //Serial.printf("Opening Detail View for Day %d\n", selectedDay);
        showDayDetailScreen(selectedDay);
        delay(300);  // Debounce touch
      }

#if defined(USE_KEYBOARD)
      if (isKeyboardClicked(touchX, touchY)) {
        showKeyboard();
      }
#endif
    }
    // STATE 2: Processing touches on the Detail Screen
    else if (currentScreen == SCREEN_DETAIL) {
      if (isBackButtonClicked(touchX, touchY)) {
        //Serial.println("Back Button Pressed! Returning to Main Dashboard...");
        showMainDashboard();
        delay(300);  // Debounce touch
      }
    }
  }
#endif
#if defined(USE_KEYBOARD)
  else if (currentScreen == SCREEN_KEYBOARD) {
    if (isBackButtonClicked(touchX, touchY)) {
      //Serial.println("Back Button Pressed! Returning to Main Dashboard...");
      showMainDashboard();
      delay(50);  // Debounce touch
    }
  }
#endif  //use keyboard

  if (Serial.available()) {
    weather_city = Serial.readString();
    weather_city.trim();
    weather_city.replace(' ', '+');
    Serial.print("New City: ");
    Serial.println(weather_city);
    appState = FETCH_MAP_CITY_TO_LOCATION;
  }

  // see if we timed out and should start a new read cycle
  uint32_t delta_time = millis() - g_last_cycle_time_ms;
  if (delta_time > g_weather_cycle_time_ms) {
    Serial.println("\n*** Start new read cycle ***");
    appState = (weather_time_zone.length() == 0) ? FETCH_MAP_CITY_TO_LOCATION : FETCH_CURRENT;
    g_last_cycle_time_ms = millis();  // don't keep hitting this
  }
}

// Returns card index (0 to 4) if pressed, or -1 if touch is outside cards
int getTouchedForecastCard(int touchX, int touchY) {
  int colWidth = 85;
  int startX = 40;

  int cardY = 185;
  int cardW = colWidth - 8;  // 77px wide
  int cardH = 128;           // 128px tall

  // Loop through all 5 cards and check bounding boxes
  for (int i = 0; i < 5; i++) {
    int cardX = (startX + (i * colWidth)) - 5;

    if (touchX >= cardX && touchX <= (cardX + cardW) && touchY >= cardY && touchY <= (cardY + cardH)) {
      return i;  // Touched card index
    }
  }

  return -1;  // No forecast card touched
}

bool isBackButtonClicked(int touchX, int touchY) {
  // Back button bounds matching: drawRoundRect(20, 260, 100, 40)
  int btnX = 20;
  int btnY = 260;
  int btnW = 100;
  int btnH = 40;

  return (touchX >= btnX && touchX <= (btnX + btnW) && touchY >= btnY && touchY <= (btnY + btnH));
}


#if defined(USE_KEYBOARD)
bool isKeyboardClicked(int touchX, int touchY) {
  //Serial_printf("%d, %d\n", touchX, touchY);

  int btnX = 400;
  int btnY = 0;
  int btnW = 44;
  int btnH = 14;

  return (touchX >= btnX && touchX <= (btnX + btnW) && touchY >= btnY && touchY <= (btnY + btnH));
}
#endif