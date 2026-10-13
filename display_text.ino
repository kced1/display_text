
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// ===== ST7735S pin configuration =====
constexpr int TFT_SCLK = 18;
constexpr int TFT_MOSI = 23;
constexpr int TFT_CS   = 27;
constexpr int TFT_DC   = 25;  // A0 on the display
constexpr int TFT_RST  = 26;

Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

String message;
int cursorY = 38;

void drawHeader() {
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextWrap(false);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(5, 5);
  tft.println("ESP32 DISPLAY TEST");

  tft.drawFastHLine(0, 19, tft.width(), ST77XX_BLUE);

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(5, 25);
  tft.println("Serial input:");

  cursorY = 38;
}

void showMessage(String text) {
  // Start a fresh page if there is no more vertical space.
  if (cursorY + 8 > tft.height()) {
    drawHeader();
    tft.setCursor(5, cursorY);
    tft.println("Previous messages cleared");
    cursorY += 12;
  }

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(5, cursorY);
  tft.println(text);

  cursorY += 12;
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1000);

  // Explicit SPI clock and MOSI pins for the ESP32.
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  // Common ST7735 initialization.
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  drawHeader();

  Serial.println();
  Serial.println("=== ESP32 + ST7735S ===");
  Serial.println("Display initialization completed.");
  Serial.println("Type a message and press Enter:");
}

void loop() {
  if (Serial.available() > 0) {
    message = Serial.readStringUntil('\n');
    message.trim();

    if (message.length() > 0) {
      Serial.print("Received: ");
      Serial.println(message);

      showMessage(message);

      Serial.println("Enter another message:");
    }
  }
}
