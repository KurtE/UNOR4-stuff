
// Screen state tracker: 0 = Main Dashboard, 1 = Day Detail Screen
enum ScreenState { SCREEN_MAIN, SCREEN_DETAIL, SCREEN_KEYBOARD };
ScreenState currentScreen = SCREEN_MAIN;
int currentSelectedDay = -1;

// Global state tracking
enum AppState {
  START,
  FETCH_MAP_CITY_TO_LOCATION,
  READ_MAP_CITY_TO_LOCATION,
  FETCH_CURRENT,
  READ_CURRENT,
  FETCH_HOURLY,
  READ_HOURLY,
  FETCH_DAILY,
  READ_DAILY,
  FETCH_DAILY2,
  READ_DAILY2,
  FETCH_AIR_QUALITY,
  READ_AIR_QUALITY,
  DONE_APP
};

AppState appState = START;

struct AQI_Category {
  uint16_t aqiMin;
  uint16_t aqiMax;
  const char* label;
  uint16_t color;
};

struct Breakpoint {
  float cLow, cHigh;
  int iLow, iHigh;
};

// RGB565 Colors matching standard EPA thresholds
#define AQI_COLOR_GOOD        0x3672  // Green
#define AQI_COLOR_MODERATE    0xEFE0  // Yellow
#define AQI_COLOR_UNHEALTHY_S 0xFBE0  // Orange
#define AQI_COLOR_UNHEALTHY   0xF980  // Red
#define AQI_COLOR_VERY_UNH    0x8800  // Dark Red / Maroon
#define AQI_COLOR_HAZARDOUS   0x7010  // Purple

// Breakpoints defined directly from EPA table
const AQI_Category AQI_TABLE[] = {
  { 0,   50,  "Good",                            AQI_COLOR_GOOD },
  { 51,  100, "Moderate",                        AQI_COLOR_MODERATE },
  { 101, 150, "Unhealthy for Sensitive Groups", AQI_COLOR_UNHEALTHY_S },
  { 151, 200, "Unhealthy",                       AQI_COLOR_UNHEALTHY },
  { 201, 300, "Very Unhealthy",                  AQI_COLOR_VERY_UNH },
  { 301, 500, "Hazardous",                       AQI_COLOR_HAZARDOUS }
};

// PM2.5 24-hour concentration limits from provided table
const Breakpoint PM25_BREAKPOINTS[] = {
  { 0.0,    9.0,     0,  50 },
  { 9.0,   35.4,    51, 100 },
  { 35.5,  55.4,   101, 150 },
  { 55.5, 125.4,   151, 200 },
  { 125.5, 225.4,  201, 300 },
  { 225.5, 325.4,  301, 400 },
  { 325.5, 500.4,  401, 500 }
};


/*************************************************/
// Custom Colors (RGB565 format)
#define COLOR_BG        0x0000 // Black
#define COLOR_BLACK     0x0000
#define COLOR_WHITE     0xFFFF
#define COLOR_CYAN      0x07FF
#define COLOR_YELLOW    0xFFE0
#define COLOR_ORANGE    0xFD20
#define COLOR_GRAY      0x7BEF
#define COLOR_DARKGREY  0x7BEF
#define COLOR_NAVY      0x000F
#define COLOR_BLUE      0x00FF