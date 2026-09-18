struct WeatherData {
  // Header / Current
  String location = "Loading...";
  String currentTime = "--/--/---- --:--";
  String conditionText = "--";
  int currentWmoCode = 0;
  float currentTemp = 0.0;
  
  // Right side metrics (Current Day)
  String sunrise = "--:--";
  String sunset = "--:--";
  int humidity = 0;
  float windSpeed = 0.0;
  int windDirection = 0;
  float pressure = 0.0;
  float rain = 0.0;
  float snow = 0.0;
  float precip = 0.0;
  int AQI = 0;
  float PM25 = 0.0;
  float PM10 = 0.0;

  // Multi-Day Forecast Storage
  String days[7];
  String dates[7];
  float tempsHigh[7];
  float tempsLow[7];
  uint8_t wmoCodes[7];
  int pop[7];           
  float precipSum[7];   
  float rainSum[7];     
  float snowSum[7];     
  float windMax[7];     
  float gustsMax[7];    
  float cloudCover[7];

  // Expanded Daily Array Fields
  String dailySunrise[7];
  String dailySunset[7];
  int dailyHumidity[7];
} weather;

// Header detection state machine across loop iterations
uint8_t headerState = 0;
bool headerComplete = false;

// Helper to construct exact URL query
String buildQueryString(const char *basePath, JsonDocument &params) {
  String query = String(basePath);
  bool first = true;

  JsonObject obj = params.as<JsonObject>();
  for (JsonPair kv : obj) {
    query += first ? '?' : '&';
    first = false;

    query += kv.key().c_str();
    query += '=';

    if (kv.value().is<JsonArray>()) {
      bool firstElement = true;
      for (JsonVariant val : kv.value().as<JsonArray>()) {
        if (!firstElement) query += ',';
        query += val.as<String>();
        firstElement = false;
      }
    } else {
      query += kv.value().as<String>();
    }
  }
  return query;
}

// Convert Open-Meteo weather codes to human-readable strings
const char* getWeatherDescription(int code) {
  switch (code) {
    case 0:  return "Clear sky";
    case 1:  return "Mainly clear";
    case 2:  return "Partly cloudy";
    case 3:  return "Overcast";
    case 45: return "Fog";
    case 48: return "Depositing rime fog";
    case 51: return "Light drizzle";
    case 53: return "Moderate drizzle";
    case 55: return "Dense drizzle";
    case 61: return "Slight rain";
    case 63: return "Moderate rain";
    case 65: return "Heavy rain";
    case 71: return "Slight snow";
    case 73: return "Moderate snow";
    case 75: return "Heavy snow";
    case 80: return "Slight rain showers";
    case 81: return "Moderate rain showers";
    case 82: return "Violent rain showers";
    case 95: return "Thunderstorm";
    default: return "Unknown";
  }
}

// -------------------------------------------------------------------
// Request Builders
// -------------------------------------------------------------------
bool sendMapCityRequest() {
  client.stop(); // Clear existing socket
  if (!client.connect(geocoding_api_server, port)) return false;

  JsonDocument params;
  params["name"] = weather_city;
  params["count"] = 1;

  String resource = buildQueryString("/v1/search", params);
  Serial.print("Name Query:");
  Serial.println(resource);
  
  client.print("GET ");
  client.print(resource.c_str());
  client.print(" HTTP/1.0\r\nHost: geocoding-api.open-meteo.com\r\nConnection: close\r\n\r\n");
  headerState = 0;
  headerComplete = false;
  return true;
}

bool sendCurrentRequest() {
  client.stop(); // Clear existing socket
  if (!client.connect(server, port)) return false;

  JsonDocument params;
  params["latitude"] = weather_latitude;
  params["longitude"] = weather_longitude;

  JsonArray current = params["current"].to<JsonArray>();
  current.add("temperature_2m");
  current.add("wind_speed_10m");
  current.add("wind_direction_10m");
  current.add("weather_code");
  current.add("surface_pressure");
  current.add("rain");
  current.add("snowfall");
	current.add("precipitation");

  params["timezone"] = weather_time_zone;
  params["wind_speed_unit"] = "mph";
  params["temperature_unit"] = "fahrenheit";
  params["precipitation_unit"] = "inch";

  String resource = buildQueryString("/v1/forecast", params);

  client.print("GET ");
  client.print(resource.c_str());
  client.print(" HTTP/1.0\r\nHost: api.open-meteo.com\r\nConnection: close\r\n\r\n");
  
  headerState = 0;
  headerComplete = false;
  return true;
}

