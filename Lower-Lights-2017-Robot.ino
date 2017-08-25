#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 5
#define PINQ 4
#define NUM_PIXELS 60
#define NUM_PIXELSQ 60

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripQ = Adafruit_NeoPixel(NUM_PIXELSQ, PINQ, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int Speed = 0;
int Speed2 = 75;
int Wait = 750;

const int blueSwitch = 2;
const int redSwitch = 3;
volatile int blueSensorValue = digitalRead(blueSwitch);
volatile int redSensorValue = digitalRead(redSwitch);


void setup() {
  /* This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
     End of Trinket special code
  */
  /*
  pinMode(blueSwitch, INPUT_PULLUP);
  pinMode(redSwitch, INPUT_PULLUP);
  attachInterrupt (digitalPinToInterrupt(blueSwitch), loop, FALLING);
  attachInterrupt (digitalPinToInterrupt(redSwitch), loop, FALLING);
*/
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(20);
  stripQ.begin();
  stripQ.show(); // Initialize all pixels to 'off'
  stripQ.setBrightness(20);
}

void loop() {
  /*
  AllianceSelector();
*/
  //FIRST(Speed2);
//  TwoColorWheel(strip.Color(255, 0, 0), strip.Color(200, 46, 0));

  colorWipe(strip.Color(255, 0, 0), Speed); // Red
  delay(Wait);
  colorWipe(strip.Color(0, 0, 0), Speed); //Blank
  colorWipe(strip.Color(0, 0, 255), Speed); // Blue
  delay(Wait);
  colorWipe(strip.Color(0, 0, 0), Speed); //Blank
  colorWipe(strip.Color(200, 46, 0), Speed); //Orange
  delay(Wait);
  colorWipe(strip.Color(0, 0, 0), Speed); //Blank
  colorWipe(strip.Color(0, 225, 0), Speed); //Green
  delay(Wait);
  colorWipe(strip.Color(0, 0, 0), Speed); //Blank
/*
  crossFade(0);
  colorWipeRainbow(Speed);
  theaterChaseRainbow(Speed2);
*/
  // Send a theater pixel chase in...
  theaterChase(strip.Color(0, 0, 255), Speed2); // Blue
  theaterChase(strip.Color(200, 46, 0), Speed2); // Sweet, sweet, orange
  theaterChase(strip.Color(200, 46, 0), Speed2); // Sweet orange
  theaterChase(strip.Color(0, 225, 0), Speed2); //Green
  theaterChase(strip.Color(200, 46, 0), Speed2); // Sweet, sweet, orange
  theaterChase(strip.Color(200, 46, 0), Speed2); // Sweet orange
  theaterChase(strip.Color(255, 0, 0), Speed2); // Red
  //Pulse(255);
}

void setStrips(Adafruit_NeoPixel* strip1, Adafruit_NeoPixel* strip2, int i, uint32_t c) {
  int  h = 119 - i;
  strip1->setPixelColor(i, c);
  if (i <= 120 && i >= 88) {
    if (h < 19) {
      h = h + 1;
    }
    strip2->setPixelColor(h, c);
  }
  strip2->show();
  strip1->show();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    //setStrips(&strip, &stripQ, i, c);
    strip.setPixelColor(i, c);
    stripQ.setPixelColor(i, c);
    delay(wait);
  }
}
//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
        stripQ.setPixelColor(i + q, c);
      }
      strip.show();
      stripQ.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
        stripQ.setPixelColor(i + q, 0);
      }
    }
  }
}


//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
        stripQ.setPixelColor(i + q, Wheel(((i + 89) + j) % 255));
      }
      strip.show();
      stripQ.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
        stripQ.setPixelColor(i + q, 0);
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void colorWipeQ(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    stripQ.setPixelColor(i, c);
    stripQ.show();
    delay(wait);
  }
}

int lowbound = 0;
int highbound = 9;
int slwdwn = 1;
int r = 0;
int g = 0;
int b = 0;

void colorWipeRainbow (uint8_t wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(i));
    setStrips(&strip, &stripQ, i, Wheel(i));
    delay(wait);
  } delay(1000);
}

void colorWipePixRainbow () {
  for (int i = 0; i <= strip.numPixels(); i++) {
    for (int c = 0; c < 255; c += 10) {
      setStrips(&strip, &stripQ, i, Wheel(c + i));
      setStrips(&strip, &stripQ, strip.numPixels() - i - 1, Wheel(c + i));
      setStrips(&strip, &stripQ, i + 57, Wheel(c + i));
      setStrips(&strip, &stripQ, 57 - i - 1, Wheel(c + i));
    }
  }
  colorWipe(strip.Color(0, 0, 0), Speed); //Blank
}

