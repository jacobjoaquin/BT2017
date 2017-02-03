/*  OctoWS2811 BasicTest.ino - Basic RGB LED Test
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    Copyright (c) 2013 Paul Stoffregen, PJRC.COM, LLC

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
    pin 8:  LED strip #3    A 100 ohm resistor should used
    pin 6:  LED strip #4    between each Teensy pin and the
    pin 20: LED strip #5    wire to the LED strip, to minimize
    pin 21: LED strip #6    high frequency ringining & noise.
    pin 5:  LED strip #7
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.

  This test is useful for checking if your LED strips work, and which
  color config (WS2811_RGB, WS2811_GRB, etc) they require.


  Notes:

  ** Important! **
  I've renamed the strip numbers from 1-8 to 0-7. This is more aligned to
  computer programming thinking as the index of the first element in a
  data structure is 0.

  A strip is a square consisting of 456 LEDs

  Breakdown of segments:
  segment 0: 0 - 113
  segment 1: 114 - 227
  segment 2: 228 - 341
  segment 3: 342 - 455


  //Strip indexes
  0 = Green
  1 = Yellow
  2 = Red

  // Modeled on d8
  0 r2  g3  y1
  1 y1- g0  r1
  2 r1- g1  y2-
  3 y2  g2  r2-

  4 y3- g2  r3
  5 r3- g3  y0-
  6 y0  g0  r0-
  7 r0  g1  y3
*/

void setFaceColor(int face, int r, int g, int b) {
  if (face == 0) {
    setSegment(2, 2, r, g, b);
    setSegment(0, 3, r, g, b);
    setSegment(1, 1, r, g, b);
  } else if (face == 1) {
    setSegment(1, 1, r, g, b);
    setSegment(0, 0, r, g, b);
    setSegment(2, 1, r, g, b);
  } else if (face == 2) {
    setSegment(2, 1, r, g, b);
    setSegment(0, 1, r, g, b);
    setSegment(1, 2, r, g, b);
  } else if (face == 3) {
    setSegment(1, 2, r, g, b);
    setSegment(0, 2, r, g, b);
    setSegment(2, 2, r, g, b);
  } else if (face == 4) {
    setSegment(1, 3, r, g, b);
    setSegment(0, 2, r, g, b);
    setSegment(2, 3, r, g, b);
  } else if (face == 5) {
    setSegment(2, 3, r, g, b);
    setSegment(0, 3, r, g, b);
    setSegment(1, 1, r, g, b);
  } else if (face == 6) {
    setSegment(1, 0, r, g, b);
    setSegment(0, 0, r, g, b);
    setSegment(2, 0, r, g, b);
  } else if (face == 7) {
    setSegment(2, 0, r, g, b);
    setSegment(0, 1, r, g, b);
    setSegment(1, 3, r, g, b);
  }
}

#include <OctoWS2811.h>

const int ledsPerStrip = 456;
const int nStrips = 3;
const int nFaces = 8;
const uint16_t nLeds = ledsPerStrip * nStrips;
const int ledsPerSegment = 114;
const uint16_t ledsPerTriangle = 3 * ledsPerSegment;

DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// User define variables
int currentFace = 0;

void setup() {
  leds.begin();
}

void loop() {
  clear();
  setFaceColor(currentFace, 255, 0, 128);
  leds.show();
  currentFace = (currentFace + 1) % nFaces;
  delay(250);
}

void setSegment(int stripNumber, int segmentNumber, int r, int g, int b) {
  uint16_t start = stripNumber * ledsPerStrip + segmentNumber * ledsPerSegment;
  uint16_t end = start + ledsPerSegment;
  for (uint16_t i = start; i < end; i++) {
    leds.setPixel(i, (r << 16) | (g << 8) | b);
  }
}

void setStrip(int stripNumber, int r, int g, int b) {
  uint16_t start = stripNumber * ledsPerStrip;
  uint16_t end = start + ledsPerStrip;

  for (uint16_t i = start; i < end; i++) {
    leds.setPixel(i, (r << 16) | (g << 8) | b);
  }
}

// Set all gray scale
void setAllGray(int b) {
  for (uint16_t i = 0; i < nLeds; i++) {
    leds.setPixel(i, (b << 16) | (b << 8) | b);
  }
}

// Set all color
void setAllColor(uint32_t c) {
  for (uint16_t i = 0; i < nLeds; i++) {
    leds.setPixel(i, c);
  }
}

// Set all RGB
void setAllRGB(int r, int g, int b) {
  for (uint16_t i = 0; i < nLeds; i++) {
    leds.setPixel(i, (r << 16) | (g << 8) | b);
  }
}

// Clear all the pixels
void clear() {
  setAllGray(0);
}
