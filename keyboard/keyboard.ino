/*
 * Based on Aurélien Rodot's Gamebuino <https://github.com/Rodot/Gamebuino/blob/master/Gamebuino.h>
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define LCD_DC  5
#define LCD_CS  7
#define LCD_RST 3

#define BTN_UP  2
#define BTN_LEFT  3
#define BTN_RIGHT  4
#define BTN_DOWN  8
#define BTN_ACCEPT  6

#define KEYBOARD_COLS  16
#define KEYBOARD_ROWS  8

#define FONT_WIDTH  5
#define FONT_HEIGHT 7

Adafruit_PCD8544 display = Adafruit_PCD8544(LCD_DC, LCD_CS, LCD_RST);

void setup() {
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN , INPUT_PULLUP);
  pinMode(BTN_ACCEPT, INPUT_PULLUP);  
  
  display.begin();
  display.setContrast(60);
  display.setTextSize(1);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.display();

  showKeyboard();
}

bool isButtonDown(int pin) {
  if (digitalRead(pin) == LOW) {
    delay(30);

    if (digitalRead(pin) == LOW) {
      return true;
    }

    return false;
  }

  return false;
}

void showKeyboard() {
  int activeX = 0;
  int activeY = 2;

  int currentX = 0;
  int currentY = 0;

  int targetX = 0;
  int targetY = 0;

  String str = "";

  while (true) {
    display.clearDisplay();

    for (int8_t y = 0; y < KEYBOARD_ROWS; y++) {
      for (int8_t x = 0; x < KEYBOARD_COLS; x++) {
        display.drawChar(
          currentX + x * (FONT_WIDTH + 2),
          currentY + y * (FONT_HEIGHT + 2),
          (char)(x + y * KEYBOARD_COLS),
          BLACK,
          WHITE,
          1
        );
      }
    }

    display.drawRoundRect(
      currentX + activeX * (FONT_WIDTH + 2) - 1,
      currentY + activeY * (FONT_HEIGHT + 2) - 2,
      (FONT_WIDTH + 2) + (FONT_WIDTH - 1) % 2,
      (FONT_HEIGHT + 4),
      3,
      BLACK
    );

    display.fillRect(0, 37, display.width(), 11, WHITE);
    
    display.setCursor(0, 39);
    display.print(str);

    display.display();

    if (isButtonDown(BTN_RIGHT)) {
      activeX++;
      delay(100);
    }
    
    else if (isButtonDown(BTN_LEFT)) {
      activeX--;
      delay(100);
    }
    
    else if (isButtonDown(BTN_DOWN)) {
      activeY++;
      delay(100);
    }
    
    else if (isButtonDown(BTN_UP)) {
      activeY--;
      delay(100);
    }
    
    else if (isButtonDown(BTN_ACCEPT)) {
      str += (char)(activeX + KEYBOARD_COLS * activeY);
      delay(100);
    }

    if (activeX == KEYBOARD_COLS) {
      activeX = 0;
    }
    
    else if (activeX < 0) {
      activeX = KEYBOARD_COLS - 1;
    }

    else if (activeY == KEYBOARD_ROWS) {
      activeY = 0;
    }

    else if (activeY < 0) {
      activeY = KEYBOARD_ROWS - 1;
    }

    targetX = -(FONT_WIDTH + 1) * activeX + LCDWIDTH / 2 - 3;
    targetY = -(FONT_HEIGHT + 1) * activeY + LCDHEIGHT / 2 - 4 - FONT_HEIGHT;

    currentX = (targetX + currentX) / 2;
    currentY = (targetY + currentY) / 2;
  }
}

void loop() {
}

