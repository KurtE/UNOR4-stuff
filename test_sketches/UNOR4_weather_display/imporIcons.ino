
void drawPNG(const char *filename, int x, int y) {
#ifdef USE_COMPRESSED_IMAGE
    struct image_info *image_info = (struct image_info*)filename;
    if (image_info->image_top) {
        tft.fillRect(x, y, 64, image_info->image_top, COLOR_BLACK);
    }
    if (image_info->image_bottom < 63) {
        tft.fillRect(x, y + image_info->image_bottom, 64, 63 - image_info->image_bottom, COLOR_BLACK);
    }

    if (image_info->palette == nullptr) {
        tft.writeRect(x, y + image_info->image_top, 64, (image_info->image_bottom - image_info->image_top) + 1, (const uint16_t *)image_info->image);
    } else {
        tft.writeRect8BPP(x, y + image_info->image_top, 64, (image_info->image_bottom - image_info->image_top) + 1, (const uint8_t *)image_info->image, image_info->palette);
        //tft.writeRect8BPP(x, y, 64, 64, (const uint8_t *)image_info->image, image_info->palette);
    }
#else
    const uint16_t *image = (const uint16_t *)filename;
    tft.writeRect(x, y, 64, 64, image);
#endif
};

void drawKeyboard(const char *filename, int x, int y) {
#ifdef USE_COMPRESSED_IMAGE
    struct image_info *image_info = (struct image_info*)filename;

    if (image_info->palette == nullptr) {
        tft.writeRect(x, y, 40, 14, (const uint16_t *)image_info->image);
    } else {
        tft.writeRect8BPP(x, y, 40, 14, (const uint8_t *)image_info->image, image_info->palette);
    }
#else
    const uint16_t *image = (const uint16_t *)filename;
    tft.writeRect(x, y, 40, 14, image);
#endif 
}
#if 0
int pngDrawCallback(PNGDRAW *pDraw) {
//uint16_t usPixels[120];  // Buffer sized for maximum width
uint16_t usPixels[1];

  // Convert current scanline to RGB565 format
  png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0x0000);

  // Push pixels to display hardware
  //tft.writeRect(pngX, pngY + pDraw->y, pDraw->iWidth, 1, usPixels);
  tft.drawRGBBitmap(pngX,  pngY + pDraw->y, usPixels, pDraw->iWidth, 1);
  return 1;  // Return 1 to signal successful scanline rendering
}



void drawPNG(const uint8_t *image, int image_length, int x, int y) {

  //const uint16_t *image = (const uint16_t *)filename;
  //tft.writeRect(x, y, 64, 64, image);
  //tft.drawRGBBitmap(x, y, image, 64, 64);
  pngX = x;
  pngY = y;
  //png.openFLASH((uint8_t*)image, image_length, pngDrawCallback);
  //png.decode(NULL, 0);  // Decode and render via pngDrawCallback
  //tft.endWrite();
  //png.close();
}


void drawKeyboard(const char *filename, int x, int y) {

#if 0
    const uint16_t *image = (const uint16_t *)filename;
    //tft.writeRect(x, y, 40, 14, image);
    tft.drawRGBBitmap(x, y, image, 40, 14);
#endif
}
#endif