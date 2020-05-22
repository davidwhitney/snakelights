# Arduino SnakeLights

This library drives the Snake-LED-Matrix.

Download the latest version from the [release](https://github.com/davidwhitney/snakelights/releases) section. 
Or use the builtin Library Manager in the Arduino IDE and search for "snakelights".

# SNAKE MODE

Snake mode is what happens when you optimise for soldering 256 pixels the (moderately, slightly, a little bit more practical) easy way instead of the long boring way.

Our pixels have an odd numbering scheme - the Pixel IDs of the addressable pixels look like this:

    <------------------------<------------------------<------------
    015 014 013 012 011 010 009 008 007 006 005 004 003 002 001 000
    ------------------------->------------------------>------------
    016 017 018 019 020 021 022 023 024 025 026 027 028 029 030 031
    <------------------------<------------------------<------------
    047 046 045 044 043 042 041 040 039 038 037 036 035 034 033 032
    ------------------------->------------------------>------------
    ................

Because of this somewhat unorthadox numbering scheme, the code running on our hardware translates regular, real people pixel Ids, into **GLORIOUS SNAKE IDS** transparently. This means that in our web app, we can address our pixels using the much more reasonable pixel Ids of

    000 001 002 003 004 005 006 007 008 009 010 011 012 013 014 015
    ------------------------->------------------------>------------
    016 017 018 019 020 021 022 023 024 025 026 027 028 029 030 031
    ------------------------->------------------------>------------
    ................


We love snake mode 🐍, it makes soldiering easier. And snakes are awesome. Shut up.

## Compatibility

You need to build your own SnakeLights out of NeoPixels for this library to work.

There's a walkthrough [here](https://medium.com/@jofranchetti/have-yourself-a-matrix-led-christmas-f25cfedede74)


## Example


```c++
#include "SnakeLights.h"

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

```
