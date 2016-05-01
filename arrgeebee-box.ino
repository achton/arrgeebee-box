/**
 *  arrgeebee box (RGB box)
 *  A circuit which utilizes some WS2812B RGB SMD 5050 LEDs (eg. Neopixels) or similar
 *  and a HC-SR501 PIR sensor or similar to create a motiondetection device in a box. 
 *  Can be used as a night light, for example.
 *  Depends on Adafruit's NeoPixel library and example code.
 */

#include <Adafruit_NeoPixel.h>

#define LED_PIN           D4  // Pin number for RGB LEDs.
#define SENSOR_PIN        D5  // Pin for the PIR sensor.
#define NUMPIXELS         3   // Set to number of RGB LEDs you have.
#define DEBUG             0   // Set to 1 to get serial debug output.

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {

  if (DEBUG > 0) {
    Serial.begin(115200);
  }

  // Initialize the NeoPixel library.
  strip.begin();
  strip.setBrightness(255);
  // Turn off pixels.  
  strip.show();

  // Show init light cycle for 10s to let the PIR sensor get a chance to warm up.
  initCycle(10);
  
  delay(50);
}


// Initialize the variable which holds the current value of the sensor.
int sensorValue = 0;

void loop() {

  // Read from the sensor.
  sensorValue = digitalRead(SENSOR_PIN);

  if (DEBUG > 0) {
    Serial.println("-----");
    Serial.print("sensorValue: ");
    Serial.println(sensorValue);
  }

  // If sensor is triggered, activate the LEDs.
  if (sensorValue == 1) {
    if (DEBUG > 0) {
      Serial.println("Sensor triggered, showing sequence 1.");
    }
    rainbowCycle(5);

    if (DEBUG > 0) {
      Serial.println("Showing sequence 2.");
    }
    rainbow(25);
    
    if (DEBUG > 0) {
      Serial.println("Showing sequence 3.");
    }
    rainbowCycle(30);
    
    if (DEBUG > 0) {
      Serial.println("Sequence done.");
    }
  }
  // If sensor is not triggered, make sure the LEDs are turned off.
  else {
    if (DEBUG > 0) {
      Serial.println("Clearing pixels.");
    }

    strip.clear();
    strip.setPixelColor(0,strip.Color(0,0,0));
    strip.show();
  }

  delay(50);
}

/*
 * A simple function to activate the LEDs after a restart 
 * while the PIR sensor is starting.
 */
void initCycle(uint8_t wait) {
  uint16_t i, j;

  if (DEBUG > 0) {
    Serial.println("INIT BEGIN.");
  }

  for(j=0; j<wait; j++) {
    for(i=0; i < strip.numPixels(); i++) {
      strip.clear();
      strip.show();
      if (i % 2 == 0 && j % 2 == 0) {
        strip.setPixelColor(i, 255, 0, 0);
      } else if (i % 2 == 1 && j % 2 == 1) {
        strip.setPixelColor(i, 0, 255, 0);
      } else {
        strip.setPixelColor(i, 0, 0, 255);
      }
      if (DEBUG > 0) {
        Serial.print(".");
      }
      strip.show();
      delay(450);
    }
  }
  
  if (DEBUG > 0) {
    Serial.println("INIT END.");
  }
}

/*
 * Code below is a straight copy/paste from the 'strandtest' example in the NeoPixel library.
 */
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