void crossFade(uint8_t wait) {
  r = 200;
  g = 55;
  b = 0;

  colorWipe(strip.Color(r, g, b), Speed); //Blank
  for (int r = 200; r > 0 && g < 255; r-- && g++) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, r, g, b); //fades to green
      stripQ.setPixelColor(i, r, g, b);
    }
    strip.show();
    stripQ.show();
  }
  delay(250);
  r = 0;
  g = 255;
  for (int b = 0; b < 255; b++) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0, g, b); //fades to blue green
      stripQ.setPixelColor(i, r, g, b);
    }
    strip.show();
    stripQ.show();
  }
  b = 255;
  r = 0;
  for (int g = 255; g > 0; g--) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, r, g, b); //fades green back to only blue
      stripQ.setPixelColor(i, r, g, b);
    }
    strip.show();
    stripQ.show();
  }
  g = 0;
  b = 255;
  for (int r = 0; r < 255; r++) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, r, g, b); //fades to purple
      stripQ.setPixelColor(i, r, g, b);
    }
    strip.show();
    stripQ.show();
  }
  r = 255;
  for (int b = 255; b > 0; b--) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, r, g, b); //fades blue back to only red
      stripQ.setPixelColor(i, r, g, b);
    }
    strip.show();
    stripQ.show();
  }
  r = 255;
  b = 0;
  for (int g = 0; g < 55 && r > 200; g++ && r--) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, r, g, b); //fades to orange
      stripQ.setPixelColor(i, r, g, b);
    }
    strip.show();
    stripQ.show();
  }
  delay(500);
  colorWipe(strip.Color(0, 0, 0), Speed); //Blank
}


void Red() {
  while (digitalRead(redSwitch) == LOW) {
    colorWipe(strip.Color(100, 0, 0), Speed);
  }
  colorWipe(strip.Color(0, 0, 0), Speed); //Blank
  loop();
}

void Blue() {
  while (digitalRead(blueSwitch) == LOW) {
    colorWipe(strip.Color(0, 0, 100), Speed);
  }
  colorWipe(strip.Color(0, 0, 0), Speed); //Blank
  loop();
}

void AllianceSelector() {
  if (digitalRead(blueSwitch) == LOW) {
    Blue();
  }
  if (digitalRead(redSwitch) == LOW) {
    Red();
  }
}

void Fire() {
  for (int i = 0; i < strip.numPixels(); i++) {
    r = rand() % 106 + 150;
    g = rand() % 125;
    int      k = rand() % i;
    strip.setPixelColor(i, r, g, b);
    strip.setPixelColor(k, r, g, b);
    r = rand() % 106 + 150;
    g = rand() % 125;
    k = rand() % i;
    stripQ.setPixelColor(119 - i, r, g, b);
    stripQ.setPixelColor(119 - k, r, g, b);
    strip.show();
    stripQ.show();
    delay(10);
  }
  for (int j = 0; j < 2500; j++) {
    r = rand() % 106 + 150;
    g = rand() % 125;
    int   i = rand() % strip.numPixels() ;
    strip.setPixelColor(i, r, g, b);
    strip.show();
  }
  for (int i = 129; i > 0; i--) {
    r = rand() % 106 + 150;
    g = rand() % 125;
    int    k = rand() % i;
    strip.setPixelColor(i, 0, 0, 0); //Blanks out the pixels in a colorWipe while the remaining ones still flicker
    strip.setPixelColor(k, r, g, b); //Remaining pixels
    r = rand() % 106 + 150;
    g = rand() % 125;
    k = rand() % i;
    stripQ.setPixelColor(119 - i, 0, 0, 0); //Same as above, just with stripQ
    stripQ.setPixelColor(119 - k, r, g, b);
    strip.show();
    stripQ.show();
    delay(10);
  }
  delay(100);
}

