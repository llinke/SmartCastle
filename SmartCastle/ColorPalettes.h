#include "SerialDebug.h"
#include "FastLedInclude.h"
#include <Arduino.h>
//#include <ArduinoSTL.h>
#include <vector>
#include <map>

#define BOOST_SAT_MIN 160
#define BOOST_SAT_INCR 64
#define BOOST_VAL_MIN 128
#define BOOST_VAL_INCR 64

#define HALFFAIRY ((CRGB::FairyLight & 0xFEFEFE) / 2)
#define QUARTERFAIRY ((CRGB::FairyLight & 0xFCFCFC) / 4)

/*const*/ std::vector<String> ColorNames;
/*const*/ std::map<String, std::vector<CRGB>> ColorPalettes;

void PrintHex8(uint8_t data) // prints 8-bit data in hex with leading zeroes
{
    if (data < 0x10)
        Serial.print("0");
    DEBUG_PRINT(data, HEX);
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

void InitColorPalettes()
{
    AddColorPalette("hot pink", SimplePaletteFromColor(CRGB(0xFF0066)));
    AddColorPalette("immensity1", SimplePaletteFromColor(CRGB(0x6F9DD1)));
    AddColorPalette("immensity2", SimplePaletteFromColor(CRGB(0x647497)));
    AddColorPalette("immensity3", SimplePaletteFromColor(CRGB(0x665467)));
    AddColorPalette("immensity4", SimplePaletteFromColor(CRGB(0xB06A67)));
    AddColorPalette("immensity5", SimplePaletteFromColor(CRGB(0xF16260)));
    AddColorPalette("dutch teal", SimplePaletteFromColor(CRGB(0x1693A5)));
    AddColorPalette("heart of gold", SimplePaletteFromColor(CRGB(0xFBB829)));
    AddColorPalette("heart of gold 2", SimplePaletteFromColor(CRGB(0xFBB829)), false);
    AddColorPalette("hot p wow ink", SimplePaletteFromColor(CRGB(0xC61AE9)));
    AddColorPalette("blue bb", SimplePaletteFromColor(CRGB(0x93A5CC)));
    AddColorPalette("Cascadian Blue", SimplePaletteFromColor(CRGB(0x234B79)));
    AddColorPalette("Outland Terrain", SimplePaletteFromColor(CRGB(0x318D6B)));
    AddColorPalette("Acorus", SimplePaletteFromColor(CRGB(0x36A44A)));
    AddColorPalette("Summer Sky", SimplePaletteFromColor(CRGB(0x37A2BF)));
    AddColorPalette("Carry me tonight", SimplePaletteFromColor(CRGB(0xC623A9)));
    return;
    /*
    AddColorPalette("DeepPinkAnalogous", {CRGB(0xff1493), CRGB(0xff1445), CRGB(0xff14e1), CRGB(0xff7ac2), CRGB(0x898989), CRGB(0xffffff)});
    AddColorPalette("DeepPinkMonocromatic", {CRGB(0xff1493), CRGB(0x7a0042), CRGB(0xffadd9), CRGB(0x898989)});
    AddColorPalette("DeepPinkComplementary", {CRGB(0xff1493), CRGB(0xff1445), CRGB(0x14ff80)});
    AddColorPalette("DeepPinkSplit Complementary", {CRGB(0xff1493), CRGB(0xff1445), CRGB(0x1eff14), CRGB(0x14fff5), CRGB(0x898989), CRGB(0xffffff)});
    AddColorPalette("DeepPinkTriadic", {CRGB(0xff1493), CRGB(0x93ff14), CRGB(0x1493ff)});
    AddColorPalette("DarkTurquoiseAnalogous", {CRGB(0x00ced1), CRGB(0x0088d1), CRGB(0x00d18e), CRGB(0x38fcff), CRGB(0x686868), CRGB(0x000000)});
    AddColorPalette("DarkTurquoiseMonocromatic", {CRGB(0x00ced1), CRGB(0x003738), CRGB(0x6bfdff), CRGB(0x686868)});
    AddColorPalette("DarkTurquoiseComplementary", {CRGB(0x00ced1), CRGB(0x0088d1), CRGB(0xd10300)});
    AddColorPalette("DarkTurquoiseSplit Complementary", {CRGB(0x00ced1), CRGB(0x0088d1), CRGB(0xd10066), CRGB(0xd16b00), CRGB(0x686868), CRGB(0x000000)});
    AddColorPalette("DarkTurquoiseTriadic", {CRGB(0x00ced1), CRGB(0xd100ce), CRGB(0xced100)});
    AddColorPalette("DarkVioletAnalogous", {CRGB(0x9400d3), CRGB(0xd300cc), CRGB(0x4e00d3), CRGB(0xc43aff), CRGB(0x696969), CRGB(0x000000)});
    AddColorPalette("DarkVioletMonocromatic", {CRGB(0x9400d3), CRGB(0x29003a), CRGB(0xd36dff), CRGB(0x696969)});
    AddColorPalette("DarkVioletComplementary", {CRGB(0x9400d3), CRGB(0xd300cc), CRGB(0x3fd300)});
    AddColorPalette("DarkVioletSplit Complementary", {CRGB(0x9400d3), CRGB(0xd300cc), CRGB(0xa8d300), CRGB(0x00d32b), CRGB(0x696969), CRGB(0x000000)});
    AddColorPalette("DarkVioletTriadic", {CRGB(0x9400d3), CRGB(0xd39400), CRGB(0x00d394)});
    */

    AddColorPalette("Party", {CRGB(0x5500AB), CRGB(0x84007C), CRGB(0xB5004B), CRGB(0xE5001B), CRGB(0xE81700), CRGB(0xB84700), CRGB(0xAB7700), CRGB(0xABAB00), CRGB(0xAB5500), CRGB(0xDD2200), CRGB(0xF2000E), CRGB(0xC2003E), CRGB(0x8F0071), CRGB(0x5F00A1), CRGB(0x2F00D0), CRGB(0x0007F9)});
    AddColorPalette("Blues", {CRGB(0x4AD8DB), CRGB(0xD30081), CRGB(0x780072), CRGB(0x1A46F4), CRGB(0x02155B)});
    AddColorPalette("DayAndNight", {CRGB(0x00C17D), CRGB(0x00E999), CRGB(0xEF9000), CRGB(0xEDEA01), CRGB(0xFFF439)});
    AddColorPalette("Frosty", {CRGB(0x645067), CRGB(0x0095BD), CRGB(0x3CCB91), CRGB(0xA7FC98), CRGB(0xE1FFD9)});
    AddColorPalette("Warm", {/*CRGB(0xFCDEBB),*/ CRGB(0xFAC589), CRGB(0xD94709), CRGB(0xA80022), CRGB(0x8C001C)});
    AddColorPalette("Pastel1", {CRGB(0x3FB8AF), CRGB(0x7FC7AF), CRGB(0xDAD8A7), CRGB(0xFF9E9D), CRGB(0xFF3D7F)});  // Exclude?
    AddColorPalette("Pastel2", {CRGB(0x93DFB8), CRGB(0xFFC8BA), CRGB(0xE3AAD6), CRGB(0xB5D8EB), CRGB(0xFFBDD8)});  // Exclude?
    AddColorPalette("Pinkish1", {CRGB(0xCD0869), CRGB(0xF24685), CRGB(0xEC509D), CRGB(0xF487AF), CRGB(0xF8BED3)}); // Exclude?
    AddColorPalette("Pinkish2", {CRGB(0xEB2A73), CRGB(0xFF619A), CRGB(0xFF94BB), CRGB(0xFFC7DB), CRGB(0xFFE8F0)}); // Exclude?
    AddColorPalette("Pop", {CRGB(0xAAFF00), CRGB(0xFFAA00), CRGB(0xFF00AA), CRGB(0xAA00FF), CRGB(0x00AAFF)});
    AddColorPalette("Rainbow", {CRGB(0xFF0000), CRGB(0xD52A00), CRGB(0xAB5500), CRGB(0xAB7F00), CRGB(0xABAB00), CRGB(0x56D500), CRGB(0x00FF00), CRGB(0x00D52A), CRGB(0x00AB55), CRGB(0x0056AA), CRGB(0x0000FF), CRGB(0x2A00D5), CRGB(0x5500AB), CRGB(0x7F0081), CRGB(0xAB0055), CRGB(0xD5002B)});
    AddColorPalette("DarkRainbow", {CRGB(0x482344), CRGB(0x2B5166), CRGB(0x429867), CRGB(0xFAB243), CRGB(0xE02130)}); // Exclude?
    AddColorPalette("Girlish1", {CRGB(0x009999), CRGB(0x8E0039), CRGB(0xDB0058), CRGB(0xFF7400), CRGB(0xFFC700)});
    AddColorPalette("NightAndDay1", {CRGB(0x029DAF), CRGB(0xE5D599), CRGB(0xFFC219), CRGB(0xF07C19), CRGB(0xE32551)});
    AddColorPalette("NightAndDay2", {CRGB(0x75386D), CRGB(0x418592), CRGB(0x2DC0A7), CRGB(0x7EE368), CRGB(0xCEFD47)});
    AddColorPalette("Unicorn1", {CRGB(0xEEF6D0), CRGB(0xBF7DB7), CRGB(0x40F9E8), CRGB(0x49C1BB), CRGB(0x85038C)}); // Exclude?
    AddColorPalette("Unicorn2", {CRGB(0xA697E8), CRGB(0xD2ABEA), CRGB(0xFFBFEB), CRGB(0xFFDFDC), CRGB(0xFFFFCC)}); // Exclude?
    AddColorPalette("Nature1", {CRGB(0xF6C918), CRGB(0xADAF34), CRGB(0x76B86E), CRGB(0x09A590), CRGB(0x3F2542)});  // Exclude?
    AddColorPalette("Nature2", {CRGB(0xBF2A23), CRGB(0xA6AD3C), CRGB(0xF0CE4E), CRGB(0xCF872E), CRGB(0x8A211D)});
    AddColorPalette("Nature3", {CRGB(0xFCDEBB), CRGB(0xFAC589), CRGB(0xD94709), CRGB(0xA80022), CRGB(0x8C001C)});
    AddColorPalette("Nature4", {CRGB(0x410645), CRGB(0x7B0565), CRGB(0xF4630C), CRGB(0xFF9C1C), CRGB(0xFDDE14)});
    AddColorPalette("Ocean1", {CRGB(0x69D2E7), CRGB(0xA7DBD8), CRGB(0xE0E4CC), CRGB(0xF38630), CRGB(0xFA6900)}); // Exclude?
    AddColorPalette("Ocean2", {CRGB(0xCFF09E), CRGB(0xA8DBA8), CRGB(0x79BD9A), CRGB(0x3B8686), CRGB(0x0B486B)}); // Exclude?
    AddColorPalette("Ocean3", {CRGB(0xF2E8C4), CRGB(0x98D9B6), CRGB(0x3EC9A7), CRGB(0x2B879E), CRGB(0x616668)}); // Exclude?
    AddColorPalette("Ocean4", {CRGB(0x69D2E7), CRGB(0xA7DBD8), CRGB(0xE0E4CC), CRGB(0xF38630), CRGB(0xFA6900)});
    AddColorPalette("Castle1", {CRGB(0x996C97), CRGB(0x8D8EA3), CRGB(0x99A8AD), CRGB(0xEFBE9E), CRGB(0xF6DFA9)}); // Exclude?
    AddColorPalette("Castle2", {CRGB(0xF98367), CRGB(0x9BB1C6), CRGB(0xE4E2E3), CRGB(0xCDBFB4), CRGB(0x506678)}); // Exclude?
    AddColorPalette("Fresh", {CRGB(0xF35C9F), CRGB(0xF3F3F3), CRGB(0x22ADC2), CRGB(0x736F66)});                   // Exclude?

    // Christmas
    AddColorPalette("Christmas1", {CRGB(0x941F1F), CRGB(0xCE6B5D), CRGB(0xFFEFB9), CRGB(0x7B9971), CRGB(0x34502B), CRGB(0x7A7E5F)}); // Exclude?
    AddColorPalette("Christmas2", {CRGB(0x9E3333), CRGB(0xAB6060), CRGB(0xD4D1A5), CRGB(0x7BBD82), CRGB(0x3D9949), CRGB(0x7A7E5F)}); // Exclude?
    AddColorPalette("Christmas3", {/*CRGB::Black,*/ CRGB::DarkRed, CRGB::DarkOrange, CRGB::DarkGreen});
    AddColorPalette("Christmas4", {/*CRGB::Black,*/ CRGB::DarkViolet, CRGB::DarkRed, /*CRGB::Black,*/ CRGB::DarkOrange, CRGB::DarkGreen});
    AddColorPalette("Christmas5", {/*CRGB::Black,*/ CRGB::DarkViolet, CRGB::DarkRed, /*CRGB::Black,*/ CRGB::DarkCyan, CRGB::DarkBlue, /*CRGB::Black,*/ CRGB::DarkOrange, CRGB::DarkGreen});
    AddColorPalette("Christmas6", {/*CRGB::Black,*/ CRGB::DarkRed, CRGB::Gray, CRGB::DarkRed, CRGB::DarkGreen, CRGB::Gray, CRGB::DarkGreen});
    AddColorPalette("Christmas7", {/*CRGB::Black,*/ CRGB::CRGB::FairyLight, HALFFAIRY, CRGB::CRGB::FairyLight, QUARTERFAIRY, CRGB::CRGB::FairyLight}); // Exclude?
    AddColorPalette("Christmas8", {/*CRGB::Black,*/ CRGB::DarkRed, CRGB::DarkOrange, CRGB::DarkRed, CRGB::DarkGreen});

    // Fires
    AddColorPalette("CozyFire1", {CRGB(0x330A04), CRGB(0x6C1305), CRGB(0xB32900), CRGB(0xDE8531), CRGB(0xEFAC41)});
    AddColorPalette("CozyFire2", {CRGB(0x992C61), CRGB(0xFF244A), CRGB(0xFF631B), CRGB(0xFFBF1E), CRGB(0xFFF3CF)}); // Exclude?
    AddColorPalette("CozyFire3", {CRGB::DarkRed, CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::DarkOrange});
    AddColorPalette("GrecianFire", {CRGB(0x604860), CRGB(0x784860), CRGB(0xA86060), CRGB(0xC07860), CRGB(0xF0A848)}); // Exclude?
    // This first palette is the basic 'black body radiation' colors,
    // which run from black to red to bright yellow to white.
    //HeatColors_p,
    AddColorPalette("HeatColors", {CRGB(0x000000), CRGB(0x330000), CRGB(0x660000), CRGB(0x990000), CRGB(0xCC0000), CRGB(0xFF0000), CRGB(0xFF3300), CRGB(0xFF6600), CRGB(0xFF9900), CRGB(0xFFCC00), CRGB(0xFFFF00), CRGB(0xFFFF33), CRGB(0xFFFF66), CRGB(0xFFFF99), CRGB(0xFFFFCC), CRGB(0xFFFFFF)});
    // First, a gradient from black to red to yellow to white -- similar to HeatColors_p
    AddColorPalette("HeatColors1", {/*CRGB::Black,*/ CRGB::Red, CRGB::Yellow, CRGB::White});
    // Second, this palette is like the heat colors, but blue/aqua instead of red/yellow
    AddColorPalette("HeatColors2", {CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White}); // Exclude?
    // Third, here's a simpler, three-step gradient, from black to red to white
    AddColorPalette("HeatColors3", {CRGB::Black, CRGB::Red, CRGB::White}); // Exclude?
    // Fourth, here's a warmer gradient, from black to yellow
    AddColorPalette("HeatColors4", {/*CRGB::Black,*/ CRGB::Red, CRGB::Orange, CRGB::Yellow});
    // Fifth, here's a second warmer gradient, from black to blue
    AddColorPalette("HeatColors5", {/*CRGB::Black,*/ CRGB::Red, CRGB::Orange, CRGB::Blue});
    // Sixth, here's a third warmer gradient, from darkred to yellow
    AddColorPalette("HeatColors6", {CRGB::DarkRed, CRGB::Red, CRGB::Orange, CRGB::Yellow});
}