bool sendHourlyRequest() {
  client.stop(); // Clear existing socket
  if (!client.connect(server, port)) return false;

  JsonDocument params;
  params["latitude"] = weather_latitude;
  params["longitude"] = weather_longitude;

  JsonArray hourly = params["hourly"].to<JsonArray>();
  hourly.add("temperature_2m");
  hourly.add("precipitation_probability");
  hourly.add("precipitation");
  hourly.add("rain");
  hourly.add("snowfall");
  hourly.add("pressure_msl");
  hourly.add("wind_speed_10m");

  params["timezone"] = weather_time_zone;
  params["forecast_days"] = 1;
  params["wind_speed_unit"] = "mph";
  params["temperature_unit"] = "fahrenheit";
  params["precipitation_unit"] = "inch";

  String resource = buildQueryString("/v1/forecast", params);

  client.print("GET ");
  client.print(resource.c_str());
  client.print(" HTTP/1.0\r\nHost: api.open-meteo.com\r\nConnection: close\r\n\r\n");

  headerState = 0;
  headerComplete = false;

  return true;
}

bool sendDailyRequest() {
  client.stop(); // Clear existing socket
  if (!client.connect(server, port)) return false;

  JsonDocument params;
  params["latitude"] = weather_latitude;
  params["longitude"] = weather_longitude;

  JsonArray daily = params["daily"].to<JsonArray>();
  daily.add("temperature_2m_max");
  daily.add("temperature_2m_min");
  daily.add("snowfall_sum");
  daily.add("precipitation_probability_max");
  daily.add("weather_code");
  daily.add("wind_speed_10m_max");
  daily.add("wind_direction_10m_dominant");
  daily.add("rain_sum");
  //daily.add("precipitation_sum");
  daily.add("sunrise");
	daily.add("sunset");
	daily.add("relative_humidity_2m_mean");
  daily.add("cloud_cover_mean");
	
  params["timezone"] = weather_time_zone;
  params["wind_speed_unit"] = "mph";
  params["temperature_unit"] = "fahrenheit";
  params["precipitation_unit"] = "inch";

  String resource = buildQueryString("/v1/forecast", params);

  client.print("GET ");
  client.print(resource.c_str());
  client.print(" HTTP/1.0\r\nHost: api.open-meteo.com\r\nConnection: close\r\n\r\n");

  headerState = 0;
  headerComplete = false;
  return true;
}

bool sendAirQualityRequest() {
  //https://air-quality-api.open-meteo.com/v1/air-quality?latitude=48.5126&longitude=-122.61267&current=us_aqi,pm2_5,pm10&timezone=America%2FLos_Angeles&forecast_days=1
  client.stop(); // Clear existing socket
  if (!client.connect(air_quality_server, port)) return false;

  JsonDocument params;
  params["latitude"] = weather_latitude;
  params["longitude"] = weather_longitude;

  JsonArray current = params["current"].to<JsonArray>();
  current.add("us_aqi");
  current.add("pm2_5");
  current.add("pm10");

  params["timezone"] = weather_time_zone;

  String resource = buildQueryString("/v1/air-quality", params);  

  client.print("GET ");
  client.print(resource.c_str());
  client.print(" HTTP/1.0\r\nHost: air-quality-api.open-meteo.com\r\nConnection: close\r\n\r\n");
  headerState = 0;
  headerComplete = false;
  return true;
}

// -------------------------------------------------------------------
// Render Output Functions
// -------------------------------------------------------------------
void printMapCityData() {
  // Lets remember this data for retrieving the weather data.
  const char *name = doc["results"][0]["name"];
  weather_latitude = doc["results"][0]["latitude"];
  weather_longitude = doc["results"][0]["longitude"];
  weather_time_zone = String(doc["results"][0]["timezone"]);
  
#if defined(printForecast)
  Serial.println("\n====================== Map City ======================");
  Serial_printf("Name:                %s\n", name);
  Serial_printf("latitude:            %f\n", weather_latitude);
  Serial_printf("longitude:           %f\n", weather_longitude);
  Serial_printf("Time Zone:           %s\n", weather_time_zone.c_str());
#endif
  weather.location = String(name);
}