void Candy() {
  for (int i = 0; i < strip.numPixels(); i++) {
    r = rand() % 256;
    g = rand() % 256;
    r = rand() % 256;
    int k = rand() % i;
    strip.setPixelColor(i, r, g, b);
    strip.setPixelColor(k, r, g, b);
    r = rand() % 256;
    g = rand() % 256;
    r = rand() % 256;
    k = rand() % i;
    stripQ.setPixelColor(119 - i, r, g, b);
    stripQ.setPixelColor(119 - k, r, g, b);
    strip.show();
    stripQ.show();
    delay(10);
  }
  for (int j = 0; j < 150; j++) {
    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;
    int  i = rand() % strip.numPixels();
    strip.setPixelColor(i, r, g, b);
    strip.show();
    delay(10);
  }
  colorWipe(strip.Color(0, 0, 0), Speed);
}

void Pulse(uint32_t c) {
  for (int j = 0; j < 4; j++) { //does 10 cycles of pulsing
    for (int i = 0; i < (strip.numPixels() + 5); i++) {
      setStrips(&strip, &stripQ, i, strip.Color(0, c, 0));
      setStrips(&strip, &stripQ, i - 1, strip.Color(0, c / 2, 0));
      setStrips(&strip, &stripQ, i - 2, strip.Color(0, c / 4, 0));
      setStrips(&strip, &stripQ, i - 3, strip.Color(0, c / 8, 0));
      setStrips(&strip, &stripQ, i - 4, strip.Color(0, c / 16, 0));
      setStrips(&strip, &stripQ, i - 5, 0);
    }
  }
}

void FIRST(uint8_t wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    if (i < strip.numPixels() / 3) {
      setStrips(&strip, &stripQ, i, strip.Color(255, 0, 0));
    }
    if (i > strip.numPixels() / 3 && i < 2 * strip.numPixels() / 3) {
      setStrips(&strip, &stripQ, i, strip.Color(255, 255, 255));
    }
    if (i > 2 * strip.numPixels() / 3 && i < strip.numPixels()) {
      setStrips(&strip, &stripQ, i, strip.Color(0, 0, 255));
    }
  }
  for (int j = 0; j < 100; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        if (i < strip.numPixels() / 3) {
          strip.setPixelColor(i + q, strip.Color(255, 0, 0));
          stripQ.setPixelColor(i - q, strip.Color(0, 0, 0));
        }
        if (i > strip.numPixels() / 3 && i < 2 * strip.numPixels() / 3) {
          strip.setPixelColor(i + q, strip.Color(85, 85, 85));
          stripQ.setPixelColor(i - q, strip.Color(0, 0, 0));
        }
        if (i > (2 * strip.numPixels() / 3)) {
          strip.setPixelColor(i + q, strip.Color(0, 0, 255));
          stripQ.setPixelColor(strip.numPixels() - (i - q), strip.Color(0, 0, 255));
        }
      }
      strip.show();
      stripQ.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
        stripQ.setPixelColor(i - q, 0);
      }
    }
  }
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 0, 0);
    stripQ.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
  stripQ.show();
}

void Heartbeat() {
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 25, 0, 0);
    }
    strip.show();
    delay(1000);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 180, 0, 0);
    }
    strip.show();
    delay(50);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 25, 0, 0);
    }
    strip.show();
    delay(75);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 180, 0, 0);
    }
    strip.show();
    delay(50);
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 25, 0, 0);
    }
    strip.show();
    delay(200);
  }
}

void RainbowPincers() {
  for (int j = 0; j < 10; j++) {
    for (int i = 0; i < (strip.numPixels() + 1) / 2; i++) {
      strip.setPixelColor(i, Wheel(i));
      setStrips(&strip, &stripQ, i, Wheel(i));
      strip.setPixelColor(strip.numPixels() - i, Wheel(i));
      setStrips(&strip, &stripQ, strip.numPixels() - i, Wheel(i));
    }
    for (int i = (strip.numPixels() / 2) + 1; i > 0; i--) {
      strip.setPixelColor(i, Wheel(i));
      setStrips(&strip, &stripQ, i, 0);
      strip.setPixelColor(strip.numPixels() - i, 0);
      setStrips(&strip, &stripQ, strip.numPixels() - i, 0);
    }
  }
}

void TwoColorWheel(uint32_t a, uint32_t b) {
  for (int q = 0; q < 3; q++) {
    for (int i = 0; i < strip.numPixels(); i = + 2) {
      strip.setPixelColor(i + q, a);
      strip.setPixelColor(i + 1 + q, b);
      stripQ.setPixelColor(i - q, a);
      stripQ.setPixelColor((i+1) -q, b);
    }
    strip.show();
    stripQ.show();
  }
}

