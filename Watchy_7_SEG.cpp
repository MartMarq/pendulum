#include "Watchy_7_SEG.h"


void Watchy7SEG::drawWatchFace() {
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);

  drawTime();

  // reset step counter at midnight
  if (currentTime.Hour == 00 && currentTime.Minute == 00) {
    sensor.resetStepCounter();
    //turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
  }

  //time adjustment start
  //if you don't need a time adjustment, just delete all lines between time adjustment start and end
  //my watchy runs too fast (15 seconds and 500 ms a day), this code set back time each day by 15 seconds and 500 ms

  if (currentTime.Hour == 00 && currentTime.Minute == 30) {

    RTC.read(currentTime);
    int8_t sekunde = currentTime.Second;
    int8_t minute = currentTime.Minute;
    int8_t hour = currentTime.Hour;
    int8_t day = currentTime.Day;
    int8_t month = currentTime.Month;
    int8_t year = tmYearToY2k(currentTime.Year);

    delay(15500);

    tmElements_t tm;
    tm.Month = month;
    tm.Day = day;
    tm.Year = y2kYearToTm(year);
    tm.Hour = hour;
    tm.Minute = minute;
    tm.Second = sekunde;

    RTC.set(tm);
  }

  // time adjustment end
}


void Watchy7SEG::drawTime() {
  int8_t stundeA = currentTime.Hour;
  int minuteA = currentTime.Minute;
  int s1 = sensor.getCounter();
  int tagA = currentTime.Day;
  int monatA = currentTime.Month;
  int n, hour;
  int16_t xc, yc, x, y;
  uint16_t wc, hc;
  float radi = 0.0174532925;
  int hx = 220;
  display.setFont(&FreeSansBold9pt7b);
  float VBAT = getBatteryVoltage();
  uint8_t Protz = min<int>(100 - ((4.16 - VBAT) * 100), 100);

  display.drawCircle(100, 45, 150, GxEPD_BLACK);
  display.fillCircle(100, 45, 5, GxEPD_BLACK);


  if (minuteA <= 30) {
    for (n = 0; n <= 30; n = n + 5) {
      x = 100 + 150 * sin((n * 2.5 - 219) * radi);
      y = 45 - 150 * cos((n * 2.5 - 219) * radi);
      display.getTextBounds(String(n), 100, 100, &xc, &yc, &wc, &hc);
      display.setCursor(x - wc / 2, y - hc / 2);
      display.println(n);
    }
  }

  if (minuteA > 30) {
    for (n = 0; n <= 30; n = n + 5) {
      x = 100 + 150 * sin((n * 2.5 - 219) * radi);
      y = 45 - 150 * cos((n * 2.5 - 219) * radi);
      hx = (n - 60) * -1;
      if (hx == 60) {
        hx = 0;
      }
      display.getTextBounds(String(hx), 100, 100, &xc, &yc, &wc, &hc);
      display.setCursor(x - wc / 2, y - hc / 2);
      display.println(hx);
    }
  }
  if (minuteA > 30) {
    hx = (minuteA - 60) * -1;
  } else {
    hx = minuteA;
  }
  x = 100 + 105 * sin((hx * 2.5 - 219) * radi);
  y = 45 - 105 * cos((hx * 2.5 - 219) * radi);
  display.drawLine(100, 45, x, y, GxEPD_BLACK);
  display.fillCircle(x, y, 15, GxEPD_BLACK);
  int x3 = 100 + 128 * sin((hx * 2.5 - 219) * radi);
  int y3 = 45 - 128 * cos((hx * 2.5 - 219) * radi);
  int x1 = 100 + 115 * sin(((hx - 2) * 2.5 - 219) * radi);
  int y1 = 45 - 115 * cos(((hx - 2) * 2.5 - 219) * radi);
  int x2 = 100 + 115 * sin(((hx + 2) * 2.5 - 219) * radi);
  int y2 = 45 - 115 * cos(((hx + 2) * 2.5 - 219) * radi);
  display.fillTriangle(x3, y3, x1, y1, x2, y2, GxEPD_BLACK);
  hx = stundeA;
  display.setTextColor(GxEPD_WHITE);
  display.getTextBounds(String(hx), 100, 100, &xc, &yc, &wc, &hc);
  display.setCursor(x - wc / 2, y + 12 - hc / 2);
  display.println(hx);
  display.setTextColor(GxEPD_BLACK);



  hx = tagA;
  display.getTextBounds(String(hx), 100, 100, &xc, &yc, &wc, &hc);
  display.setCursor(30 - wc / 2, 42 - hc / 2);
  display.println(hx);
  display.drawCircle(30, 30, 15, GxEPD_BLACK);

  hx = monatA;
  display.getTextBounds(String(hx), 100, 100, &xc, &yc, &wc, &hc);
  display.setCursor(170 - wc / 2, 42 - hc / 2);
  display.println(hx);
  display.drawCircle(170, 30, 15, GxEPD_BLACK);



  hx = Protz;
  if (minuteA >= 15) {
    x = 55;
  } else {
    x = 145;
  }

  if (minuteA >= 45) {
    x = 145;
  }
  y = 100;
  display.getTextBounds(String(hx), 100, 100, &xc, &yc, &wc, &hc);
  display.setCursor(x - wc / 2, 112 - hc / 2);
  display.println(hx);
  display.drawCircle(x, y, 15, GxEPD_BLACK);


  int UteZ = currentTime.Wday;
  if (minuteA >= 15) {
    x = 110;
  } else {
    x = 20;
  }

  if (minuteA >= 45) {
    x = 20;
  }
  y = 70;
  if (UteZ == 1) {
    display.drawBitmap(x, y, son, 70, 60, GxEPD_BLACK);
  }

  if (UteZ == 2) {
    display.drawBitmap(x, y, mon, 70, 60, GxEPD_BLACK);
  }

  if (UteZ == 3) {
    display.drawBitmap(x, y, die, 70, 60, GxEPD_BLACK);
  }

  if (UteZ == 4) {
    display.drawBitmap(x, y, mit, 70, 60, GxEPD_BLACK);
  }

  if (UteZ == 5) {
    display.drawBitmap(x, y, don, 70, 60, GxEPD_BLACK);
  }

  if (UteZ == 6) {
    display.drawBitmap(x, y, fre, 70, 60, GxEPD_BLACK);
  }

  if (UteZ == 7) {
    display.drawBitmap(x, y, sam, 70, 60, GxEPD_BLACK);
  }
}