/*  Heliades.ino - Heliades Pattern
    Inspired by Helios, Disorient 2014, by Leo Villareal.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Required Connections
--------------------
pin 2:  LED Strip #0    OctoWS2811 drives 8 LED Strips.
pin 14: LED strip #1    All 8 are the same length.
pin 7:  LED strip #2
*/

#include <OctoWS2811.h>

// Combines red, green, blue into a single value
#define rgb(R, G, B)  ((((uint32_t)(R)) << 16) | (((uint32_t)(G)) << 8) | ((uint32_t)(B)))

const int ledsPerStrip = 456;
const int nStrips = 3;
const int nFaces = 8;
const int nLeds = ledsPerStrip * nStrips;
const int ledsPerBeam = ledsPerStrip / 4;
const int ledsPerFace = 3 * ledsPerBeam;
const int frameDelay = 1000 / 60;

// Setup OctoWS2811 library
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// Heliades
bool heliades[ledsPerStrip];
uint32_t forwardColor = rgb(255, 255, 255);
uint32_t reverseColor = rgb(255, 255, 255);
int offset = 0;

void setup() {
  leds.begin();
  createHeliadesBuffer();
}

void loop() {
  clear();

  for (int i = 0; i < ledsPerStrip; i++) {
    int thisOffset = (i + offset) % ledsPerStrip;
    if (heliades[thisOffset]) {
      leds.setPixel(i, forwardColor);
      leds.setPixel(i + ledsPerStrip, forwardColor);
      leds.setPixel(i + 2 *ledsPerStrip, forwardColor);

      // Opposite direction
      int r = ledsPerStrip - i - 1;
      leds.setPixel(r, reverseColor);
      leds.setPixel(r + ledsPerStrip, reverseColor);
      leds.setPixel(r + 2 *ledsPerStrip, reverseColor);
    }
  }
  offset = (offset + 1) % ledsPerStrip;
  leds.show();
  delay(frameDelay);
}


// Clear all the pixels
void clear() {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, 0);
  }
}

void createHeliadesBuffer() {
  bool flip = true;
  for (int i = 0; i < ledsPerStrip; i++) {
    heliades[i] = flip;

    if (flip) {
      if (random(255) < 128) {
        flip = !flip;
      }
    } else {
      if (random(255) < 32) {
        flip = !flip;
      }
    }
  }
}
