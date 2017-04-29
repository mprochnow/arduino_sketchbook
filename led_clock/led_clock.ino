#include <DS3232RTC.h>        //http://github.com/JChristensen/DS3232RTC
#include <Streaming.h>        //http://arduiniana.org/libraries/streaming/
#include <Time.h>             //http://playground.arduino.cc/Code/Time
#include <Wire.h>             //http://arduino.cc/en/Reference/Wire
#include <Adafruit_NeoPixel.h>

#define NUMPIXELS          60
#define LED_PIN            2

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  setSyncProvider(RTC.get);
  
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {
  static time_t time_last;
  time_t time_now = now();
  
  if (time_now != time_last)
  {
    time_last = time_now;
    
    pixels.clear();
    pixels.setBrightness(127);
    
    showHour(time_now);
    showMinute(time_now);
    showSecond(time_now);
    
    pixels.show();
  }
}

void showHour(const time_t t)
{
  int h = hour(t);
  int p = h ? ((h % 12) * 5 - 1) : 0;

  for (int i = p - 2; i < p + 3; ++i)
  {
    pixels.setPixelColor((i < 0 ) ? 60 + i : ((i > 59) ? i - 60: i), pixels.Color(255, 0, 0));
  }
}

void showMinute(const time_t t)
{
  int m = minute(t);
  
  for (int i = m - 1; i < m + 2; ++i)
  {
    pixels.setPixelColor((i < 0 ) ? 60 + i : ((i > 59) ? i - 60: i), pixels.Color(255, 255, 0));
  }
}

void showSecond(const time_t t)
{
  int s = second(t);
  
  pixels.setPixelColor(s, pixels.Color(0, 0, 255));
}
