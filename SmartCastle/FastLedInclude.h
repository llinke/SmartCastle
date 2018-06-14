// **************************************************
// *** FastLED Neopixel
// **************************************************
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_ESP8266_RAW_PIN_ORDER

//#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ESP8266_DMA
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
FASTLED_USING_NAMESPACE

#define PIXEL_PIN D3
#define PIXEL_TYPE NEOPIXEL

#define PIXEL_COUNT 137

#define BUTTON_PIN_START_STOP D6
#define BUTTON_PIN_FX D5
#define BUTTON_PIN_COL D7

// Static size:
extern struct CRGB leds[];
// Dynamic size:
//extern struct CRGB *leds;
