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
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.

  This test is useful for checking if your LED strips work, and which
  color config (WS2811_RGB, WS2811_GRB, etc) they require.
*/


/*
R = Strip #0
G = Strip #1
B = Strip #2

0 - 113
114 - 227
228 - 341
342 - 455
*/

#include <OctoWS2811.h>

typedef uint32_t COLOR;

const int ledsPerStrip = 456;
const int nStrips = 3;
uint16_t nLeds = ledsPerStrip * nStrips;
int ledsPerSegment = 114;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

void setup() {
  leds.begin();
}

void loop() {
  clear();
  setSegment(0, 0, 255, 48, 0);
  leds.show();
  delay(500);
  clear();
  setSegment(0, 1, 255, 0, 128);
  leds.show();
  delay(500);
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
void setAllColor(COLOR c) {
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








