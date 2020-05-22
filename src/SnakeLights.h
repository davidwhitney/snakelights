#include <Adafruit_NeoPixel.h>
#ifndef _SNAKELIGHTS_h
#define _SNAKELIGHTS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class SnakeLights
{  
public:
  void init();
  void device_ready_light();
  void update_pixel(int pixel, int r, int g, int b);
  void update_all_lights(const String& palette, const String& pixels, boolean preSnaked);

private:
  String hex_for_position(const String& palette, const int palette_ref);
  int adjust_for_snake_mode(int pixel);
  void log(int pixel, int r, int g, int b);
};

#endif
