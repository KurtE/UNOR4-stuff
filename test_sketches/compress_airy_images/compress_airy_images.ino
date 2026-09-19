
//****************************************************/
// Returns the file name corresponding to the WMO Weather Code
// lets see if we can avoid the files...
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

struct image_info {
  const void *image;
  const uint16_t *palette;
  uint8_t image_top;
  uint8_t image_bottom;
};

void compress_image_file(const char *name, const uint16_t *image, size_t image_size) {
  uint16_t palette[256];
  uint16_t count_of_colors = 0;

  // how may possible rows or columns can we skip.
  uint8_t image_top=0, image_bottom=0, skip_left=0, skip_right = 0;

  for (size_t i = 0; i < image_size; i++) {
    uint16_t palette_index;
    for (palette_index = 0; palette_index < count_of_colors; palette_index++) {
      if (image[i] == palette[palette_index]) break;
    }
    if (palette_index == count_of_colors) {
      palette[count_of_colors++] = image[i];
      if (count_of_colors > 256) break;
    }
  }

  // check to see if we can skip rows and / or columns at edges
  if (image_size == (64*64)) {
    // First scan top
    for (size_t i = 0; i < image_size; i++) {
      if (image[i] != 0) {
        image_top = i / 64; 
        break;
      }
    }

    // bottom
    for (int i = image_size - 1; i >= 0; i--) {
      if (image[i] != 0) {
        image_bottom =  i / 64; 
        break;
      }
    }
#if 0
    // left
    skip_left = 64;
    for (size_t i = 0; i < image_size; i += 64) {
      for (uint8_t j = 0; j < skip_left; j ++) {
        if (image[i+j] != 0) {
          skip_left = j;
          break;
        }
      }
      if (skip_left == 0) break; // nothing to skip
    }
      
    // right
    skip_left = 64;
    for (size_t i = 0; i < image_size; i++) {
      for (uint8_t j = 0; j < skip_left; j ++) {
        if (image[i+j] <= 0) {
          skip_left = j;
          break;
        }
      }
      if (skip_left == 0) break; // nothing to skip
    }
#endif

  }


  Serial.println("\n//===============================================");
  if (count_of_colors > 256) Serial.printf("// %s: > 256 colors\n", name);
  else Serial.printf("// %s: %u colors\n", name, count_of_colors);
  Serial.printf("// Top: %u Bottom: %u\n", image_top, image_bottom);
  Serial.println("//===============================================");

  // <= 256 colors use palette
  if (count_of_colors <= 256) {
    Serial.printf("static const uint16_t %s_palette[] PROGMEM = {\n    ", name);
    for (uint16_t i = 0; i < count_of_colors; i++) {
      Serial.printf("0x%04X, ", palette[i]);
      if ((i & 0xf) == 0xf) Serial.printf("\n    ");
    }
    Serial.println("\n};");
    Serial.printf("static const uint8_t %s_data[] PROGMEM = {\n    ", name);
#if 1
    size_t image_last = (image_size == (64*64))? ((image_bottom + 1) * 64) : image_size;
    for (size_t i = image_top * 64 ; i < image_last; i++) {
#else
    for (size_t i = 0; i < image_size; i++) {
#endif    
      uint16_t palette_index;
      for (palette_index = 0; palette_index < count_of_colors; palette_index++) {
        if (image[i] == palette[palette_index]) break;
      }
      Serial.printf("%u, ", palette_index);
      if ((i & 0x1f) == 0x1f) Serial.print("\n    ");
    }
    Serial.println("\n};");
    Serial.printf("const struct image_info %s PROGMEM = { (void*)%s_data, %s_palette, %u, %u }; \n", name, name, name, image_top, image_bottom);
  
  } else {
    // > 256 colors row image...
    Serial.printf("static const uint16_t %s_data[] PROGMEM = {\n    ", name);
#if 1
    size_t image_last = (image_size == (64*64))? ((image_bottom + 1) * 64) : image_size;
    for (size_t i = image_top * 64 ; i < image_last; i++) {
#else
    for (size_t i = 0; i < image_size; i++) {
#endif    
      Serial.printf("0x%04X, ", image[i]);
      if ((i & 0xf) == 0xf) Serial.printf("\n    ");
    }
    Serial.println("\n};");
    Serial.printf("const struct image_info %s PROGMEM = { (void*)%s_data, nullptr, %u, %u }; \n", name, name, image_top, image_bottom);
  }

}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  compress_image_file("image_clear", image_clear, sizeof(image_clear) / sizeof(image_clear[0]));
  compress_image_file("image_dense_drizzle", image_dense_drizzle, sizeof(image_dense_drizzle) / sizeof(image_dense_drizzle[0]));
  compress_image_file("image_dense_freezing_drizzle", image_dense_freezing_drizzle, sizeof(image_dense_freezing_drizzle) / sizeof(image_dense_freezing_drizzle[0]));
  compress_image_file("image_fog", image_fog, sizeof(image_fog) / sizeof(image_fog[0]));
  compress_image_file("image_heavy_freezing_rain", image_heavy_freezing_rain, sizeof(image_heavy_freezing_rain) / sizeof(image_heavy_freezing_rain[0]));
  compress_image_file("image_heavy_rain", image_heavy_rain, sizeof(image_heavy_rain) / sizeof(image_heavy_rain[0]));
  compress_image_file("image_heavy_snowfall", image_heavy_snowfall, sizeof(image_heavy_snowfall) / sizeof(image_heavy_snowfall[0]));
  compress_image_file("image_light_drizzle", image_light_drizzle, sizeof(image_light_drizzle) / sizeof(image_light_drizzle[0]));
  compress_image_file("image_light_freezing_drizzle", image_light_freezing_drizzle, sizeof(image_light_freezing_drizzle) / sizeof(image_light_freezing_drizzle[0]));
  compress_image_file("image_light_freezing_rain", image_light_freezing_rain, sizeof(image_light_freezing_rain) / sizeof(image_light_freezing_rain[0]));
  compress_image_file("image_light_rain", image_light_rain, sizeof(image_light_rain) / sizeof(image_light_rain[0]));
  compress_image_file("image_moderate_drizzle", image_moderate_drizzle, sizeof(image_moderate_drizzle) / sizeof(image_moderate_drizzle[0]));
  compress_image_file("image_moderate_rain", image_moderate_rain, sizeof(image_moderate_rain) / sizeof(image_moderate_rain[0]));
  compress_image_file("image_moderate_snowfall", image_moderate_snowfall, sizeof(image_moderate_snowfall) / sizeof(image_moderate_snowfall[0]));
  compress_image_file("image_mostly_clear", image_mostly_clear, sizeof(image_mostly_clear) / sizeof(image_mostly_clear[0]));
  compress_image_file("image_overcast", image_overcast, sizeof(image_overcast) / sizeof(image_overcast[0]));
  compress_image_file("image_partly_cloudy", image_partly_cloudy, sizeof(image_partly_cloudy) / sizeof(image_partly_cloudy[0]));
  compress_image_file("image_rime_fog", image_rime_fog, sizeof(image_rime_fog) / sizeof(image_rime_fog[0]));
  compress_image_file("image_slight_snowfall", image_slight_snowfall, sizeof(image_slight_snowfall) / sizeof(image_slight_snowfall[0]));
  compress_image_file("image_snowflake", image_snowflake, sizeof(image_snowflake) / sizeof(image_snowflake[0]));
  compress_image_file("image_thunderstorm_with_hail", image_thunderstorm_with_hail, sizeof(image_thunderstorm_with_hail) / sizeof(image_thunderstorm_with_hail[0]));
  compress_image_file("image_thunderstorm", image_thunderstorm, sizeof(image_thunderstorm) / sizeof(image_thunderstorm[0]));
  compress_image_file("image_keypad", image_keypad, sizeof(image_keypad) / sizeof(image_keypad[0]));

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(500);
}