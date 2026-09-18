
//****************************************************/
// Returns the file name corresponding to the WMO Weather Code
// lets see if we can avoid the files...
#ifdef USE_COMPRESSED_IMAGE
#include "airy/compressed_images.h"
const char* getWeatherIconFilename(uint8_t wmo) {
  switch (wmo) {
    case 0:
      return (const char*)&image_clear;                  // Clear sky
    case 1:
      return (const char*)&image_mostly_clear;
    case 2:
      return (const char*)&image_partly_cloudy;
    case 3:
      return (const char*)&image_overcast;          // Mainly clear, partly cloudy, overcast

    case 45:
      return (const char*)&image_fog;
    case 48:
      return (const char*)&image_rime_fog;                    // Fog and depositing rime fog

    case 51:
      return (const char*)&image_light_drizzle;
    case 53:
      return  (const char*)&image_moderate_drizzle;
    case 55:
      return (const char*)&image_dense_drizzle;                // Drizzle: Light, moderate, dense

    case 56:
      return (const char*)&image_light_freezing_drizzle;
    case 57:
      return (const char*)&image_dense_freezing_drizzle;       // Freezing Drizzle

    case 61:
      return (const char*)&image_light_rain;
    case 63:
      return (const char*)&image_moderate_rain;
    case 65:
      return (const char*)&image_heavy_rain;                   // Rain: Slight, moderate, heavy

    case 66:
      return (const char*)&image_light_freezing_rain;
    case 67:
      return (const char*)&image_heavy_freezing_rain;          // Freezing Rain

    case 71:
      return (const char*)&image_slight_snowfall;
    case 73:
      return (const char*)&image_moderate_snowfall;
    case 75:
      return (const char*)&image_heavy_snowfall;                   // Snow fall: Slight, moderate, heavy

    case 77:
      return (const char*)&image_snowflake;            // Snow grains

    case 80:
      return (const char*)&image_light_rain;
    case 81:
      return (const char*)&image_moderate_rain;
    case 82:
      return (const char*)&image_heavy_rain;  
    case 83:
      return (const char*)&image_heavy_rain;           // Rain showers: Slight, moderate, violent

    case 85:
      return (const char*)&image_slight_snowfall;
    case 86:
      return (const char*)&image_slight_snowfall;           // Snow showers: Slight and heavy

    case 95:
      return (const char*)&image_thunderstorm;           // Thunderstorm: Slight or moderate

    case 96:
      return (const char*)&image_thunderstorm_with_hail;
    case 99:
      return (const char*)&image_thunderstorm_with_hail;      // Thunderstorm with hail
    
    case 100:
      return (const char*)&image_keypad;

    default:
      return (const char*)&image_clear;                // Fallback icon
  }
}



#else
#include "airy/clear.h"
#include "airy/dense-drizzle.h"
#include "airy/dense-freezing-drizzle.h"
#include "airy/fog.h"
#include "airy/heavy-freezing-rain.h"
#include "airy/heavy-rain.h"
#include "airy/heavy-snowfall.h"
#include "airy/light-drizzle.h"
#include "airy/light-freezing-drizzle.h"
#include "airy/light-freezing-rain.h"
#include "airy/light-rain.h"
#include "airy/moderate-drizzle.h"
#include "airy/moderate-rain.h"
#include "airy/moderate-snowfall.h"
#include "airy/mostly-clear.h"
#include "airy/overcast.h"
#include "airy/partly-cloudy.h"
#include "airy/rime-fog.h"
#include "airy/slight-snowfall.h"
#include "airy/snowflake.h"
#include "airy/thunderstorm-with-hail.h"
#include "airy/thunderstorm.h"
#include "airy/keypad.h"

