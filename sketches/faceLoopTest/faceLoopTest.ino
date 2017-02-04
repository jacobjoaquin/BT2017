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
*/

#include <OctoWS2811.h>

#define color(R, G, B)  ( ( R << 16 ) | ( G << 8 ) | B )

const int ledsPerStrip = 456;
const int nStrips = 3;
const int nFaces = 8;
const int nLeds = ledsPerStrip * nStrips;
const int ledsPerBeam = 114;
const int ledsPerFace = 3 * ledsPerBeam;

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
  for (int i = 0; i < ledsPerFace; i++) {
    clear();
    setFaceLED(currentFace, i, 255, 0, 128);
    leds.show();
    delay(1000 / 120);  // Attempting 120 frames per second
  }
  currentFace = (currentFace + 1) % nFaces;
}

void setBeam(uint8_t stripNumber, uint8_t beamNumber, uint8_t r, uint8_t g, uint8_t b) {
  int start = stripNumber * ledsPerStrip + beamNumber * ledsPerBeam;
  int end = start + ledsPerBeam;
  for (int i = start; i < end; i++) {
    leds.setPixel(i, color(r, g, b));
  }
}

void setStrip(uint8_t stripNumber, uint8_t r, uint8_t g, uint8_t b) {
  int start = stripNumber * ledsPerStrip;
  int end = start + ledsPerStrip;

  for (int i = start; i < end; i++) {
    leds.setPixel(i, color(r, g, b));
  }
}

// Set all gray scale
void setAllGray(uint8_t b) {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, color(b, b, b));
  }
}

// Set all color
void setAllColor(uint32_t c) {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, c);
  }
}


// Set all RGB
void setAllRGB(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, color(r, g, b));
  }
}

// Clear all the pixels
void clear() {
  setAllGray(0);
}

// Set a face
void setFaceColor(uint8_t face, uint8_t r, uint8_t g, uint8_t b) {
  if (face == 0) {
    setBeam(2, 2, r, g, b);
    setBeam(0, 3, r, g, b);
    setBeam(1, 1, r, g, b);
  } else if (face == 1) {
    setBeam(1, 1, r, g, b);
    setBeam(0, 0, r, g, b);
    setBeam(2, 1, r, g, b);
  } else if (face == 2) {
    setBeam(2, 1, r, g, b);
    setBeam(0, 1, r, g, b);
    setBeam(1, 2, r, g, b);
  } else if (face == 3) {
    setBeam(1, 2, r, g, b);
    setBeam(0, 2, r, g, b);
    setBeam(2, 2, r, g, b);
  } else if (face == 4) {
    setBeam(1, 3, r, g, b);
    setBeam(0, 2, r, g, b);
    setBeam(2, 3, r, g, b);
  } else if (face == 5) {
    setBeam(2, 3, r, g, b);
    setBeam(0, 3, r, g, b);
    setBeam(1, 1, r, g, b);
  } else if (face == 6) {
    setBeam(1, 0, r, g, b);
    setBeam(0, 0, r, g, b);
    setBeam(2, 0, r, g, b);
  } else if (face == 7) {
    setBeam(2, 0, r, g, b);
    setBeam(0, 1, r, g, b);
    setBeam(1, 3, r, g, b);
  }
}

/*
   face: Index of face. [0-7]  (use -1 for DEBUG)
   index: index of LED in face. [0-ledsPerFace)
   r: red [0-255]
   g: green [0-255]
   b: blue [0-255]
*/
void setFaceLED(int face, int index, uint8_t r, uint8_t g, uint8_t b) {
  int i = 0;
  if (face == 0) {
    // [r2, g3, -y1]
    if (index < ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 2, 2, 0, false);
    } else if (index < 2 * ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 0, 3, 1, false);
    } else {
      i = faceIndexToDisplayIndex(index, 1, 1, 2, true);
    }
  } else if (face == 1) {
    // [-y1, g0, r1]
    if (index < ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 1, 1, 0, true);
    } else if (index < 2 * ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 0, 0, 1, false);
    } else {
      i = faceIndexToDisplayIndex(index, 2, 1, 2, false);
    }
  } else if (face == 2) {
    // [-r1, g1, -y2]
    if (index < ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 2, 1, 0, true);
    } else if (index < 2 * ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 0, 1, 1, false);
    } else {
      i = faceIndexToDisplayIndex(index, 1, 2, 2, true);
    }
  } else if (face == 3) {
    // [y2, g2, -r2]
    if (index < ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 1, 2, 0, false);
    } else if (index < 2 * ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 0, 2, 1, false);
    } else {
      i = faceIndexToDisplayIndex(index, 2, 2, 2, true);
    }
  } else if (face == 4) {
    // [-y3, g2, r3]
    if (index < ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 1, 3, 0, true);
    } else if (index < 2 * ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 0, 2, 1, false);
    } else {
      i = faceIndexToDisplayIndex(index, 2, 3, 2, false);
    }
  } else if (face == 5) {
    // [-r3, g3, -y0]
    if (index < ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 2, 3, 0, true);
    } else if (index < 2 * ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 0, 3, 1, false);
    } else {
      i = faceIndexToDisplayIndex(index, 1, 0, 2, true);
    }
  } else if (face == 6) {
    // [y0, g0, -r0]
    if (index < ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 1, 0, 0, false);
    } else if (index < 2 * ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 0, 0, 1, false);
    } else {
      i = faceIndexToDisplayIndex(index, 2, 0, 2, true);
    }
  } else if (face == 7) {
    //     [r0, g1, y3]
    if (index < ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 2, 0, 0, false);
    } else if (index < 2 * ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 0, 1, 1, false);
    } else {
      i = faceIndexToDisplayIndex(index, 1, 3, 2, false);
    }

  } else if (face == -1) {
    // DEBUG
    if (index < ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 0, 1, 0, true);
      //      i = index;
    } else if (index < 2 * ledsPerBeam) {
      i = faceIndexToDisplayIndex(index, 1, 1, 1, true);
    } else {
      i = faceIndexToDisplayIndex(index, 2, 1, 2, true);
    }
  }

  leds.setPixel(i, color(r, g, b));
}

/*
   index: LED position in traingle. [0, ledsPerFace)
   strip: Square index. g = 0, y = 1, r = 2
   beam: Beam index in square. [0, 4)
   traingle: traingle beam index. [0, 3)
   isReverse: Determines the direction of flow
*/
int faceIndexToDisplayIndex(int index, int strip, int beam, int triangle, boolean isReverse) {
  index -= triangle * ledsPerBeam;
  index *= 1 - isReverse * 2;
  return strip * ledsPerStrip + ((beam + isReverse) * ledsPerBeam - isReverse) + index;
}


