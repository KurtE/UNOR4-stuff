#if 0
void * myOpen(const char *filename, int32_t *size) {
  pngFile = SD.open(filename);
  if (pngFile) {
    *size = pngFile.size();
    return &pngFile;
  }
  return NULL;
}

void myClose(void *handle) {
  if (pngFile) pngFile.close();
}

int32_t myRead(PNGFILE *handle, uint8_t *buffer, int32_t length) {
  if (!pngFile) return 0;
  return pngFile.read(buffer, length);
}

int32_t mySeek(PNGFILE *handle, int32_t position) {
  if (!pngFile) return 0;
  return pngFile.seek(position);
}

// Change return type from 'void' to 'int'
int pngDrawCallback(PNGDRAW *pDraw) {
  uint16_t usPixels[480]; // Buffer sized for maximum width
  
  // Convert current scanline to RGB565 format
  png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0x0000);
  
  // Push pixels to display hardware
  tft.writeRect(pngX, pngY + pDraw->y, pDraw->iWidth, 1, usPixels);

  return 1; // Return 1 to signal successful scanline rendering
}

// Reads PNG header metadata and outputs details to Serial
bool getPNGInfo(const char *filename) {
  int rc = png.open(filename, myOpen, myClose, myRead, mySeek, NULL);
  
  if (rc == PNG_SUCCESS) {
    Serial_printf("--- PNG Info: %s ---\n", filename);
    Serial_printf("Dimensions : %d x %d px\n", png.getWidth(), png.getHeight());
    Serial_printf("BPP        : %d bits per pixel\n", png.getBpp());
    Serial_printf("Color Type : %d\n", png.getPixelType());
    Serial_printf("Buffer Size: %d bytes required\n", png.getBufferSize());
    Serial.println("---------------------------");
    
    png.close(); // Close file after inspecting
    return true;
  } else {
    Serial_printf("Failed to open PNG '%s'. Error code: %d\n", filename, rc);
    return false;
  }
}
#endif
// Helper to draw PNG files from SD card at (x, y) coordinates
void drawPNG(const char *filename, int x, int y) {

 #if 1
    const uint16_t *image = (const uint16_t *)filename;
 #ifdef ARDUINO_GIGA
    tft.drawRGBBitmap(x, y, image, 64, 64);
#else
    tft.writeRect(x, y, 64, 64, image);
#endif    

 
 #else
  pngX = x;
  pngY = y;

  // Optional: print image info to Serial before drawing
  //getPNGInfo(filename);

  int rc = png.open(filename, myOpen, myClose, myRead, mySeek, pngDrawCallback);
  if (rc == PNG_SUCCESS) {
    //tft.startWrite();
    png.decode(NULL, 0); // Decode and render via pngDrawCallback
    //tft.endWrite();
    png.close();
  } else {
    Serial_printf("PNG Decode Error [%d] on file: %s\n", rc, filename);
  }
 #endif  
}