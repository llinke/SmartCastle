#include "SerialDebug.h"
#include "FastLedInclude.h"
#include <Arduino.h>
//#include <ArduinoSTL.h>
#include <vector>
#include <map>

#define DUMP_PALETTE
#define BOOST_SAT_MIN 160
#define BOOST_SAT_INCR 64
#define BOOST_VAL_MIN 128
#define BOOST_VAL_INCR 64

// Helper macro
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

/*const*/ std::vector<String> ColorNames;
/*const*/ std::map<String, std::vector<CRGB>> ColorPalettes;

void PrintHex8(uint8_t data) // prints 8-bit data in hex with leading zeroes
{
    if (data < 0x10)
        Serial.print("0");
    DEBUG_PRINT(data, HEX);
}

void DumpPalette(std::vector<CRGB> pal)
{
    DEBUG_PRINT("Colors: ");
    CRGB rgbcolor;
    for (int c = 0; c < pal.size(); c++)
    {
        rgbcolor = pal.at(c);
        DEBUG_PRINT("0x");
        PrintHex8(rgbcolor.r);
        PrintHex8(rgbcolor.g);
        PrintHex8(rgbcolor.b);
        if (c != pal.size() - 1)
        {
            DEBUG_PRINT(",");
        }
    }
    DEBUG_PRINTLN(".");
}

void AddColorPalette(
    String palName,
    std::vector<CRGB> palColors,
    bool boostSaturation = true)
{
    DEBUG_PRINT("ColorPalettes: adding color palette '");
    DEBUG_PRINT(palName);
    DEBUG_PRINTLN("'.");
    ColorNames.push_back(palName);
    if (!boostSaturation)
    {
        ColorPalettes[palName] = palColors;
    }
    else
    {
        //rgb2hsv_approximate
        std::vector<CRGB> palColorsBoosted;
        CRGB rgbcolor;
        CHSV hsvcolor;
        int colCount = palColors.size();
        for (int c = 0; c < colCount; c++)
        {
            rgbcolor = palColors[c];
            hsvcolor = rgb2hsv_approximate(rgbcolor);
            if (hsvcolor.s < BOOST_SAT_MIN || hsvcolor.v < BOOST_VAL_MIN)
            {
                if (rgbcolor.r == rgbcolor.g && rgbcolor.g == rgbcolor.b)
                {
                    DEBUG_PRINT("   Keeping 0x");
                    PrintHex8(rgbcolor.r);
                    PrintHex8(rgbcolor.g);
                    PrintHex8(rgbcolor.b);
                    DEBUG_PRINTLN(", pure grey/white.");
                }
                else
                {
                    DEBUG_PRINT("   Boosting 0x");
                    PrintHex8(rgbcolor.r);
                    PrintHex8(rgbcolor.g);
                    PrintHex8(rgbcolor.b);
                    DEBUG_PRINT(",");
                    if (hsvcolor.s < BOOST_SAT_MIN)
                    {
                        DEBUG_PRINT(" S:");
                        DEBUG_PRINT(hsvcolor.s);
                        DEBUG_PRINT("->");
                        while (hsvcolor.s < BOOST_SAT_MIN)
                            hsvcolor.s += BOOST_SAT_INCR;
                        DEBUG_PRINT(hsvcolor.s);
                    }
                    if (hsvcolor.v < BOOST_VAL_MIN)
                    {
                        DEBUG_PRINT(" V:");
                        DEBUG_PRINT(hsvcolor.v);
                        DEBUG_PRINT("->");
                        while (hsvcolor.v < BOOST_VAL_MIN)
                            hsvcolor.v += BOOST_VAL_INCR;
                        DEBUG_PRINT(hsvcolor.v);
                    }
                    rgbcolor = CRGB(hsvcolor);
                    DEBUG_PRINT(", now 0x");
                    PrintHex8(rgbcolor.r);
                    PrintHex8(rgbcolor.g);
                    PrintHex8(rgbcolor.b);
                    DEBUG_PRINTLN(".");
                }
            }
            palColorsBoosted.push_back(rgbcolor);
        }
        ColorPalettes[palName] = palColorsBoosted;
    }
#ifdef DUMP_PALETTE
    DumpPalette(ColorPalettes[palName]);
#endif
}

std::vector<CRGB> SimplePaletteFromColor(
    CRGB baseCol, int variants = 1, uint8_t variantBy = 32)
{
    std::vector<CRGB> newPal;
    CRGB rgb = CRGB(baseCol);
    newPal.push_back(rgb);
    for (int i = 0; i < variants; i++)
    {
        CHSV hsv = rgb2hsv_approximate(rgb);
        hsv.h += 32;
        rgb = CRGB(hsv);
        newPal.push_back(rgb);
    }
    return newPal;
}

