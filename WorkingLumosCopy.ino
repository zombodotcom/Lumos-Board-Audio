#include "FastLED.h"

FASTLED_USING_NAMESPACE

//led stuff
#define DATA_PIN   27
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    300
CRGB leds[NUM_LEDS];
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120
int CENTER_LED = NUM_LEDS/2;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
String pattern;
String pat;
String sBrightFix;
int brightFixs;
String rxColorFix;
String sColorfix1;
String sColorFix;
uint8_t rgbR=128;
uint8_t rgbG=128;
uint8_t rgbB=128;
CRGB solidColor=CRGB::Blue;
#include "ble.h"
#include "patterns.h"
#include "audio.h"
#define FASTLED_SHOW_CORE 0

// -- Task handles for use in the notifications
static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t userTaskHandle = 0;

/** show() for ESP32
    Call this function instead of FastLED.show(). It signals core 0 to issue a show,
    then waits for a notification that it is done.
*/
void FastLEDshowESP32()
{
  if (userTaskHandle == 0) {
    // -- Store the handle of the current task, so that the show task can
    //    notify it when it's done
    userTaskHandle = xTaskGetCurrentTaskHandle();

    // -- Trigger the show task
    xTaskNotifyGive(FastLEDshowTaskHandle);

    // -- Wait to be notified that it's done
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
    ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    userTaskHandle = 0;
  }
}

/** show Task
    This function runs on core 0 and just waits for requests to call FastLED.show()
*/
void FastLEDshowTask(void *pvParameters)
{
  // -- Run forever...
  for (;;) {
    // -- Wait for the trigger
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    // -- Do the show (synchronously)
    FastLED.show();

    // -- Notify the calling task
    xTaskNotifyGive(userTaskHandle);
  }
}




void setup() {
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  // Create the BLE Device
 setupBLE();
  Serial.println("Waiting a client connection to notify...");
int core = xPortGetCoreID();
  Serial.print("Main code running on core ");
  Serial.println(core);

  // -- Create the FastLED show task
  xTaskCreatePinnedToCore(FastLEDshowTask, "FastLEDshowTask", 2048, NULL, 2, &FastLEDshowTaskHandle, FASTLED_SHOW_CORE);
}

void loop() {
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }

  if (deviceConnected) {


    // Fabricate some arbitrary junk for now...
    if (pattern == "rainbow") {
//      FastLED.show();
      fill_rainbow( leds, NUM_LEDS, gHue, 7);

    }
    if (pattern=="printaudio"){

      print_audio();
    }
    if (pattern == "off") {
//      FastLED.show();
      fill_solid(leds, NUM_LEDS, CRGB::Black);
    }

    if (pattern == "sinelon") {
//      FastLED.show();
      sinelon();
    }
    if (pattern == "bpm") {
//      FastLED.show();
      bpm();
    }
    if (pattern == "juggle") {
//      FastLED.show();
      radiate();
    }

    if (pattern == "pride") {
//      FastLED.show();
      pride();
    }
if (pattern == "color") {
      FastLED.show();
      fill_solid(leds, NUM_LEDS, CRGB(rgbR,rgbG,rgbB));
    }



  }
  
  if (!deviceConnected) {
    FastLED.show();
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }
  
FastLEDshowESP32();
}


