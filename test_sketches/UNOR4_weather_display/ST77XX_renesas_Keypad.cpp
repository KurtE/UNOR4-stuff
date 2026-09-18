/*
  The MIT License (MIT)

  library writen by Kris Kasprzak

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  On a personal note, if you develop an application or product using this library
  and make millions of dollars, I'm happy for you!

*/

#include "ST77XX_renesas_Keypad.h"

#if __has_include("Adafruit_FT6206.h")
#include <Adafruit_FT6206.h>
#define FT
#define touch_dev Adafruit_FT6206
#elif __has_include("XPT2046_Touchscreen.h")
#include <XPT2046_Touchscreen.h>
#define touch_dev XPT2046_Touchscreen
#define XPT
#endif

//#define debug

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// keyboard

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Keyboard::Keyboard(ST77XX_renesas *Display, touch_dev *Touch) {
  d = Display;
  t = Touch;
}
void Keyboard::init(uint16_t BackColor, uint16_t TextColor, uint16_t ButtonColor, uint16_t BorderColor, uint16_t PressedTextColor, 
              uint16_t PressedButtonColor, uint16_t PressedBorderColor, const ILI9341_t3_font_t &ButtonFont) {
  kcolor = BackColor;
  tcolor = TextColor;
  bcolor = ButtonColor;
  rcolor = BorderColor;
  inputt = BackColor;
  inputb = TextColor;
  ptcolor = PressedTextColor;
  pbcolor = PressedButtonColor;
  brcolor = PressedBorderColor;
  bfont = ButtonFont;
  clickpin = -1;
  rad = 0;
  Size = BUTTON_SIZE;
  screenX0 = 0, screenX320 = 0, screenY0 = 480, screenY240 = 320;
    
}


void Keyboard::setTouchLimits(uint16_t ScreenLeft, uint16_t ScreenRight, uint16_t ScreenTop, uint16_t ScreenBottom) {

  screenX0 = ScreenLeft;
  screenX320 = ScreenRight;
  screenY0 = ScreenTop;
  screenY240 = ScreenBottom;
	Serial.print(screenX0); Serial.print(", "); Serial.println(screenX320);
	Serial.print(screenY0); Serial.print(", "); Serial.println(screenY240);


}

void Keyboard::setDisplayColor(uint16_t TextColor, uint16_t BackColor) {
  inputt = TextColor;
  inputb = BackColor;
}

void Keyboard::BuildButton(BUTTON *temp, int Col, int Row) {
  temp->x = Col;
  temp->y = Row;
  temp->w = 1;
}

void Keyboard::BuildButton(BUTTON *temp, int Col, int Row, uint8_t Wide) {
  temp->x = Col;
  temp->y = Row;
  temp->w = Wide;
}

