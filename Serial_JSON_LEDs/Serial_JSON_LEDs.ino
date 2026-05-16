#include <ArduinoJson.h>
#include <FastLED.h>
#define DATA_PIN 6
#define NUM_LEDS 93
CRGB leds[NUM_LEDS];
String mode = "rainbow";
uint8_t baseHue = 0;
uint8_t hueSpread = 4;
uint8_t rainbowSpeed = 20;
uint8_t saturation = 255;
uint8_t value = 255;
uint8_t brightness = 100;
uint8_t r = 255;
uint8_t g = 255;
uint8_t b = 255;

int blinkSpeed = 150;
int blinkCount = 6;
bool blinkActive = false;
int blinkStep = 0;
unsigned long lastBlinkTime = 0;
bool blinkOn = false;
void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.setBrightness(brightness);
  FastLED.show();

  Serial.begin(9600);
}

void loop() {
  readValues();

  FastLED.setBrightness(brightness);

  if (mode == "rainbow") {
    runRainbow();
  }
  else if (mode == "static") {
    runStatic();
  }
  else if (mode == "blink") {
    runBlink();
  }
  else if (mode == "off") {
    FastLED.clear();
    FastLED.show();
  }
}

void runRainbow() {
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t hue = baseHue + (i * hueSpread);
    leds[i] = CHSV(hue, saturation, value);
  }

  FastLED.show();

  baseHue++;
  delay(rainbowSpeed);
}

void runStatic() {
  fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
  FastLED.show();
  delay(20);
}

void runBlink() {
  unsigned long now = millis();

  if (!blinkActive) {
    blinkActive = true;
    blinkStep = 0;
    blinkOn = false;
    lastBlinkTime = now;
  }

  if (now - lastBlinkTime >= blinkSpeed) {
    lastBlinkTime = now;
    blinkOn = !blinkOn;

    if (blinkOn) {
      fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    } else {
      FastLED.clear();
      blinkStep++;
    }

    FastLED.show();

    if (blinkStep >= blinkCount) {
      blinkActive = false;
      mode = "rainbow";
    }
  }
}

void readValues() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() == 0) {
      return;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, input);

    if (error) {
      return;
    }

    mode = doc["mode"] | "rainbow";
    brightness = doc["brightness"] | brightness;

    if (mode == "rainbow") {
      baseHue = doc["baseHue"] | baseHue;
      hueSpread = doc["hueSpread"] | hueSpread;
      rainbowSpeed = doc["speed"] | rainbowSpeed;
      saturation = doc["saturation"] | saturation;
      value = doc["value"] | value;
      blinkActive = false;
    }

    else if (mode == "static") {
      r = doc["r"] | r;
      g = doc["g"] | g;
      b = doc["b"] | b;
      blinkActive = false;
    }

    else if (mode == "blink") {
      r = doc["r"] | r;
      g = doc["g"] | g;
      b = doc["b"] | b;
      blinkSpeed = doc["blinkSpeed"] | blinkSpeed;
      blinkCount = doc["blinkCount"] | blinkCount;

      blinkActive = false;
      blinkStep = 0;
    }

    else if (mode == "off") {
      brightness = 0;
      blinkActive = false;
    }
  }
}
