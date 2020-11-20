#include <Adafruit_NeoPixel.h>
#include "SnakeLights.h"

#if defined(ESP32)
#define PIN 21
#else
#define PIN 4
#endif

#define NUM_LIGHTS  256

Adafruit_NeoPixel _strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800);

void SnakeLights::init()
{
  _strip.begin();
  _strip.show();

  // Top left set to red while booting.
  _strip.setPixelColor(0, Adafruit_NeoPixel::Color(255, 0, 0));  
  for (auto i = 1; i < NUM_LIGHTS; i++) { _strip.setPixelColor(i, Adafruit_NeoPixel::Color(0, 0, 0)); }
}

void SnakeLights::device_ready_light()
{
  _strip.setPixelColor(0, Adafruit_NeoPixel::Color(0, 255, 0));  
  for (auto i = 1; i < NUM_LIGHTS; i++) { _strip.setPixelColor(i, Adafruit_NeoPixel::Color(0, 0, 0)); }
}

int SnakeLights::adjust_for_snake_mode(int pixel) 
{
  auto currentRowNumber = pixel / 16;
  auto shouldSnake = currentRowNumber % 2 == 0;
  if (!shouldSnake) 
  {
    return pixel;
  }
  
  /*
      Snaking involves working out what position on a row the number is in
      and then counting from the end of the row.
  
      We can work out our regular row position by taking the remainder when
      we divide our pixelId by the number of pixels on a row:
      
          positionOnRowWhenNotSnaked = pixelId % (modulo) 16      
  
      Because each of our rows contains 16 elements (from 0 to 15)
      a snaked-row-position is the position of a pixel on the current row
      counted in from the right hand side, instead of from the left.
  
      First row is snaked, second is normal:
      <--------------<------------------<------------
      15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
      --------------->------------------>------------
      00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15
  
      So a snaked-row-position is:
  
          15 - position when not snaked
  
      If a pixels row position is 3, it's snaked-row-position would be 12
      because 12, is three elements from the right hand side.
  
          15 - 3 = 12
      
      We then have to add the total number of pixels from all the previous rows
      to our snaked-row-position to work out the final snakePixelId.
  */    
  
  auto positionOnRowWhenNotSnaked = (pixel % 16);
  auto positionOnRowWhenSnaked = (15 - positionOnRowWhenNotSnaked);    
  auto snakePixelId = positionOnRowWhenSnaked + (currentRowNumber * 16);
  return snakePixelId;
}

void SnakeLights::log(int pixel, int r, int g, int b)
{
  Serial.println(F("Updating lights."));
  Serial.print(pixel);
  Serial.print(F(":("));
  Serial.print(r);
  Serial.print(F(","));
  Serial.print(g);
  Serial.print(F(","));
  Serial.print(b);
  Serial.print(F(")\r\n"));
}

void SnakeLights::update_pixel(int pixel, int r, int g, int b)
{  
  auto snakePixelId = adjust_for_snake_mode(pixel);

  log(pixel, r, g, b);
  _strip.setPixelColor(snakePixelId, r, g, b);
  _strip.show();

  Serial.println(F("Lights set"));
}

String SnakeLights::hex_for_position(const String& palette, const int palette_ref)
{
  const auto palette_offset = (6 * palette_ref) + palette_ref;
  const auto substr = palette.substring(palette_offset, palette_offset + 6);
  return substr;
}

// Supports Run Length Encoding
void SnakeLights::update_all_lights(const String& palette, const String& pixels, boolean preSnaked)
{
  Serial.println(F("update_all_lights"));
  
  auto pixel_start = pixels.indexOf(',') + 1;
  auto pixel_number = 0;
  bool at_end;

  do
  {
    const auto pixel_end = pixels.indexOf(',', pixel_start);
    at_end = pixel_end == -1;

    const auto pixel_data = at_end
      ? pixels.substring(pixel_start)
      : pixels.substring(pixel_start, pixel_end);

    const auto index_of_multiplication_symbol = pixel_data.indexOf('x');

    const auto palette_ref = index_of_multiplication_symbol == -1
      ? pixel_data.toInt()
      : pixel_data.substring(0, index_of_multiplication_symbol).toInt();

    const auto times = index_of_multiplication_symbol == -1
      ? 1
      : pixel_data.substring(index_of_multiplication_symbol + 1).toInt();

    pixel_start = pixel_end + 1;

    char hex_code[6];
    int r, g, b = 0;
    
    hex_for_position(palette, palette_ref).toCharArray(hex_code, 6);
    sscanf(hex_code, "%02x%02x%02x", &r, &g, &b);

    for (auto i = 0; i < times; i++)
    {
      if (preSnaked) 
      {        
        log(pixel_number, r, g, b);    
        _strip.setPixelColor(pixel_number, r, g, b);
      } 
      else 
      {
        SnakeLights::update_pixel(pixel_number, r, g, b);
      }

      pixel_number++;
    }

  } while (!at_end);

  _strip.show();

  Serial.println(F("Lights set"));
}