const char* getWeatherIconFilename(uint8_t wmo) {
  switch (wmo) {
    case 0:
      return (const char*)image_clear;                  // Clear sky
    case 1:
      return (const char*)image_mostly_clear;
    case 2:
      return (const char*)image_partly_cloudy;
    case 3:
      return (const char*)image_overcast;          // Mainly clear, partly cloudy, overcast

    case 45:
      return (const char*)image_fog;
    case 48:
      return (const char*)image_rime_fog;                    // Fog and depositing rime fog

    case 51:
      return (const char*)image_light_drizzle;
    case 53:
      return  (const char*)image_moderate_drizzle;
    case 55:
      return (const char*)image_dense_drizzle;                // Drizzle: Light, moderate, dense

    case 56:
      return (const char*)image_light_freezing_drizzle;
    case 57:
      return (const char*)image_dense_freezing_drizzle;       // Freezing Drizzle

    case 61:
      return (const char*)image_light_rain;
    case 63:
      return (const char*)image_moderate_rain;
    case 65:
      return (const char*)image_heavy_rain;                   // Rain: Slight, moderate, heavy

    case 66:
      return (const char*)image_light_freezing_rain;
    case 67:
      return (const char*)image_heavy_freezing_rain;          // Freezing Rain

    case 71:
      return (const char*)image_slight_snowfall;
    case 73:
      return (const char*)image_moderate_snowfall;
    case 75:
      return (const char*)image_heavy_snowfall;                   // Snow fall: Slight, moderate, heavy

    case 77:
      return (const char*)image_snowflake;            // Snow grains

    case 80:
      return (const char*)image_light_rain;
    case 81:
      return (const char*)image_moderate_rain;
    case 82:
      return (const char*)image_heavy_rain;  
    case 83:
      return (const char*)image_heavy_rain;           // Rain showers: Slight, moderate, violent

    case 85:
      return (const char*)image_slight_snowfall;
    case 86:
      return (const char*)image_slight_snowfall;           // Snow showers: Slight and heavy

    case 95:
      return (const char*)image_thunderstorm;           // Thunderstorm: Slight or moderate

    case 96:
      return (const char*)image_thunderstorm_with_hail;
    case 99:
      return (const char*)image_thunderstorm_with_hail;      // Thunderstorm with hail
    
    case 100:
      return (const char*)image_keypad;

    default:
      return (const char*)image_clear;                // Fallback icon
  }
}
#endif

// try to remove bold
//#define Arial_12_Bold Arial_12
//#define Arial_11_Bold Arial_11
//#define Arial_10_Bold Arial_10

