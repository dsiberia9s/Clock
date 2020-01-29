#include <M5Stack.h>

uint8_t conv2d(const char * p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

uint8_t H = conv2d(__TIME__), M = conv2d(__TIME__+3), S = conv2d(__TIME__+6);

void Clock(int bgColor, int X, int Y, int R, int H, int M, int S) {
  M5.Lcd.drawCircle(X, Y, R, TFT_WHITE);
  M5.Lcd.drawCircle(X, Y, R - 1, TFT_WHITE);
  M5.Lcd.drawCircle(X, Y, R - 2, TFT_WHITE);

  H = (H < 0) ? 0 : H;
  H = (H > 12) ? H - 12 : H;
  H = (H > 12) ? 0 : H;

  M = (M < 0) ? 0 : M;
  M = (M > 59) ? 0 : M;

  S = (S < 0) ? 0 : S;
  S = (S > 59) ? 0 : S;

  float H_sdeg, M_sdeg, S_sdeg;
  float H_sx, M_sx, S_sx;
  float H_sy, M_sy, S_sy;
  float H_osx, M_osx, S_osx;
  float H_osy, M_osy, S_osy;

  H_sdeg = ((H - 1 < 0) ? 11 : H - 1) * 6 * 5;
  H_sx = cos((H_sdeg - 90) * 0.0174532925);
  H_sy = sin((H_sdeg - 90) * 0.0174532925);
  H_osx = (H_sx * (R * 0.5 - 5)) + X;
  H_osy = (H_sy * (R * 0.5 - 5)) + Y;
  M5.Lcd.drawLine(H_osx, H_osy, X, Y, bgColor);

  M_sdeg = ((M - 1 < 0) ? 59 : M - 1) * 6;
  M_sx = cos((M_sdeg - 90) * 0.0174532925);
  M_sy = sin((M_sdeg - 90) * 0.0174532925);
  M_osx = (M_sx * (R * 0.8 - 5)) + X;
  M_osy = (M_sy * (R * 0.8 - 5)) + Y;
  M5.Lcd.drawLine(M_osx, M_osy, X, Y, bgColor);

  S_sdeg = ((S - 1 < 0) ? 59 : S - 1) * 6;
  S_sx = cos((S_sdeg - 90) * 0.0174532925);
  S_sy = sin((S_sdeg - 90) * 0.0174532925);
  S_osx = (S_sx * (R - 5)) + X;
  S_osy = (S_sy * (R - 5)) + Y;
  M5.Lcd.drawLine(S_osx, S_osy, X, Y, bgColor);

  H_sdeg = H * 6 * 5;
  H_sx = cos((H_sdeg - 90) * 0.0174532925);
  H_sy = sin((H_sdeg - 90) * 0.0174532925);
  H_osx = (H_sx * (R * 0.5 - 5)) + X;
  H_osy = (H_sy * (R * 0.5 - 5)) + Y;
  M5.Lcd.drawLine(H_osx, H_osy, X, Y, TFT_WHITE);

  M_sdeg = M * 6;
  M_sx = cos((M_sdeg - 90) * 0.0174532925);
  M_sy = sin((M_sdeg - 90) * 0.0174532925);
  M_osx = (M_sx * (R * 0.8 - 5)) + X;
  M_osy = (M_sy * (R * 0.8 - 5)) + Y;
  M5.Lcd.drawLine(M_osx, M_osy, X, Y, TFT_WHITE);

  S_sdeg = S * 6;
  S_sx = cos((S_sdeg - 90) * 0.0174532925);
  S_sy = sin((S_sdeg - 90) * 0.0174532925);
  S_osx = (S_sx * (R - 5)) + X;
  S_osy = (S_sy * (R - 5)) + Y;
  M5.Lcd.drawLine(S_osx, S_osy, X, Y, TFT_RED);
}

void setup(void) {
  M5.begin();
  SPIFFS.begin(true);
  M5.Lcd.fillScreen(16716);
}

unsigned long previousMillis = 0;

void loop() {
  if (millis() - previousMillis >= 1000) {
    S++;
    M = (S > 59) ? M + 1 : M;
    H = (M > 59) ? H + 1 : H;
    S = (S > 59) ? 0 : S;
    M = (M > 59) ? 0 : M;
    H = (H > 24) ? 0 : H;
    Clock(16716, 285, 55, 26, H, M, S);
    previousMillis = millis();
  }
}
