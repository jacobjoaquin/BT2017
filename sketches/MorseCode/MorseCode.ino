/*
MorseCode.ino - Morse Code Pattern
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
#include "encoded.h"

// Combines red, green, blue into a single value
#define rgb(R, G, B)  ((((uint32_t)(R)) << 16) | (((uint32_t)(G)) << 8) | ((uint32_t)(B)))

// Get encoded
#define getEncoded(I) (pgm_read_byte(&encoded[(I)]))

const int ledsPerStrip = 456;
const int nStrips = 3;
const int nFaces = 8;
const int nLeds = ledsPerStrip * nStrips;
const int ledsPerBeam = ledsPerStrip / 4;
const int ledsPerFace = 3 * ledsPerBeam;
const int frameDelay = 1000 / 120;

// Setup OctoWS2811 library
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// Colors
uint32_t orange = rgb(255, 64, 0);
uint32_t magenta = rgb(255, 0, 128);
uint32_t black = rgb(0, 0, 0);
uint32_t white = rgb(255, 255, 255);
// uint32_t dotColor = magenta;
// uint32_t dahColor = orange;
uint32_t dotColor = orange;
uint32_t dahColor = orange;

// Morse Code
int stripOffsets[nStrips] = {0};
uint32_t palette[] = {black, orange, magenta};
int paletteSize = sizeof(palette) / sizeof(uint32_t);
int lastColors[nStrips] = {0};
int currentColors[nStrips] = {1};

int targetStrip = 0;
int targetDirection = 1;

int framesPerTransition = ledsPerStrip / 4;
int framesLeft = framesPerTransition;
void setup() {
  leds.begin();
}

void loop() {
  clear();

  framesLeft--;

  int offset = stripOffsets[targetStrip];
  offset += targetDirection;
  if (offset == ledsPerStrip) {
    offset -= ledsPerStrip;
  } else if (offset < 0) {
    offset += ledsPerStrip;
  }
  stripOffsets[targetStrip] = offset;

  // Display LEDs
  for (int i = 0; i < nStrips; i++ ) {
    int stripOffset = stripOffsets[i];

    // Color
    uint32_t c0 = palette[lastColors[i]];
    uint32_t c1 = palette[currentColors[i]];
    uint32_t c = lerpColor(c0, c1, (float) (framesPerTransition - 1 - framesLeft) / (float) framesPerTransition);

    for (int j = 0; j < ledsPerStrip; j++) {
      int index = (j + stripOffset) % encodedLength;
      uint8_t v = getEncoded(index);
      if (v) {
        leds.setPixel(j + i * ledsPerStrip, c);
      }
    }
  }

  // Choose next
  if (framesLeft == 0) {
    framesLeft = framesPerTransition;

    lastColors[targetStrip] = currentColors[targetStrip];


    // Select new target
    int lastTargetStrip = targetStrip;
    targetStrip = random(0, nStrips);

    if (targetStrip == lastTargetStrip) {
      targetDirection *= -1;
    } else {
      targetDirection = random(2) ? -1 : 1;
    }

    // Select from colors
    int newColor = random(0, paletteSize);
    while (newColor == currentColors[targetStrip]) {
      newColor = random(0, paletteSize);
    }
    // currentColors[targetStrip] = 1 - currentColors[targetStrip];
    currentColors[targetStrip] = newColor;
  }

  leds.show();
  delay(frameDelay);
}

// Clear all the pixels
void clear() {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, 0);
  }
}

// Interpolate between two colors.
uint32_t lerpColor(uint32_t c1, uint32_t c2, float amt) {
  int i = (int) (amt * 256.0) + 1;
  int di = 256 - i;
  uint32_t r1 = (c1 & 0xff0000) >> 16;
  uint32_t g1 = (c1 & 0x00ff00) >> 8;
  uint32_t b1 = (c1 & 0x0000ff);
  uint32_t r2 = (c2 & 0xff0000) >> 16;
  uint32_t g2 = (c2 & 0x00ff00) >> 8;
  uint32_t b2 = (c2 & 0x0000ff);
  r1 = ((r2 * i) + (r1 * di)) >> 8;
  g1 = ((g2 * i) + (g1 * di)) >> 8;
  b1 = ((b2 * i) + (b1 * di)) >> 8;
  return (r1 << 16) | (g1 << 8) | b1;
}