//#define Arial_12_Bold   &FreeSans10pt7b
//#define Arial_12  &FreeSans10pt7b
//#define Arial_11_Bold   &FreeSans10pt7b
//#define Arial_11  &FreeSans10pt7b
//#define Arial_10_Bold   &FreeSans10pt7b
//#define Arial_10  &FreeSans10pt7b

 
void drawWeatherDashboard() {
  tft.setTextWrap(false);

  // -------------------------------------------------------------
  // 1. HEADER ROW (Location & Date/Time)
  // -------------------------------------------------------------
  tft.setTextColor(COLOR_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.setFont(Arial_12_Bold);
  tft.print(weather.location);

  tft.setCursor(220, 10);
  tft.setTextColor(COLOR_ORANGE);
  tft.print(weather.currentTime);

  drawKeyboard(getWeatherIconFilename(100), 400, 10);

  // -------------------------------------------------------------
  // 2. MAIN CONDITION & CURRENT TEMP (Left Center)
  // -------------------------------------------------------------
  tft.drawRoundRect(25, 30, 450, 150, 12, COLOR_YELLOW);
  tft.setCursor(30, 40);
  tft.setFont(Arial_12_Bold);
  tft.setTextColor(COLOR_CYAN);
  tft.print("Current Conditions:");
  tft.setCursor(35, 60);
  tft.setFont(Arial_12_Bold);
  tft.setTextColor(COLOR_WHITE);
  tft.print(weather.conditionText);

  // Draw current condition PNG icon dynamically
  const char *image = getWeatherIconFilename(weather.currentWmoCode);
  drawPNG(image, 175, 85);

  //Air Quality Index
  //tft.setCursor(45, 150);
  //tft.print("AQI: ");
  //tft.setCursor(90, 150);
  //tft.print(weather.AQI);
  drawAQIMetric(45, 150, weather.AQI);

  // Current Temperature Data
  tft.setFont(Arial_12);
  tft.setTextColor(COLOR_YELLOW);
  tft.setCursor(40, 110);
  tft_printf("%.1f deg F", weather.currentTemp);

  // -------------------------------------------------------------
  // 3. METRICS LIST (Right Side)
  // -------------------------------------------------------------
  int rightX = 290;
  int startY = 35;
  int lineSpacing = 16;

  struct Metric { const char* label; String val; };
  Metric metrics[] = {
    {"Sunrise:",  weather.sunrise},
    {"Sunset:",   weather.sunset},
    {"Humidity:", String(weather.humidity) + "%"},
    {"Wind:",     String(weather.windSpeed, 1) + " mph"},
    {"Wind Dir:", String(weather.windDirection) + " deg"},
    {"Pressure:", String(weather.pressure, 1) + " hPa"},
    {"Rain:", String(weather.rain, 2) + " in"},
    {"Snow:", String(weather.snow, 1) + " in"},
    {"Precip:", String(weather.precip, 2) + " in"}
  };

  for (int i = 0; i < 9; i++) {
    int y = startY + (i * lineSpacing);
    
    tft.setFont(Arial_11);
    tft.setTextColor(COLOR_CYAN);
    tft.setCursor(rightX, y);
    tft.print(metrics[i].label);

    tft.setTextColor(COLOR_WHITE);
    tft.setCursor(rightX + 90, y);
    tft.print(metrics[i].val);
  }

  // Horizontal Divider Line
  //tft.drawFastHLine(10, 175, 460, COLOR_GRAY);

  // -------------------------------------------------------------
  // 4. 5-DAY FORECAST CARDS (Bottom)
  // -------------------------------------------------------------
  int colWidth = 85;
  int startX = 40;

  tft.setFont(Arial_10_Bold);
  for (int i = 0; i < 5; i++) {
    int x = startX + (i * colWidth);

    // Card Container
    tft.drawRoundRect(x - 5, 185, colWidth - 8, 128, 12, COLOR_YELLOW);

    // Weather Icon
    const char* icon = getWeatherIconFilename(weather.wmoCodes[i]);
    drawPNG(icon, x - 2, 216);

    // Date & Day
    tft.setTextColor(COLOR_WHITE);
    tft.setCursor(x, 192);
    tft.print(weather.dates[i]);
    
    tft.setCursor(x, 208);
    tft.print(weather.days[i]);

    // High / Low Temperatures
    tft.setTextColor(COLOR_ORANGE);
    tft.setCursor(x+5, 282);
    tft_printf("%.1fF", weather.tempsHigh[i]);

    tft.setTextColor(COLOR_CYAN);
    tft.setCursor(x+5, 297);
    tft_printf("%.1fF", weather.tempsLow[i]);
  }
}


void showMainDashboard() {
  currentScreen = SCREEN_MAIN;
  tft.fillScreen(COLOR_BG);
  drawWeatherDashboard();
}

void showDayDetailScreen(int dayIndex) {
  currentScreen = SCREEN_DETAIL;
  currentSelectedDay = dayIndex;
  
  tft.fillScreen(COLOR_BG);

  // -------------------------------------------------------------
  // 1. TOP HEADER BAR
  // -------------------------------------------------------------
  tft.setFont(Arial_12_Bold);
  tft.setTextColor(COLOR_YELLOW);
  tft.setCursor(20, 15);
  tft_printf("Forecast: %s (%s)", weather.days[dayIndex].c_str(), weather.dates[dayIndex].c_str());

  // -------------------------------------------------------------
  // 2. MAIN CONDITION & TEMPS (LEFT CARD)
  // -------------------------------------------------------------
  tft.drawRoundRect(15, 45, 215, 195, 10, COLOR_YELLOW);

  // Condition Text
  tft.setFont(Arial_12_Bold);
  tft.setTextColor(COLOR_WHITE);
  tft.setCursor(25, 55);
  tft.print(getWeatherDescription(weather.wmoCodes[dayIndex]));

  // Icon
  const char* icon = getWeatherIconFilename(weather.wmoCodes[dayIndex]);
  drawPNG(icon, 80, 80);

  // High / Low Temps
  tft.setFont(Arial_12_Bold);
  tft.setTextColor(COLOR_ORANGE);
  tft.setCursor(30, 185);
  tft_printf("High: %.1f deg F", weather.tempsHigh[dayIndex]);

  tft.setTextColor(COLOR_CYAN);
  tft.setCursor(30, 210);
  tft_printf("Low:  %.1f deg F", weather.tempsLow[dayIndex]);

  // -------------------------------------------------------------
  // 3. DETAILED METRIC LIST (RIGHT CARD)
  // -------------------------------------------------------------
  tft.drawRoundRect(245, 45, 220, 195, 10, COLOR_YELLOW);

  int startX = 255;
  int startY = 55;
  int spacing = 20;

  struct Metric { const char* label; String val; uint16_t color; };
  Metric metrics[] = {
    {"Sunrise:",   weather.dailySunrise[dayIndex],               COLOR_YELLOW},
    {"Sunset:",    weather.dailySunset[dayIndex],                COLOR_ORANGE},
    {"Humidity:",  String(weather.dailyHumidity[dayIndex]) + "%", COLOR_CYAN},
    {"PoP:",       String(weather.pop[dayIndex]) + "%",                 COLOR_CYAN},
    {"Precip:",    String(weather.precipSum[dayIndex], 2) + " in",      COLOR_WHITE},
    {"Rain:",      String(weather.rainSum[dayIndex], 2) + " in",        COLOR_WHITE},
    {"Snow:",      String(weather.snowSum[dayIndex], 2) + " in",        COLOR_WHITE},
    {"Max Wind:",  String(weather.windMax[dayIndex], 1) + " mph",      COLOR_YELLOW},
    {"Max Gusts:", String(weather.gustsMax[dayIndex], 1) + " mph",     COLOR_ORANGE}
  };

  for (int i = 0; i < 9; i++) {
    int y = startY + (i * spacing);
    
    tft.setFont(Arial_11_Bold);
    tft.setTextColor(COLOR_CYAN);
    tft.setCursor(startX, y);
    tft.print(metrics[i].label);

    tft.setTextColor(metrics[i].color);
    tft.setCursor(startX + 95, y);
    tft.print(metrics[i].val);
  }

  // -------------------------------------------------------------
  // 4. BOTTOM ACTION BAR (BACK BUTTON)
  // -------------------------------------------------------------
  tft.drawRoundRect(20, 255, 120, 45, 8, COLOR_WHITE);
  tft.setFont(Arial_12_Bold);
  tft.setTextColor(COLOR_WHITE);
  tft.setCursor(55, 270);
  tft.print("BACK");
}

#if defined(USE_KEYBOARD)
bool showKeyboard() {
  currentScreen = SCREEN_KEYBOARD;
  Serial.println(weather_city);
  strcpy(MyKeyboard.data, "");
  tft.fillScreen(COLOR_BG);
  // -------------------------------------------------------------
  // 4. BOTTOM ACTION BAR (BACK BUTTON)
  // -------------------------------------------------------------
  tft.drawRoundRect(20, 255, 120, 45, 8, COLOR_WHITE);
  tft.setFont(Arial_12_Bold);
  tft.setTextColor(COLOR_WHITE);
  tft.setCursor(55, 270);
  tft.print("BACK");

  MyKeyboard.getInput();
  Serial.print("New City is: ");
  Serial.println(MyKeyboard.data);

  //check if blank
  String test = MyKeyboard.data;
  if(test.length() < 2) {
    currentScreen = SCREEN_MAIN;
    weather_city = weather_city;
    Serial.println(weather_city);
    appState = FETCH_MAP_CITY_TO_LOCATION; 
    return true;
  }

  //only update on new city
  if(weather_city != MyKeyboard.data) {
    currentScreen = SCREEN_MAIN;
    weather_city = MyKeyboard.data;
    weather_city.trim();
    weather_city.replace(' ', '+');
    //Serial.print("New City: ");
    //Serial.println(weather_city);
    appState = FETCH_MAP_CITY_TO_LOCATION; 
    return true;
  }

  return false;
}
#endif //use keyboard

/***************** AQI ANALYSIS **********************/
// Retrieve category information from AQI score
AQI_Category getAQIInfo(int aqi) {
  aqi = constrain(aqi, 0, 500);
  for (int i = 0; i < 6; i++) {
    if (aqi >= AQI_TABLE[i].aqiMin && aqi <= AQI_TABLE[i].aqiMax) {
      return AQI_TABLE[i];
    }
  }
  return AQI_TABLE[5]; // Default to Hazardous fallback
}

// Linear Interpolation Equation: I = ((I_high - I_low)/(C_high - C_low)) * (C - C_low) + I_low
int calculatePM25AQI(float pm25) {
  if (pm25 < 0.0) return 0;
  if (pm25 > 500.4) return 500;

  for (int i = 0; i < 7; i++) {
    if (pm25 >= PM25_BREAKPOINTS[i].cLow && pm25 <= PM25_BREAKPOINTS[i].cHigh) {
      float cLow  = PM25_BREAKPOINTS[i].cLow;
      float cHigh = PM25_BREAKPOINTS[i].cHigh;
      int iLow    = PM25_BREAKPOINTS[i].iLow;
      int iHigh   = PM25_BREAKPOINTS[i].iHigh;

      return round(((float)(iHigh - iLow) / (cHigh - cLow)) * (pm25 - cLow) + iLow);
    }
  }
  return 500;
}

void drawAQIMetric(int x, int y, int aqiVal) {
  AQI_Category aqi = getAQIInfo(aqiVal);

  // Background badge
  tft.fillRoundRect(x, y, 200, 24, 6, aqi.color);

  // Text label
  tft.setFont(Arial_10_Bold);
  tft.setTextColor(COLOR_BLACK);
  tft.setCursor(x + 8, y + 5);
  tft_printf("AQI: %d", aqiVal);

  // Category status readout next to badge
  tft.setCursor(x + 75, y + 5);
  tft.setTextColor(COLOR_BLACK);
  tft.print(aqi.label);
}