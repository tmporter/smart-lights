#include "FastLED.h"
#include <Mailbox.h>
#include <QueueArray.h>
#include <stdlib.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN 5
//#define CLK_PIN   4
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS 50
CRGB leds[NUM_LEDS];

#define FRAMES_PER_SECOND 120

int BRIGHTNESS = 127;

void setup()
{
   delay(3000); // 3 second delay for recovery
   setupMailbox();
   setupLeds();
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0;                  // rotating "base color" used by many of the patterns

void setupMailbox()
{
   Bridge.begin();
   Mailbox.begin();

   // Initialize Serial
   SerialUSB.begin(9600);

   // Wait until a Serial Monitor is connected.
   //while (!SerialUSB);

   SerialUSB.println("Waiting for Mailbox messages\n");
}

void setupLeds()
{
   // tell FastLED about the LED strip configuration
   FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

   // set master brightness control
   FastLED.setBrightness(BRIGHTNESS);

   setColor(0xffffff);
}

void loop()
{
   // Call the current pattern function once, updating the 'leds' array
   // gPatterns[gCurrentPatternNumber]();

   String message;

   if (Mailbox.messageAvailable())
   {
      while (Mailbox.messageAvailable())
      {
         Mailbox.readMessage(message);
         // SerialUSB.println(message);
         handleMessage(message);
      }
   }

   // send the 'leds' array out to the actual LED strip
   FastLED.show();
   // insert a delay to keep the framerate modest
   FastLED.delay(1000 / FRAMES_PER_SECOND);

   // do some periodic updates
   EVERY_N_MILLISECONDS(20) { gHue++; }   // slowly cycle the "base color" through the rainbow
   EVERY_N_SECONDS(10) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void handleMessage(String message)
{
   if (message.startsWith("0x"))
   {
      SerialUSB.println(message);
      SerialUSB.println(strtol(message.c_str(), NULL, 0));
      setColor(strtol(message.c_str(), NULL, 0));
   }

   if (message.startsWith("b")) {
      message = message.substring(1);

      BRIGHTNESS = 255 * strtol(message.c_str(), NULL, 0) / 100;
      FastLED.setBrightness(BRIGHTNESS);
   }
}

void setColor(long hex)
{
   fill_solid(leds, NUM_LEDS, hex);
}

void nextPattern()
{
   // add one to the current pattern number, and wrap around at the end
   gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void rainbow()
{
   // FastLED's built-in rainbow generator
   fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
   // built-in FastLED rainbow, plus some random sparkly glitter
   rainbow();
   addGlitter(80);
}

void addGlitter(fract8 chanceOfGlitter)
{
   if (random8() < chanceOfGlitter)
   {
      leds[random16(NUM_LEDS)] += CRGB::White;
   }
}

void confetti()
{
   // random colored speckles that blink in and fade smoothly
   fadeToBlackBy(leds, NUM_LEDS, 10);
   int pos = random16(NUM_LEDS);
   leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon()
{
   // a colored dot sweeping back and forth, with fading trails
   fadeToBlackBy(leds, NUM_LEDS, 20);
   int pos = beatsin16(13, 0, NUM_LEDS);
   leds[pos] += CHSV(gHue, 255, 192);
}

void bpm()
{
   // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
   uint8_t BeatsPerMinute = 62;
   CRGBPalette16 palette = PartyColors_p;
   uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
   for (int i = 0; i < NUM_LEDS; i++)
   { //9948
      leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
   }
}

void juggle()
{
   // eight colored dots, weaving in and out of sync with each other
   fadeToBlackBy(leds, NUM_LEDS, 20);
   byte dothue = 0;
   for (int i = 0; i < 8; i++)
   {
      leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
      dothue += 32;
   }
}