std::vector<CRGB> AdjacentPaletteFromHue(
    uint8_t hue, bool addCompl = false, uint8_t distance = 30)
{
    uint8_t dist8 = (255 * distance) / 360;
    std::vector<CRGB> newPal;
    CHSV hsvBase = CHSV(hue, 255, 255);
    newPal.push_back(CRGB(hsvBase)); // base color
    CHSV hsvNew = CHSV(hsvBase.h + dist8, 255, 255);
    newPal.push_back(CRGB(hsvNew)); // right adjacent
    if (addCompl)
    {
        hsvNew = CHSV(hsvBase.h - 128, 255, 255);
        newPal.push_back(CRGB(hsvNew)); // complementary
    }
    else
    {
        newPal.push_back(CRGB(hsvBase)); // base color
    }
    hsvNew = CHSV(hsvBase.h - dist8, 255, 255);
    newPal.push_back(CRGB(hsvNew)); // left adjacent
    return newPal;
}

std::vector<CRGB> TriadPaletteFromHue(
    uint8_t hue, bool addCompl = false, uint8_t distance = 30)
{
    return AdjacentPaletteFromHue(hue, distance + 90, addCompl);
}

std::vector<CRGB> AnalogousPaletteFromHue(
    uint8_t hue, bool addCompl = false, uint8_t distance = 15)
{
    uint8_t dist8 = (255 * distance) / 360;
    std::vector<CRGB> newPal;
    CHSV hsvBase = CHSV(hue, 255, 255);
    newPal.push_back(CRGB(hsvBase)); // base color
    CHSV hsvNew = CHSV(hsvBase.h - dist8, 255, 255);
    newPal.push_back(CRGB(hsvNew)); // first left analogous
    hsvNew = CHSV(hsvBase.h - 2 * dist8, 255, 255);
    newPal.push_back(CRGB(hsvNew)); // second left analogous
    if (addCompl)
    {
        hsvNew = CHSV(hsvBase.h - 128, 255, 255);
        newPal.push_back(CRGB(hsvNew)); // complementary
    }
    else
    {
        newPal.push_back(CRGB(hsvBase)); // base color
    }
    hsvNew = CHSV(hsvBase.h + 2 * dist8, 255, 255);
    newPal.push_back(CRGB(hsvNew)); // second right analogous
    hsvNew = CHSV(hsvBase.h + dist8, 255, 255);
    newPal.push_back(CRGB(hsvNew)); // first right analogous
    return newPal;
}