void printCurrentData() {
#if defined(printForecast)
  Serial.println("\n====================== CURRENT WEATHER ======================");
  Serial_printf("Time:                %s\n", doc["current"]["time"].as<const char*>());
  Serial_printf("Condition:           %s (Code %d)\n", 
                getWeatherDescription(doc["current"]["weather_code"].as<int>()), 
                doc["current"]["weather_code"].as<int>());
  Serial_printf("Temperature:         %.1f °F\n", doc["current"]["temperature_2m"].as<float>());
  Serial_printf("Pressure (MSL):      %.1f hPa\n", doc["current"]["pressure_msl"].as<float>());
  Serial_printf("Wind Speed:          %.1f mph\n", doc["current"]["wind_speed_10m"].as<float>());
  Serial_printf("Wind Direction:      %d°\n", doc["current"]["wind_direction_10m"].as<int>());
  Serial_printf("Rain:                %.2f in\n", doc["current"]["rain_sum"].as<float>());
  Serial_printf("Snowfall:            %.2f in\n", doc["current"]["snowfall_sum"].as<float>());
#endif

  weather.currentTime = String(doc["current"]["time"].as<const char*>());
  weather.currentWmoCode = doc["current"]["weather_code"].as<int>();
  weather.conditionText = String(getWeatherDescription(weather.currentWmoCode));
  weather.currentTemp = doc["current"]["temperature_2m"].as<float>();
  weather.pressure = doc["current"]["surface_pressure"].as<float>();
  weather.windSpeed = doc["current"]["wind_speed_10m"].as<float>();
  weather.windDirection = doc["current"]["wind_direction_10m"].as<int>();
  weather.rain = doc["current"]["rain_sum"].as<float>();
  weather.snow = doc["current"]["snowfall_sum"].as<float>();
  weather.precip = doc["current"]["precipitation_sum"].as<float>();
}

void printHourlyData() {
  [[maybe_unused]] JsonArray hourlyTime   = doc["hourly"]["time"].as<JsonArray>();
  [[maybe_unused]] JsonArray hourlyTemp   = doc["hourly"]["temperature_2m"].as<JsonArray>();
  [[maybe_unused]] JsonArray hourlyProb   = doc["hourly"]["precipitation_probability"].as<JsonArray>();
  [[maybe_unused]] JsonArray hourlyPrecip = doc["hourly"]["precipitation"].as<JsonArray>();
  [[maybe_unused]] JsonArray hourlyRain   = doc["hourly"]["rain"].as<JsonArray>();
  [[maybe_unused]] JsonArray hourlySnow   = doc["hourly"]["snowfall"].as<JsonArray>();
  [[maybe_unused]] JsonArray hourlyPress  = doc["hourly"]["pressure_msl"].as<JsonArray>();
  [[maybe_unused]] JsonArray hourlyWind   = doc["hourly"]["wind_speed_10m"].as<JsonArray>();

#if defined(printForecast)
  if (hourlyTime.size() > 0) {
    Serial.println("\n========================================== HOURLY FORECAST ==========================================");
    Serial.println("   Time       Temp     PoP%    Precipitation     Rain          Snow       Pressure     Wind");
    Serial.println("-----------------------------------------------------------------------------------------------------");

    size_t hoursToPrint = min(hourlyTime.size(), (size_t)24);
    for (size_t i = 0; i < hoursToPrint; i++) {
      const char* fullTime = hourlyTime[i];
      const char* timeOnly = (strlen(fullTime) >= 16) ? (fullTime + 11) : fullTime;

      Serial_printf("   %-8s  %4.1f °F  %3d%%       %5.2f in      %5.2f in      %5.2f in      %4.1f    %4.1f mph\n",
                    timeOnly,
                    hourlyTemp[i].as<float>(),
                    hourlyProb[i].as<int>(),
                    hourlyPrecip[i].as<float>(),
                    hourlyRain[i].as<float>(),
                    hourlySnow[i].as<float>(),
                    hourlyPress[i].as<float>(),
                    hourlyWind[i].as<float>());
    }
  }
#endif
}

