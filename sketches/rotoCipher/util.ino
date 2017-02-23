// Create the beamBuffer
void createBeamBuffer() {
  int halfBeam = ledsPerBeam / 2;

  for (int i = 0; i < halfBeam; i++) {
    float amt = (float) i / (float) halfBeam;
    uint32_t c0 = lerpColor(0, orange, amt);
    uint32_t c1 = lerpColor(magenta, 0, amt);
    for (int j = 0; j < nBeams; j++) {
      beamBuffer[i + j * ledsPerBeam] = c0;
      beamBuffer[i + halfBeam + j * ledsPerBeam] = c1;
    }
  }
}

void createBeamBuffer2() {
  int tail = 8;
  int halfBeam = ledsPerBeam / 2 - tail;

  for (int i = 0; i < halfBeam; i++) {
    float amt = (float) i / (float) halfBeam;
    uint32_t c0 = lerpColor(0, orange, amt);
    uint32_t c1 = lerpColor(magenta, 0, amt);
    for (int j = 0; j < nBeams; j++) {
      beamBuffer[tail + i + j * ledsPerBeam] = c0;
      beamBuffer[i + halfBeam + j * ledsPerBeam] = c1;
    }
  }
}

// Sparkle buffer
void createSparkleBuffer() {
  for (int i = 0; i < nLeds; i++) {
    sparkleBuffer[i] = random(256);
  }
}

// sets LEDs to contents of beamBuffer
void beamBufferToLEDs() {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, beamBuffer[i]);
  }
}

void beamBufferToLEDs2() {
  for (int i = 0; i < nLeds; i++) {
    uint32_t c = 0;
    if (random(256) < 16) {
      c = lerpColor(0, beamBuffer[i], (int) random(256));
    } else {
      c = beamBuffer[i];
    }
    leds.setPixel(i, c);
  }
}

void beamBufferToLEDs3() {
  for (int i = 0; i < nLeds; i++) {
    uint32_t c = beamBuffer[i];
    int amt = sparkleBuffer[i];
    c = lerpColor(shiftColor(c, 1), c, amt);
    leds.setPixel(i, c);

    amt -= (random(8, 12));
    amt += 256 * (amt < 0);
    sparkleBuffer[i] = amt;
  }
}


// Generate SineTable
void createSineTable() {
  for (int i = 0; i < sineTableSize; i++) {
    float v = (sin((float) i / (float) sineTableSize * TWOPI) * 0.5 + 0.5) * 255.0;
    sineTable[i] = (int) v;
  }
}

// Clear all the pixels
void clear() {
  for (int i = 0; i < nLeds; i++) {
    leds.setPixel(i, 0);
  }
}

// Interpolate between two colors. amt: [0.0-1.0)
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

// Interpolate between two colors. amt: [0-255]
uint32_t lerpColor(uint32_t c1, uint32_t c2, int amt) {
  int i = amt + 1;
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

uint32_t shiftColor(uint32_t c, int shift) {
  uint32_t r1 = (c & 0xff0000) >> 16;
  uint32_t g1 = (c & 0x00ff00) >> 8;
  uint32_t b1 = (c & 0x0000ff);
  r1 = r1 >> shift;
  g1 = g1 >> shift;
  b1 = b1 >> shift;
  return (r1 << 16) | (g1 << 8) | b1;
}