void InitColorPalettes()
{
    DEBUG_PRINTLN("Initializing color palettes...");

    uint8_t hues[] = {
        HUE_PINK,
        HUE_AQUA,
        HUE_ORANGE,
        HUE_PURPLE,
        HUE_GREEN,
        HUE_RED,
        HUE_BLUE,
        HUE_YELLOW};
    for (int h = 0; h < ARRAY_SIZE(hues); h++)
    {
        uint8_t hue = hues[h];

        /*
    uint8_t baseHue = HUE_PINK; //baseHsv.h;
    uint8_t palCount = 8;
    uint8_t hueDiff = 256 / palCount;
    for (int c = 0; c < palCount; c++)
    {
        uint8_t hue = baseHue + c * hueDiff;
*/
        //AddColorPalette("Adjacent_" + String(hue), AdjacentPaletteFromHue(hue), false);
        //AddColorPalette("Triad_" + String(hue), TriadPaletteFromHue(hue), false);
        AddColorPalette("Analogous_" + String(hue), AnalogousPaletteFromHue(hue), false);
        //AddColorPalette("AdjacentCompl_" + String(hue), AdjacentPaletteFromHue(hue, true), false);
        //AddColorPalette("TriadCompl_" + String(hue), TriadPaletteFromHue(hue, true), false);
        AddColorPalette("AnalogousCompl_" + String(hue), AnalogousPaletteFromHue(hue, true), false);
    }

    // Add some more pretty palettes :-)
    AddColorPalette("Rainbow", {CRGB(0xFF0000), CRGB(0xD52A00), CRGB(0xAB5500), CRGB(0xAB7F00), CRGB(0xABAB00), CRGB(0x56D500), CRGB(0x00FF00), CRGB(0x00D52A), CRGB(0x00AB55), CRGB(0x0056AA), CRGB(0x0000FF), CRGB(0x2A00D5), CRGB(0x5500AB), CRGB(0x7F0081), CRGB(0xAB0055), CRGB(0xD5002B)});
    AddColorPalette("Party", {CRGB(0x5500AB), CRGB(0x84007C), CRGB(0xB5004B), CRGB(0xE5001B), CRGB(0xE81700), CRGB(0xB84700), CRGB(0xAB7700), CRGB(0xABAB00), CRGB(0xAB5500), CRGB(0xDD2200), CRGB(0xF2000E), CRGB(0xC2003E), CRGB(0x8F0071), CRGB(0x5F00A1), CRGB(0x2F00D0), CRGB(0x0007F9)});
    AddColorPalette("Pop", {CRGB(0xAAFF00), CRGB(0xFFAA00), CRGB(0xFF00AA), CRGB(0xAA00FF), CRGB(0x00AAFF)});

    // Colourlovers Dynamic Colors
    AddColorPalette("hot pink", SimplePaletteFromColor(CRGB(0xFF0066)));
    AddColorPalette("immensity1", SimplePaletteFromColor(CRGB(0x6F9DD1)));
    AddColorPalette("immensity3", SimplePaletteFromColor(CRGB(0x665467)));

    // Colourlovers Palettes
    AddColorPalette("Blues", {CRGB(0x4AD8DB), CRGB(0xD30081), CRGB(0x780072), CRGB(0x1A46F4), CRGB(0x02155B)});
    AddColorPalette("DayAndNight", {CRGB(0x00C17D), CRGB(0x00E999), CRGB(0xEF9000), CRGB(0xEDEA01), CRGB(0xFFF439)});
    AddColorPalette("Frosty", {CRGB(0x645067), CRGB(0x0095BD), CRGB(0x3CCB91), CRGB(0xA7FC98), CRGB(0xE1FFD9)});
    AddColorPalette("Warm", {/*CRGB(0xFCDEBB),*/ CRGB(0xFAC589), CRGB(0xD94709), CRGB(0xA80022), CRGB(0x8C001C)});
    AddColorPalette("Pastel1", {CRGB(0x3FB8AF), CRGB(0x7FC7AF), CRGB(0xDAD8A7), CRGB(0xFF9E9D), CRGB(0xFF3D7F)});  // Exclude?
    AddColorPalette("Pinkish2", {CRGB(0xEB2A73), CRGB(0xFF619A), CRGB(0xFF94BB), CRGB(0xFFC7DB), CRGB(0xFFE8F0)}); // Exclude?
    AddColorPalette("Girlish1", {CRGB(0x009999), CRGB(0x8E0039), CRGB(0xDB0058), CRGB(0xFF7400), CRGB(0xFFC700)});
    AddColorPalette("NightAndDay1", {CRGB(0x029DAF), CRGB(0xE5D599), CRGB(0xFFC219), CRGB(0xF07C19), CRGB(0xE32551)});
    AddColorPalette("Unicorn1", {CRGB(0xEEF6D0), CRGB(0xBF7DB7), CRGB(0x40F9E8), CRGB(0x49C1BB), CRGB(0x85038C)}); // Exclude?
    AddColorPalette("Nature2", {CRGB(0xBF2A23), CRGB(0xA6AD3C), CRGB(0xF0CE4E), CRGB(0xCF872E), CRGB(0x8A211D)});
    AddColorPalette("Nature3", {CRGB(0xFCDEBB), CRGB(0xFAC589), CRGB(0xD94709), CRGB(0xA80022), CRGB(0x8C001C)});
    AddColorPalette("Ocean2", {CRGB(0xCFF09E), CRGB(0xA8DBA8), CRGB(0x79BD9A), CRGB(0x3B8686), CRGB(0x0B486B)}); // Exclude?

    // Christmas
    AddColorPalette("Christmas1", {CRGB(0x941F1F), CRGB(0xCE6B5D), CRGB(0xFFEFB9), CRGB(0x7B9971), CRGB(0x34502B), CRGB(0x7A7E5F)}); // Exclude?
    AddColorPalette("Christmas2", {CRGB(0x9E3333), CRGB(0xAB6060), CRGB(0xD4D1A5), CRGB(0x7BBD82), CRGB(0x3D9949), CRGB(0x7A7E5F)}); // Exclude?
    AddColorPalette("Christmas4", {/*CRGB::Black,*/ CRGB::DarkViolet, CRGB::DarkRed, /*CRGB::Black,*/ CRGB::DarkOrange, CRGB::DarkGreen});
    AddColorPalette("Christmas8", {/*CRGB::Black,*/ CRGB::DarkRed, CRGB::DarkOrange, CRGB::DarkRed, CRGB::DarkGreen});

    // Fires
    AddColorPalette("CozyFire3", {CRGB::DarkRed, CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::DarkOrange});
}
