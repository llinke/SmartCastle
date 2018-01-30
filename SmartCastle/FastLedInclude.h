#define FASTLED_ESP8266_RAW_PIN_ORDER
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ESP8266_D1_PIN_ORDER

#define FASTLED_ALLOW_INTERRUPTS 0
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ESP8266_DMA
#include <FastLED.h>
FASTLED_USING_NAMESPACE

#define PIXEL_PIN D1
#define PIXEL_TYPE NEOPIXEL

#define PIXEL_COUNT 72

#define BUTTON_PIN_FX D5
#define BUTTON_PIN_COL D6

// Static size:
//extern struct CRGB leds[];
// Dynamic size:
extern struct CRGB *leds;

// Uncomment to enable printing debug messages.
#define DST_DEBUG
#define DEBUG_PRINTER Serial
#ifdef DST_DEBUG
#define DEBUG_PRINT(...)                  \
    {                                     \
        DEBUG_PRINTER.print(__VA_ARGS__); \
    }
#define DEBUG_PRINTLN(...)                  \
    {                                       \
        DEBUG_PRINTER.println(__VA_ARGS__); \
    }
#else
#define DEBUG_PRINT(...) \
    {                    \
    }
#define DEBUG_PRINTLN(...) \
    {                      \
    }
#endif