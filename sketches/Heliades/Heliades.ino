/*
Heliades.ino - Heliades Pattern
Inspired by Helios, Disorient 2014, by Leo Villareal.
Coded by Jacob Joaquin.

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
bool heliadesReverse[ledsPerStrip];
int offset = 0;
uint forwardOdds = 128;  // Chance on will flip to off. [0-255]
uint reverseOdds = 32;   // Chance off will flip to on. [0-255]

void setup() {
  leds.begin();
  createHeliadesBuffer(heliades);
  createHeliadesBuffer(heliadesReverse);
}

void loop() {
  clear();

  for (int i = 0; i < ledsPerStrip; i++) {
    int r = ledsPerStrip - i - 1;
    int forwardOffset = (i + offset) % ledsPerStrip;
    int reverseOffset = (r + offset) % ledsPerStrip;
    bool forward = heliades[forwardOffset];
    bool reverse = heliadesReverse[reverseOffset];

    if (forward) {
      int brightness = random(200, 255);
      int color = rgb(brightness, brightness, brightness);
      leds.setPixel(i, color);
      leds.setPixel(i + ledsPerStrip, color);
      leds.setPixel(i + 2 *ledsPerStrip, color);
    }
    if (reverse) {
      int brightness = random(200, 255);
      int color = rgb(brightness, brightness, brightness);
      leds.setPixel(i, color);
      leds.setPixel(i + ledsPerStrip, color);
      leds.setPixel(i + 2 *ledsPerStrip, color);
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

void createHeliadesBuffer(bool *buffer) {
  bool flip = true;
  for (int i = 0; i < ledsPerStrip; i++) {
    buffer[i] = flip;

    if (flip) {
      if (random(255) < forwardOdds) {
        flip = !flip;
      }
    } else {
      if (random(255) < reverseOdds) {
        flip = !flip;
      }
    }
  }
}