void Keyboard::DrawButton(BUTTON *temp, uint8_t ASCII, uint8_t State) {


  if (State == BUTTON_PRESSED) {
    if (rad > 0) {
      d->fillRoundRect(temp->x, temp->y, Size + ((temp->w - 1) * (2 + Size)), Size, rad, pbcolor);
	  d->drawRoundRect(temp->x, temp->y, Size + ((temp->w - 1) * (2 + Size)), Size, rad, brcolor);
    } else {
      d->fillRect(temp->x, temp->y, Size + ((temp->w - 1) * (2 + Size)), Size, pbcolor);
	  d->drawRect(temp->x, temp->y, Size + ((temp->w - 1) * (2 + Size)), Size, brcolor);
    }
    d->setTextColor(ptcolor, bcolor);
  } else {
    if (rad > 0) {
      d->fillRoundRect(temp->x, temp->y, Size + ((temp->w - 1) * (2 + Size)), Size, rad, bcolor);
	  d->drawRoundRect(temp->x, temp->y, Size + ((temp->w - 1) * (2 + Size)), Size, rad, rcolor);
    } else {
      d->fillRect(temp->x, temp->y, Size + ((temp->w - 1) * (2 + Size)), Size, bcolor);
	  d->drawRect(temp->x, temp->y, Size + ((temp->w - 1) * (2 + Size)), Size, rcolor);
    }
    d->setTextColor(tcolor, bcolor);
  }
  
  d->setFont(bfont);
#if __has_include("ILI9341_t3.h")
  if (ASCII == 168) {	  
	xm = d->measureTextWidth("Space");	 // returns inconsistent values	
	ym = d->measureTextHeight("Space");
	d->setCursor(temp->x + ((4*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("Space");
  } else if (ASCII == 169) {
	xm = d->measureTextWidth("Done");	 // returns inconsistent values	
	ym = d->measureTextHeight("Done");
	d->setCursor(temp->x + ((3*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("Done");
  } else if (ASCII == 170) {
	xm = d->measureTextWidth("Cancel");	 // returns inconsistent values	
	ym = d->measureTextHeight("Cancel");
	d->setCursor(temp->x + ((4*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("Cancel");
  } else if (ASCII == 171) {
	xm = d->measureTextWidth("Back");	 // returns inconsistent values	
	ym = d->measureTextHeight("Back");
	d->setCursor(temp->x + ((3*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("Back");
  } else if (ASCII == 172) {
	xm = d->measureTextWidth("Aa");	 // returns inconsistent values	
	ym = d->measureTextHeight("Aa");
	d->setCursor(temp->x + ((3*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("Aa");
  } else if (ASCII == 173) {
	xm = d->measureTextWidth("#$%^");	 // returns inconsistent values	
	ym = d->measureTextHeight("#$%^");
	d->setCursor(temp->x + ((3*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("#$%^");
  } else {
	xm = d->measureTextWidth("0");	 // returns inconsistent values	
	ym = d->measureTextHeight("0");
	d->setCursor(temp->x + (Size - xm)/2,temp->y+(Size - ym)/2);
	d->print((char) ASCII);	
  }
#else
  if (ASCII == 168) {	  
	d->getTextBounds("Space", 0, 0, &xT, &yT, &wT, &hT);
	xm = wT;	 // returns inconsistent values	
	ym = hT;
	d->setCursor(temp->x + ((4*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("Space");
  } else if (ASCII == 169) {
	d->getTextBounds("Done", 0, 0, &xT, &yT, &wT, &hT);
	xm = wT;	 // returns inconsistent values	
	ym = hT;
	d->setCursor(temp->x + ((3*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("Done");
  } else if (ASCII == 170) {
	d->getTextBounds("Cancel", 0, 0, &xT, &yT, &wT, &hT);
	xm = wT;	 // returns inconsistent values	
	ym = hT;
	d->setCursor(temp->x + ((4*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("Cancel");
  } else if (ASCII == 171) {
	d->getTextBounds("Back", 0, 0, &xT, &yT, &wT, &hT);
	xm = wT;	 // returns inconsistent values	
	ym = hT;
	d->setCursor(temp->x + ((3*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("Back");
  } else if (ASCII == 172) {
	d->getTextBounds("Aa", 0, 0, &xT, &yT, &wT, &hT);
	xm = wT;	 // returns inconsistent values	
	ym = hT;
	d->setCursor(temp->x + ((3*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("Aa");
  } else if (ASCII == 173) {
	d->getTextBounds("#$%^", 0, 0, &xT, &yT, &wT, &hT);
	xm = wT;	 // returns inconsistent values	
	ym = hT;
	d->setCursor(temp->x + ((3*Size) - xm)/2,temp->y+(Size - ym)/2);
	d->print("#$%^");
  } else {
	d->getTextBounds("0", 0, 0, &xT, &yT, &wT, &hT);
	xm = wT;	 // returns inconsistent values	
	ym = hT;
	d->setCursor(temp->x + (Size - xm)/2,temp->y+(Size - ym)/2);
	d->print((char) ASCII);	
  }
#endif
}

bool Keyboard::Pressed(BUTTON *temp, uint8_t ASCII) {
  bool found = false;
  bool redraw = true;
  bool redrawoff = false;

  if ((BtnX > temp->x) && (BtnX < (temp->x + (Size * temp->w)))) {
    if ((BtnY > temp->y) && (BtnY < (temp->y + Size))) {

	if (clickpin > 0) {
	  analogWrite(clickpin, 60);
	  delay(5);
	  analogWrite(clickpin, 0);
	}

      while (t->touched()) {
        if (((BtnX > temp->x) && (BtnX < (temp->x + (Size * temp->w)))) && ((BtnY > temp->y) && (BtnY < (temp->y + Size)))) {
          if (redraw) {
            DrawButton(temp, ASCII, BUTTON_PRESSED);
            redraw = false;
            redrawoff = true;
          }
          found = true;
        } else {
          if (redrawoff) {
            DrawButton(temp, ASCII, BUTTON_RELEASED);
            redrawoff = false;
          }
          found = false;
          redraw = true;
        }

        ProcessTouch();
      }

      DrawButton(temp, ASCII, BUTTON_RELEASED);
      return found;
    }
  }
  return false;
}

void Keyboard::DisplayInput() {
	d->getTextBounds("A", 0, 0, &xT, &yT, &wT, &hT);

	ym = hT;

	d->fillRect(Col1, Row0, Col10 + (2 * 9), Size, inputt);
	d->setCursor(Col1 + 5, Row0 + +(Size - ym)/2);
	d->setFont(bfont);
	d->setTextColor(inputb, inputt);

  if (hideinput) {
	d->print(hc);
  } else {
	d->print(dn);
  }

}

void Keyboard::hideInput() {

  hideinput = true;
}


void Keyboard::setCornerRadius(uint8_t Radius) {
  rad = Radius;
  if (Radius > 10) {
    rad = 10;
  }
}

void Keyboard::getInput() {

  uint8_t np = 0;
  uint16_t b = 0, i = 0;
  CapsLock = false;
  bool SpecialChar = false;
  bool KeepIn = true;
  bool found = false;
  memset(dn, '\0', MAX_KEYBOARD_CHARS + 2);
  memset(hc, '\0', MAX_KEYBOARD_CHARS + 2);

	strcpy(dn, data);

  if (strlen(data) > 0) {
    strcpy(dn, data);
    np = strlen(dn);
  }
  
    if (hideinput){
		for (i = 0; i < strlen(dn); i++){	  
		  hc[i] = '*';
		}
	}
  

  BUTTON Buttons[74];

  BuildButton(&Buttons[0], Col1, Row1);      //,  "!");
  BuildButton(&Buttons[1], Col5, Row3);      //,  "''");
  BuildButton(&Buttons[2], Col3, Row1);      //,,  "#");
  BuildButton(&Buttons[3], Col4, Row1);      //,,  "$");
  BuildButton(&Buttons[4], Col5, Row1);      //,,  "%");
  BuildButton(&Buttons[5], Col7, Row1);      //,,  "&");
  BuildButton(&Buttons[6], Col6, Row3);      //,,  "'");
  BuildButton(&Buttons[7], Col9, Row1);      //,,  "(");
  BuildButton(&Buttons[8], Col10, Row1);     //,,  ")");
  BuildButton(&Buttons[9], Col8, Row1);      //,,  "*");
  BuildButton(&Buttons[10], Col6, Row2);     //,,  "+");
  BuildButton(&Buttons[11], Col7, Row3);     //,,  ",");
  BuildButton(&Buttons[12], Col3, Row2);     //,,  "-");
  BuildButton(&Buttons[13], Col8, Row3);     //,,  ".");
  BuildButton(&Buttons[14], Col5, Row4);     //,,  "/");
  BuildButton(&Buttons[15], Col1, Row1);     //,, "0");
  BuildButton(&Buttons[16], Col2, Row1);     //,, "1");
  BuildButton(&Buttons[17], Col3, Row1);     //,,  "2");
  BuildButton(&Buttons[18], Col4, Row1);     //,,  "3");
  BuildButton(&Buttons[19], Col5, Row1);     //,,  "4");
  BuildButton(&Buttons[20], Col6, Row1);     //,,  "5");
  BuildButton(&Buttons[21], Col7, Row1);     //,,  "6");
  BuildButton(&Buttons[22], Col8, Row1);     //,,  "7");
  BuildButton(&Buttons[23], Col9, Row1);     //,,  "8");
  BuildButton(&Buttons[24], Col10, Row1);    //,,  "9");
  BuildButton(&Buttons[25], Col3, Row3);     //,,  ":");
  BuildButton(&Buttons[26], Col4, Row3);     //,,  ";");
  BuildButton(&Buttons[27], Col9, Row3);     //,,  "<");
  BuildButton(&Buttons[28], Col5, Row2);     //,,  "=");
  BuildButton(&Buttons[29], Col10, Row3);    //,,  ">");
  BuildButton(&Buttons[30], Col6, Row4);     //,,  "?");
  BuildButton(&Buttons[31], Col2, Row1);     //,,  "@");
  BuildButton(&Buttons[32], Col1, Row3);     //,,  "A");
  BuildButton(&Buttons[33], Col6, Row4);     //,,  "B");
  BuildButton(&Buttons[34], Col4, Row4);     //,,  "C");
  BuildButton(&Buttons[35], Col3, Row3);     //,,  "D");
  BuildButton(&Buttons[36], Col3, Row2);     //,,  "E");
  BuildButton(&Buttons[37], Col4, Row3);     //,,  "F");
  BuildButton(&Buttons[38], Col5, Row3);     //,,  "G");
  BuildButton(&Buttons[39], Col6, Row3);     //,,  "H");
  BuildButton(&Buttons[40], Col8, Row2);     //,,  "I");
  BuildButton(&Buttons[41], Col7, Row3);     //,,  "J");
  BuildButton(&Buttons[42], Col8, Row3);     //,,  "K");
  BuildButton(&Buttons[43], Col9, Row3);     //,,  "L");
  BuildButton(&Buttons[44], Col8, Row4);     //,,  "M");
  BuildButton(&Buttons[45], Col7, Row4);     //,,  "N");
  BuildButton(&Buttons[46], Col9, Row2);     //,,  "O");
  BuildButton(&Buttons[47], Col10, Row2);    //,,  "P");
  BuildButton(&Buttons[48], Col1, Row2);     //,,  "Q");
  BuildButton(&Buttons[49], Col4, Row2);     //,,  "R");
  BuildButton(&Buttons[50], Col2, Row3);     //,,  "S");
  BuildButton(&Buttons[51], Col5, Row2);     //,,  "T");
  BuildButton(&Buttons[52], Col7, Row2);     //,,  "U");
  BuildButton(&Buttons[53], Col5, Row4);     //,,  "V");
  BuildButton(&Buttons[54], Col2, Row2);     //,,  "W");
  BuildButton(&Buttons[55], Col3, Row4);     //,,  "X");
  BuildButton(&Buttons[56], Col6, Row2);     //,,  "Y");
  BuildButton(&Buttons[57], Col2, Row4);     //,,  "Z");
  BuildButton(&Buttons[58], Col7, Row2);     //,,  "[");
  BuildButton(&Buttons[59], Col1, Row3);     //,,  "\\");
  BuildButton(&Buttons[60], Col8, Row2);     //,,  "]");
  BuildButton(&Buttons[61], Col6, Row1);     //,,  "^");
  BuildButton(&Buttons[62], Col4, Row2);     //,,  "_");
  BuildButton(&Buttons[63], Col1, Row2);     //,,  "`");
  BuildButton(&Buttons[64], Col9, Row2);     //,,  "{");
  BuildButton(&Buttons[65], Col2, Row3);     //,,  "|");
  BuildButton(&Buttons[66], Col10, Row2);    //,,  "}");
  BuildButton(&Buttons[67], Col2, Row2);     //,,  "~");
  BuildButton(&Buttons[68], Col4, Row5, 4);  //,, 3, "Space");
  BuildButton(&Buttons[69], Col1, Row6, 3);  //,, 3, "Done");
  BuildButton(&Buttons[70], Col4, Row6, 4);  //,, 3, "Cancel");
  BuildButton(&Buttons[71], Col8, Row6, 3);  //,, 3, "Back");
  BuildButton(&Buttons[72], Col1, Row5, 3);  //,, 3, "Caps");
  BuildButton(&Buttons[73], Col8, Row5, 3);  //,, 3, "$%");

  DisplayInput();
  
	if (hasinittext){
		d->fillRect(Col1, Row0, Col10 + (2 * 9), Size, inputt);
		d->setCursor(Col1 + 5, Row0 + 6);
		d->setFont(bfont);
		d->setTextColor(inputb, inputt);
		Serial.println(inittext);
		d->print(inittext);
	}
  // draw numbers
  for (i = 15; i < 25; i++) {
    DrawButton(&Buttons[i], i + 33, BUTTON_RELEASED);
  }

  if (CapsLock) {
    // A-Z
    for (i = 32; i < 58; i++) {
      DrawButton(&Buttons[i], i + 33, BUTTON_RELEASED);
    }
  } else {
    // a-z
    for (i = 32; i < 58; i++) {
      DrawButton(&Buttons[i], i + 33 + 32, BUTTON_RELEASED);
    }
  }

  for (i = 68; i < 74; i++) {
    DrawButton(&Buttons[i], i + 100, BUTTON_RELEASED);
  }

  Serial.println("Keyboard: Enter Keepin loop");
  while (KeepIn) {

    if (t->touched()) {

      ProcessTouch();
      //go thru all the KeyboardBtn, checking if they were pressed


      if (SpecialChar) {
        found = false;
        for (b = 0; b < 15; b++) {
          if (Pressed(&Buttons[b], b + 33)) {
			  if (np >= MAX_KEYBOARD_CHARS){
				   break;
			  }
            dn[np] = (char)(b + 33);
			hc[np] = '*';
			np++;
            DisplayInput();
            found = true;
            break;
          }
        }
        for (b = 25; b < 32; b++) {
          if (Pressed(&Buttons[b], b + 33) && !found) {
			if (np >= MAX_KEYBOARD_CHARS){
			break;
			}
            dn[np] = (char)(b + 33);
			hc[np] = '*';
			np++;
            DisplayInput();
            found = true;
            break;
          }
        }
        for (b = 58; b < 66; b++) {
          if (Pressed(&Buttons[b], b + 33) && !found) {
			if (np >= MAX_KEYBOARD_CHARS){
			break;
			}
            dn[np] = (char)(b + 33);
			hc[np] = '*';
			np++;
            DisplayInput();
            found = true;
            break;
          }
        }
        for (b = 58; b < 64; b++) {
          if (Pressed(&Buttons[b], b + 33) && !found) {
				if (np >= MAX_KEYBOARD_CHARS){
				break;
				}
            dn[np] = (char)(b + 33);
			hc[np] = '*';
			np++;
            DisplayInput();
            found = true;
            break;
          }
        }
        for (b = 64; b < 68; b++) {
          if (Pressed(&Buttons[b], b + 33 + 26) && !found) {
			if (np >= MAX_KEYBOARD_CHARS){
			break;
			}
            dn[np] = (char)(b + 33 + 26);
			hc[np] = '*';
			np++;
            DisplayInput();
            found = true;
            break;
          }
        }
      } else {
        found = false;
        // check numbers
        for (b = 15; b < 25; b++) {
			if (np >= MAX_KEYBOARD_CHARS){
			break;
			}
          if (Pressed(&Buttons[b], b + 33) && !found) {
            dn[np] = (char)(b + 33);
			hc[np] = '*';
			np++;
            DisplayInput();
            found = true;
            break;
          }
        }
        // check letters
        for (b = 32; b < 58; b++) {
			if (np >= MAX_KEYBOARD_CHARS){
			   break;
			}
          if (CapsLock) {
            if (Pressed(&Buttons[b], b + 33) && !found) {
              dn[np] = (char)(b + 33);
			  hc[np] = '*';
			  np++;
              DisplayInput();
              found = true;
              break;
            }
          } else {
            if (Pressed(&Buttons[b], b + 33 + 32) && !found) {
              dn[np] = (char)(b + 33 + 32);
			  hc[np] = '*';
			  np++;
              DisplayInput();
              found = true;
              break;
            }
          }
        }
      }

      for (b = 68; b < 74; b++) {
        if (Pressed(&Buttons[b], b + 100)) {
          break;
        }
      }

      // caps lock
      if (b == 72) {
        CapsLock = !CapsLock;
        SpecialChar = false;
        SpecialChar = false;
        for (i = 15; i < 25; i++) {
          DrawButton(&Buttons[i], i + 33, BUTTON_RELEASED);
        }
        if (CapsLock) {
          for (i = 32; i < 58; i++) {
            DrawButton(&Buttons[i], i + 33, BUTTON_RELEASED);
          }
        } else {
          for (i = 32; i < 58; i++) {
            DrawButton(&Buttons[i], i + 33 + 32, BUTTON_RELEASED);
          }
        }
      }
      // special characters
      if (b == 73) {
        SpecialChar = !SpecialChar;
        d->fillRect(Col1, Row1, Col10 + Size, 4 * (Size + 2), kcolor);
        if (SpecialChar) {
          for (i = 0; i < 15; i++) {
            DrawButton(&Buttons[i], i + 33, BUTTON_RELEASED);
          }
          for (i = 25; i < 32; i++) {
            DrawButton(&Buttons[i], i + 33, BUTTON_RELEASED);
          }
          for (i = 58; i < 66; i++) {
            DrawButton(&Buttons[i], i + 33, BUTTON_RELEASED);
          }
          for (i = 58; i < 64; i++) {
            DrawButton(&Buttons[i], i + 33, BUTTON_RELEASED);
          }
          for (i = 64; i < 68; i++) {
            DrawButton(&Buttons[i], i + 33 + 26, BUTTON_RELEASED);
          }
        } else {
          for (i = 15; i < 25; i++) {
            DrawButton(&Buttons[i], i + 33, BUTTON_RELEASED);
          }
          if (CapsLock) {
            for (i = 32; i < 58; i++) {
              DrawButton(&Buttons[i], i + 33, BUTTON_RELEASED);
            }
          } else {
            for (i = 32; i < 58; i++) {
              DrawButton(&Buttons[i], i + 33 + 32, BUTTON_RELEASED);
            }
          }
        }
      }


      if (b == 68) {
			if (np < MAX_KEYBOARD_CHARS){
				// space
				dn[np] = ' ';
				hc[np] = '*';
				np++;
				DisplayInput();
			}
      }
      else if (b == 71) {
        // back space
        if (np > 0) {			 
          --np;
          dn[np] = ' ';
		 hc[np] = ' ';
        }
        DisplayInput();
      }
      else if (b == 69) {
        // done
        dn[np] = '\0';
		
        strcpy(data, dn);

        KeepIn = false;
        break;
      }
      else if (b == 70) {
        // cancel, just get the heck out
        KeepIn = false;
        break;
      }

      delay(10);
    }
  }

}

void Keyboard::setInitialText(const char *Text) {

  uint8_t i;

  for (i = 0; i < (MAX_KEYBOARD_CHARS); i++) {
    inittext[i] = Text[i];
  }
  hasinittext = true;
Serial.println(inittext);
  
}

void Keyboard::setClickPin(int Value) {

clickpin = Value;

}

void Keyboard::ProcessTouch() {
//	  screenX0 = ScreenLeft;
//  screenX320 = ScreenRight;
//  screenY0 = ScreenTop;
//  screenY240 = ScreenBottom;

#if defined(FT)
	if (t->touched()){	
		p = t->getPoint();
		BtnX = p.y;
		BtnY = p.x;
		
	#ifdef debug
		 Serial.print(" real coordinates:");
		 Serial.print(BtnX);
		 Serial.print(" ,");
		 Serial.print (BtnY);
	#endif

		BtnX = map(BtnX, screenX0, screenX320, 0, 479);
		BtnY = map(BtnY, screenY0, screenY240, 0, 319);
		
		//BtnX = constrain(BtnX, 0, 479);
		//BtnY = constrain(BtnY, 0, 319);
	#ifdef debug
		 Serial.print(" , Mapped coordinates:");
		 Serial.print(BtnX);
		 Serial.print(" ,");
		 Serial.println(BtnY);
		 d->fillCircle(BtnX, BtnY, 2, ST7735_RED);
	#endif
	}
#endif	//End FT6236 Touch

#if defined(XPT)

		if (t->touched()){	
			p = t->getPoint();
			BtnX = p.x;
			BtnY = p.y;
			
	#ifdef debug
		 Serial.print(" real coordinates:");
		 Serial.print(BtnX);
		 Serial.print(" ,");
		 Serial.print (BtnY);
	#endif
			BtnX = map(BtnX, screenX0, screenX320, 0, 479);
			BtnY = map(BtnY, screenY0, screenY240, 0, 319);
			
			BtnX = constrain(BtnX, 0, 479);
			BtnY = constrain(BtnY, 0, 319);
		
	#ifdef debug
		 Serial.print(" , Mapped coordinates:");
		 Serial.print(BtnX);
		 Serial.print(" ,");
		 Serial.println(BtnY);
		 d->fillCircle(BtnX, BtnY, 2, ST7735_RED);
	#endif
		}

#endif
}
