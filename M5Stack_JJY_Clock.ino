#include <M5Stack.h>
#include "SPIFFS.h"
#include "FS.h"
#include "arduinoFFT.h" // https://github.com/kosme/arduinoFFT

#define SIGNAL_LENGTH 512
#define M5STACKFIRE_MICROPHONE_PIN 34
#define M5STACKFIRE_SPEAKER_PIN 25 // speaker DAC, only 8 Bit
#define SAMPLINGFREQUENCY 40000
#define SAMPLING_TIME_US (1000000UL/SAMPLINGFREQUENCY)
#define ANALOG_SIGNAL_INPUT M5STACKFIRE_MICROPHONE_PIN

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

double adcBuffer[SIGNAL_LENGTH];
double vImag[SIGNAL_LENGTH];
double AdcMeanValue = 0;

int H, M, S;

int fft() {
  int n;
  uint32_t nextTime = 0;
  for (n = 1; n < SIGNAL_LENGTH; n++) {
    double v = analogRead( ANALOG_SIGNAL_INPUT );
    AdcMeanValue += (v - AdcMeanValue) * 0.001;
    adcBuffer[n] = v - AdcMeanValue;
    
    while (micros() < nextTime);
    nextTime = micros() + SAMPLING_TIME_US;
  }

  FFT.Windowing(adcBuffer, SIGNAL_LENGTH, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(adcBuffer, vImag, SIGNAL_LENGTH, FFT_FORWARD);
  FFT.ComplexToMagnitude(adcBuffer, vImag, SIGNAL_LENGTH);
  int x = FFT.MajorPeak(adcBuffer, SIGNAL_LENGTH, SAMPLINGFREQUENCY);

  int maxAmplitudeDB = 0;
  for (n = 1; n < SIGNAL_LENGTH; n++) {
    int a = log10(adcBuffer[n]) * 20 - 54.186;
    if (a > maxAmplitudeDB) maxAmplitudeDB = a;
    adcBuffer[n] = (a + 30) * 5;
    vImag[n] = 0;
  }

  //a = maxAmplitudeDB;
  return x;
}

void jjy() {
  unsigned long t  = 0;
  unsigned long t1;
  int f;
  char pre = ' ';
  bool T[17];
  int Tb = 0;
  bool r = false;
  while (true) {
    t1 = millis();
    f = fft();
    if (f > 12000) {
      t += millis() - t1;
    } else {
      if (t) {
        if ((t > 100) && (t < 250)) {
          if (pre == 'M') {
            r = true;
          }
          pre = 'M';
          M5.Lcd.setTextColor(TFT_RED);
        } else if ((t > 400) && (t < 600)) {
          if (r) {
            T[Tb] = true;
            Tb++;
          }
          pre = '1';
          M5.Lcd.setTextColor(TFT_YELLOW);
        } else if ((t > 700) && (t < 900)) {
          if (r) {
            T[Tb] = false;
            Tb++;
          }
          pre = '0';          
          M5.Lcd.setTextColor(TFT_GREEN);
        }
        if (Tb > 16) {
          Tb = 0;
          r = false;
          M = 0;
          H = 0;
          S = 0;
          M = (T[0]) ? M + 40 : M;
          M = (T[1]) ? M + 20 : M;
          M = (T[2]) ? M + 10 : M;
          M = (T[4]) ? M + 8 : M;
          M = (T[5]) ? M + 4 : M;
          M = (T[6]) ? M + 2 : M;
          M = (T[7]) ? M + 1 : M;
          H = (T[10]) ? H + 20 : H;
          H = (T[11]) ? H + 10 : H;
          H = (T[13]) ? H + 8 : H;
          H = (T[14]) ? H + 4 : H;
          H = (T[15]) ? H + 2 : H;
          H = (T[16]) ? H + 1 : H;
          S = 19;
          //M5.Lcd.setTextColor(TFT_WHITE);
          //M5.Lcd.print(H);M5.Lcd.print(":");M5.Lcd.print(M);
        }
        //M5.Lcd.print("* ");
      }
      t = 0;
    }
  }
}

void Clock(int bgColor, int X, int Y, int R, int H, int M, int S) {
  M5.Lcd.drawCircle(X, Y, R, TFT_WHITE);
  M5.Lcd.drawCircle(X, Y, R - 1, TFT_WHITE);
  M5.Lcd.fillCircle(X, Y, R - 2, bgColor);

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

void watch(void * pvParameters) {
  unsigned long previousMillis = 0;
  while (true) {
    unsigned long d = millis() - previousMillis;
    if (d >= 1000) {
      S += d / 1000;
      M = (S > 59) ? M + 1 : M;
      H = (M > 59) ? H + 1 : H;
      S = (S > 59) ? 0 : S;
      M = (M > 59) ? 0 : M;
      H = (H > 24) ? 0 : H;
      Clock(16716, 285, 55, 26, H, M, S);
      previousMillis = millis();
    }
    delay(5);
  }
}
  

void setup(void) {
  M5.begin();
  SPIFFS.begin(true);
  dacWrite(M5STACKFIRE_SPEAKER_PIN, 0);
  M5.Lcd.fillScreen(16716);
  xTaskCreatePinnedToCore(watch, "watch", 4096, NULL, 1, NULL, 1);
  M5.Lcd.drawJpgFile(SPIFFS, "/wallpapers.jpg", 0, 0);
}

void loop() {
  jjy();
}
