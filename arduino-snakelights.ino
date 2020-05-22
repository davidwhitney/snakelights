#include "src/SnakeLights.h"

SnakeLights snakeLights;

void setup() 
{
  Serial.begin(115200);
  Serial.println(F("setup"));

  snakeLights.init();
  // Connect to wifi, etc
  snakeLights.device_ready_light();
}

void loop() 
{  
  Serial.println(F("loop"));

  auto pixel = random(0, 255);
  auto r = random(0, 255);
  auto g = random(0, 255);
  auto b = random(0, 255);
  
  snakeLights.update_pixel(pixel, r, g, b);
  delay(1000);
}
