#ifndef _arduino_r4wifi_matrix_gfx_h_
#define _arduino_r4wifi_matrix_gfx_h_
#include <Arduino_LED_Matrix.h>
#include <Adafruit_GFX.h>

extern ArduinoLEDMatrix matrix;

#define MATRIX_BLACK 0   ///< Draw 'off' pixels
#define MATRIX_WHITE 1   ///< Draw 'on' pixels
#define MATRIX_INVERSE 2 ///< Invert pixels
#define MATRIX_WIDTH 12
#define MATRIX_HEIGHT 8

#ifndef NO_ADAFRUIT_COLOR_COMPATIBILITY
#define BLACK MATRIX_BLACK     ///< Draw 'off' pixels
#define WHITE MATRIX_WHITE     ///< Draw 'on' pixels
#define INVERSE MATRIX_INVERSE ///< Invert pixels
#endif


class ArduinoLEDMatrixGFX : public Adafruit_GFX {
  public:
    // constructor
    ArduinoLEDMatrixGFX(); 

    // destructor
    ~ArduinoLEDMatrixGFX(); 

    // begin start up the underlying matrix object
    bool begin();

    // Clears the frame buffer;
    void clearDisplay();

    // Update the screen from the current frame buffer
    void display();

    // virtual overrides - draw pixel to frame buffer
    virtual  void drawPixel(int16_t x, int16_t y, uint16_t color);

    // Fill screen
    virtual void fillScreen(uint16_t color);


    //virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    //virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    uint32_t _frame_buffer[3]; // 96 bits.


};

#endif