void printDailyData() {
  JsonArray dailyTime   = doc["daily"]["time"].as<JsonArray>();
  JsonArray dailyMax    = doc["daily"]["temperature_2m_max"].as<JsonArray>();
  JsonArray dailyMin    = doc["daily"]["temperature_2m_min"].as<JsonArray>();
  JsonArray dailyPop    = doc["daily"]["precipitation_probability_max"].as<JsonArray>();
  JsonArray dailyPrecip = doc["daily"]["precipitation_sum"].as<JsonArray>();
  JsonArray dailyRain   = doc["daily"]["rain_sum"].as<JsonArray>();
  JsonArray dailySnow   = doc["daily"]["snowfall_sum"].as<JsonArray>();
  JsonArray dailyWind   = doc["daily"]["wind_speed_10m_max"].as<JsonArray>();
  JsonArray dailyGusts  = doc["daily"]["wind_gusts_10m_max"].as<JsonArray>();
  JsonArray dailyCode   = doc["daily"]["weather_code"].as<JsonArray>();
  JsonArray sunrise     = doc["daily"]["sunrise"].as<JsonArray>();
  JsonArray sunset      = doc["daily"]["sunset"].as<JsonArray>();
  JsonArray humidity    = doc["daily"]["relative_humidity_2m_mean"].as<JsonArray>();
  JsonArray dailyCloudCover = doc["daily"]["cloud_cover_mean"].as<JsonArray>();

  size_t count = min((size_t)7, dailyTime.size());

#if defined(printForecast)
  // 1. SERIAL LOGGING TABLE
  if (count > 0) {
    Serial.println("\n====================================================== DAILY FORECAST ======================================================");
    Serial.println("   Date       Max/Min Temp    PoP%   Precip    Rain    Snow    MaxWind   Gusts   Sunrise  Sunset   Hum%  Condition");
    Serial.println("----------------------------------------------------------------------------------------------------------------------------");

    for (size_t i = 0; i < count; i++) {
      const char* fullDate   = dailyTime[i];
      const char* dateOnly   = (strlen(fullDate) >= 10) ? (fullDate + 5) : fullDate;
      const char* fullRise   = sunrise[i] ? sunrise[i].as<const char*>() : "";
      const char* fullSet    = sunset[i]  ? sunset[i].as<const char*>()  : "";
      
      String riseTime = (strlen(fullRise) >= 16) ? String(fullRise + 11) : "--:--"; // Extract HH:MM
      String setTime  = (strlen(fullSet) >= 16)  ? String(fullSet + 11)  : "--:--";

      Serial_printf("   %-8s  %4.1f / %4.1f deg F  %3d%%  %5.2f in %5.2f in %5.2f in %4.1f mph %4.1f mph  %s  %s   %3d%%  %s\n",
                    dateOnly,
                    dailyMax[i].as<float>(),
                    dailyMin[i].as<float>(),
                    dailyPop[i].as<int>(),
                    dailyPrecip[i].as<float>(),
                    dailyRain[i].as<float>(),
                    dailySnow[i].as<float>(),
                    dailyWind[i].as<float>(),
                    dailyGusts[i].as<float>(),
                    riseTime.c_str(),
                    setTime.c_str(),
                    humidity[i].as<int>(),
                    getWeatherDescription(dailyCode[i].as<int>()));
    }
    Serial.println("================================================================================--------------------------------------------\n");
  }
#endif

  // 2. STORE INTO STRUCT
  for (size_t i = 0; i < count; i++) {
  const char* fullDate = dailyTime[i]; // "YYYY-MM-DD"
    
    // Keep original YYYY-MM-DD string to compute day-of-week
    String dateString = String(fullDate);

    // Store 3-letter weekday (e.g., "Mon", "Tue")
    weather.days[i] = getDayOfWeek(dateString);

    // Store short display date (MM-DD)
    weather.dates[i] = (strlen(fullDate) >= 10) ? String(fullDate + 5) : dateString;
    //weather.dates[i]     = (strlen(fullDate) >= 10) ? String(fullDate + 5) : String(fullDate);
    //weather.days[i]      = "Day " + String(i + 1);
    weather.tempsHigh[i] = dailyMax[i].as<float>();
    weather.tempsLow[i]  = dailyMin[i].as<float>();
    weather.pop[i]       = dailyPop[i].as<int>();
    weather.cloudCover[i]= dailyCloudCover[i].as<float>();
    weather.wmoCodes[i]  = dailyCode[i].as<float>();
    //try to refine wmo code 3
    weather.wmoCodes[i] = refineWMOCode(weather.wmoCodes[i], weather.cloudCover[i], (float) dailyPop[i].as<int>());

    weather.precipSum[i] = dailyPrecip[i].as<float>();
    weather.rainSum[i]   = dailyRain[i].as<float>();
    weather.snowSum[i]   = dailySnow[i].as<float>();
    weather.windMax[i]   = dailyWind[i].as<float>();
    weather.gustsMax[i]  = dailyGusts[i].as<float>();

    // Parse daily sun & humidity values
    const char* fullRise = sunrise[i] ? sunrise[i].as<const char*>() : "";
    const char* fullSet  = sunset[i]  ? sunset[i].as<const char*>()  : "";
    weather.dailySunrise[i]  = (strlen(fullRise) >= 16) ? String(fullRise + 11) : "--:--";
    weather.dailySunset[i]   = (strlen(fullSet) >= 16)  ? String(fullSet + 11)  : "--:--";
    weather.dailyHumidity[i] = humidity[i].as<int>();
  }

  // Keep global current-day metrics updated
  if (count > 0) {
    weather.sunrise  = weather.dailySunrise[0];
    weather.sunset   = weather.dailySunset[0];
    weather.humidity = weather.dailyHumidity[0];
  }

  tft.fillScreen(COLOR_BG);
}

void printAirQualityData() {
#if defined(printForecast)
  Serial.println("\n====================== CURRENT AIR QUALITY ======================");
  //serializeJsonPretty(doc, Serial); 

  Serial_printf("AQI:         %d\n", doc["current"]["us_aqi"].as<int>());
  Serial_printf("PM 2.5:      %.1f hPa\n", doc["current"]["pm2_5"].as<float>());
  Serial_printf("PM 10:       %.1f hPa\n", doc["current"]["pm10"].as<float>());
#endif

  weather.AQI = doc["current"]["us_aqi"].as<int>();
  weather.PM25 = doc["current"]["pm2_5"].as<float>();
  weather.PM10 = doc["current"]["pm10"].as<float>();
}


// Stream reader that strips HTTP headers and parses incoming payload
bool processIncomingStream(void (*outputFunc)()) {
  while (client.available() > 0 && !headerComplete) {
    char c = client.read();
    switch (headerState) {
      case 0: headerState = (c == '\r') ? 1 : 0; break;
      case 1: headerState = (c == '\n') ? 2 : 0; break;
      case 2: headerState = (c == '\r') ? 3 : 0; break;
      case 3: headerState = (c == '\n') ? 4 : 0; break;
    }
    if (headerState == 4) {
      headerComplete = true;
      
      // Wait up to 1000ms for incoming payload bytes to buffer before deserializing
      uint32_t start = millis();
      while (client.available() == 0 && (millis() - start < 1000)) {
        delay(10);
      }

      doc.clear();
      DeserializationError error = deserializeJson(doc, client);
      if (!error) {
        outputFunc();
      } else {
        Serial_printf("JSON Parsing failed: %s\n", error.c_str());
        serializeJsonPretty(doc, Serial); 
      }
    }
  }

  if (!client.connected() && client.available() == 0) {
    client.stop();
    return true; // Stream fully processed
  }
  return false;
}


// refine open-meteo wmo code 3 grouping of 1,2,3 into single code
int refineWMOCode(int originalCode, float cloudCoverMean, float popMax) {
  // Only refine dry non-precipitating codes (0 to 3)
  if (originalCode > 3) {
    return originalCode; // Keep rain, snow, fog, thunderstorm codes intact
  }

  // Override logic based on mean daily cloud cover percentage
  if (cloudCoverMean < 10.0) {
    return 0; // Clear sky
  } 
  else if (cloudCoverMean >= 10.0 && cloudCoverMean < 35.0) {
    return 1; // Mainly clear
  } 
  else if (cloudCoverMean >= 35.0 && cloudCoverMean < 70.0) {
    return 2; // Partly cloudy
  } 
  else {
    // If cloud cover is high, but PoP is very low, ensure it stays as Overcast (3)
    // rather than escalating to a rain/drizzle code.
    return 3; // Overcast
  }
}

String getDayOfWeek(String dateStr) {
  // Expected input format: "YYYY-MM-DD" or "MM-DD"
  if (dateStr.length() < 10) return "Day";

  int year  = dateStr.substring(0, 4).toInt();
  int month = dateStr.substring(5, 7).toInt();
  int day   = dateStr.substring(8, 10).toInt();

  // Zeller's Congruence algorithm for day-of-week calculation
  if (month < 3) {
    month += 12;
    year--;
  }
  
  int k = year % 100;
  int j = year / 100;
  int h = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;

  // Zeller's result mapping (0 = Saturday, 1 = Sunday, etc.)
  const char* dayNames[] = {"Sat", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri"};
  return String(dayNames[h]);
}