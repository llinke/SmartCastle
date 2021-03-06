/*
* Project SmartCastle
* Description:
* Author: Lutz Linke
* Date: 2018/02/14
*/
#include <Arduino.h>

// **************************************************
// *** Compiler Flags
// **************************************************
// --- WiFi -----------------------------------------
//#define INCLUDE_WIFI
// --- DEBUG ----------------------------------------
#define ENABLE_SERIAL_DEBUG
//#define DEBUG_LOOP
// --- Demo --------- -------------------------------
#define PLAY_DEMO true
#define COMET_DEMO
// --- Rooms / Groups -------------------------------
#define ADD_GROUP_FOR_ALL_ROOMS
#define GROUP_FOR_ALL_ROOMS_IS_LAST
#define GROUP_FOR_ALL_ROOMS_IS_ACTIVE
//#define BUTTON_TOGGLE_GROUP_ON_OFF
// --- FX/Colors ------------------------------------
//#define START_WITH_RANDOM_COLOR_PALETTE
//#define ENABLE_RANDOM_FX
//#define ENABLE_RANDOM_COL
//#define DO_NOT_START_FX_ON_INIT

// **************************************************

// **************************************************
// *** Includes
// **************************************************
#include "SerialDebug.h"
//#include <ArduinoSTL.h>
#include <vector>
#include <map>
#include <string.h>

#include "I2CInclude.h"
#include "FastLedInclude.h"
#include "ColorPalettes.h"
#include "NeoGroup.cpp"

#ifdef INCLUDE_WIFI
#include <ESP8266WiFi.h>	  //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>		  //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h> //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>	  //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#endif

// **************************************************
// *** Variable and Constamts  Declarations
// **************************************************
#ifdef INCLUDE_WIFI
const String wifiApName = "AP_SmartCastle";
const int ConfigureAPTimeout = 10;
#endif

const uint8_t globalBrightness = 128;
const uint8_t statusBrightness = 64;
const uint8_t activeGroupBrightnessCycleSpeed = 3;

const int buttonLockDuration = 200; // ignore buttons for X ms, prevent prelling
static volatile bool buttonsLocked = false;
static volatile int buttonsLockedAt = 0;

static volatile bool buttonPressedOnI2C = false;
static volatile bool roomWasChanged = false;

#ifdef ADD_GROUP_FOR_ALL_ROOMS
// Real room sizes
const std::vector<int> groupRoomSizes = {13, 26, 12, 33, 14, 12, 19};
#else
// Test 1: equally sized rooms
//const std::vector<int> groupRoomSizes = {16, 16, 16, 16, 16, 16, 16, 16};
// Test 2: differently sized rooms
//const std::vector<int> groupRoomSizes = {4, 8, 12, 16, 16, 20, 24, 28};
const std::vector<int> groupRoomSizes = {8, 24, 10, 22, 12, 20, 14, 18};
#endif

const std::vector<int> mapRoomToStatusLed = {0, 6, 1, 5, 4, 3, 2, 7};
const std::vector<int> mapI2cButtonToRoom = {4, 3, 1, 7, 0, 2, 6, 5};

int groupRoomTotalSize = 0;
int groupRoomCount = 0;
int groupRoomOffset = 1;
int groupNrAllLeds = 0;
int groupNrAllRooms = -1;
int groupNrStatus = 1;
const int groupStatusCount = 8;
int activeGrpNr = 0;

// Static size
struct CRGB leds[PIXEL_COUNT];
// Dynamic size:
//struct CRGB *leds = NULL;
bool ledsInitialized = false;
bool ledsStarted = false;
// 0: Wave, 1: Dynamic Wave, 2: Noise, 3: Confetti, 4: Fade, 5: Comet, 6: Orbit, 7: Fill
const uint8_t fxNrWave = 0;
const uint8_t fxNrDynamicWave = 1;
const uint8_t fxNrNoise = 2;
const uint8_t fxNrConfetti = 3;
const uint8_t fxNrFade = 4;
const uint8_t fxNrComet = 5;
const uint8_t fxNrOrbit = 6;
const uint8_t fxNrFill = 7;
const int maxFxNr = 6;
const int defaultFxNr = 1;
const int defaultFxNrAll = 3;
std::vector<int> currFxNr;
int maxColNr = 1; // will be dynamically assigned once palettes are generated
const int defaultColNr = 2;
const int defaultColNrAll = 1;
std::vector<int> currColNr;
const int defaultFps = 50; //25;
std::vector<int> currFps;
const int defaultGlitter = 0; //32;
std::vector<int> currGlitter;

//std::vector<NeoGroup *> neoGroups;
std::vector<NeoGroup> neoGroups;

static volatile bool buttonStartStopPressed = false;
static volatile bool buttonFxPressed = false;
static volatile bool buttonColPressed = false;
static volatile bool bothButtonsPressed = false;

volatile bool updateOledRequired = false;

// **************************************************
// *** Helper methods
// **************************************************
#pragma region Helper Methods
#ifdef INCLUDE_WIFI
bool InitWifi(bool useWifiCfgTimeout = true, bool forceReconnect = false)
{
	DEBUG_PRINTLN("WIFI ------------------------------------------------------");
	if (!forceReconnect && WiFi.status() == WL_CONNECTED)
	{
		DEBUG_PRINTLN("WiFi: already connected...");
		return true; // Is already connected...
	}

	if (ledsInitialized)
	{
		FastLED.clear(true);
		fill_solid(leds, PIXEL_COUNT, CRGB::Blue);
		FastLED.show();
	}
	delay(2500);
	//WiFiManager
	WiFiManager wifiManager;
	if (forceReconnect)
	{
		wifiManager.resetSettings();
	}
	//wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
	//fetches ssid and pass from eeprom and tries to connect
	//if it does not connect it starts an access point with the specified name
	//here  "AutoConnectAP" and goes into a blocking loop awaiting configuration
	DEBUG_PRINTLN("WiFi Manager trying to connect...");
	if (useWifiCfgTimeout)
	{
		DEBUG_PRINT("You have ");
		DEBUG_PRINT(ConfigureAPTimeout);
		DEBUG_PRINTLN(" seconds for configuration if required.");
		wifiManager.setConfigPortalTimeout(ConfigureAPTimeout);
	}
	bool connected = wifiManager.autoConnect(wifiApName.c_str());
	//or use this for auto generated name ESP + ChipID
	//wifiManager.autoConnect();
	//if you get here you have connected to the WiFi
	if (ledsInitialized)
	{
		fill_solid(leds, PIXEL_COUNT, connected ? CRGB::Green : CRGB::Red);
		FastLED.show();
	}
	if (connected)
	{
		DEBUG_PRINTLN("Wifi is connected...yay!!!");
	}
	else
	{
		DEBUG_PRINTLN("!!! WIFI NOT CONNECTED !!!");
	}
	delay(5000);

	return connected;
}
#endif

// [FastLED Helper methods]
int initStrip(bool doStart = false, bool playDemo = true)
{
	if (ledsInitialized)
	{
		return doStart ? startStrip() : PIXEL_COUNT;
	}

	DEBUG_PRINTLN("LEDStrip --------------------------------------------------");
	/*	
	DEBUG_PRINTLN("Allocating memory for LED strip data.");
	leds = (struct CRGB *)malloc(PIXEL_COUNT * sizeof(struct CRGB));
	*/
	DEBUG_PRINTLN("Assigning LEDs to FastLED.");
	FastLED.addLeds<PIXEL_TYPE, PIXEL_PIN>(leds, PIXEL_COUNT);
	//FastLED.setMaxPowerInVoltsAndMilliamps(5,3000);
	FastLED.setBrightness(globalBrightness);
	FastLED.setDither(0);
	FastLED.clear(true);
	FastLED.show();

	ledsInitialized = true;

	if (playDemo)
	{
		DEBUG_PRINT("Playing demo effect...");
#ifdef COMET_DEMO
		CRGBPalette16 colorPalette = NeoGroup::GenerateRGBPalette(ColorPalettes.find("Regenbogen")->second);
		for (int dot = 0; dot < 256; dot++)
		{
			// Comet effect :-)
			fadeToBlackBy(leds, PIXEL_COUNT, 8);
			int variant = (PIXEL_COUNT / 16);
			int pos = ease8InOutQuad(dot) + random(0 - variant, 0 + variant);
			pos = (pos * PIXEL_COUNT) / 256;
			pos = constrain(pos, 0, PIXEL_COUNT);
			//DEBUG_PRINT("Setting pixel #");
			//DEBUG_PRINTLN(pos);
			int bright = random(64, 255);

			/*
			CRGB color = CHSV(random8(), 255, 255);
			nblend(leds[pos], color, 128);
			*/
			uint8_t colpos = dot + random8(16) - 8;
			nblend(leds[pos], ColorFromPalette(colorPalette, colpos, bright), 128);

			FastLED.show();
			delay(10);
			DEBUG_PRINT(".");
		}
		DEBUG_PRINTLN("DONE");
#else
		for (int dot = 0; dot < PIXEL_COUNT; dot++)
		{
			leds[dot] = CHSV(random8(), 255, 255);
			FastLED.show();
			delay(10);
			DEBUG_PRINT(".");
		}
		DEBUG_PRINTLN("DONE");
		delay(500);
#endif

		DEBUG_PRINTLN("Fading away demo effect.");
		for (int fade = 0; fade < 20; fade++)
		{
			fadeToBlackBy(leds, PIXEL_COUNT, 20);
			FastLED.show();
			delay(50);
		}
		DEBUG_PRINTLN("Clearing LEDs.");
		FastLED.clear(true);
		FastLED.show();
	}

#ifdef INCLUDE_WIFI
	InitWifi();
#endif

	DEBUG_PRINTLN("Calculating groups.");
	groupRoomCount = groupRoomSizes.size();
#ifdef ADD_GROUP_FOR_ALL_ROOMS
	groupRoomCount++;
#endif
	groupRoomTotalSize = 0;
	for (int i = 0; i < groupRoomSizes.size(); i++)
	{
		groupRoomTotalSize += groupRoomSizes[i];
	}

	DEBUG_PRINTLN("Adding special groups.");
	neoGroups.clear();
	// Group 0: all LEDs
	addGroup("All LEDs' group", 0, PIXEL_COUNT, 0);
	groupNrAllLeds = neoGroups.size() - 1;
	// Group 1: Status LEDs
	addGroup("Status LEDs' group", 0, groupStatusCount, 0);
	groupNrStatus = neoGroups.size() - 1;
#ifdef ADD_GROUP_FOR_ALL_ROOMS
#ifndef GROUP_FOR_ALL_ROOMS_IS_LAST
	addGroup("Schloss", groupStatusCount, groupRoomTotalSize, 0);
	groupNrAllRooms = neoGroups.size() - 1;
#endif
#endif
	groupRoomOffset = groupNrAllRooms >= 0 ? groupNrAllRooms : neoGroups.size();
	activeGrpNr = 0;

	DEBUG_PRINT("Adding ");
	DEBUG_PRINT(groupRoomCount);
	DEBUG_PRINT(" groups for rooms.");
	int nextGroupStart = groupStatusCount; // Start after status LEDs
	for (int i = 0; i < groupRoomSizes.size(); i++)
	{
		String roomName = "Raum " + String(i + 1);
		addGroup(roomName, nextGroupStart, groupRoomSizes[i], 0);
		nextGroupStart += groupRoomSizes[i];
	}
#ifdef ADD_GROUP_FOR_ALL_ROOMS
#ifdef GROUP_FOR_ALL_ROOMS_IS_LAST
	addGroup("Schloss", groupStatusCount, groupRoomTotalSize, 0);
	groupNrAllRooms = neoGroups.size() - 1;
	activeGrpNr = groupRoomCount - 1;
#endif
#endif

	return doStart ? startStrip() : PIXEL_COUNT;
}

int startStrip()
{
	if (!ledsInitialized)
		return -1;

	ledsStarted = true;
	return PIXEL_COUNT;
}

int stopStrip()
{
	ledsStarted = false;

	for (int i = 2; i < neoGroups.size(); i++)
	{
		NeoGroup *neoGroup = &(neoGroups.at(i));
		neoGroup->Stop(true);
	}

	FastLED.clear(true);
	FastLED.show();

	return 0;
}

int addGroup(String grpId, int ledFirst, int ledCount, int ledOffset)
{
	if ((ledFirst >= PIXEL_COUNT) ||
		(ledCount <= 0) ||
		(ledFirst + ledCount) > PIXEL_COUNT)
		return -((((3 * 1000) + ledFirst) * 1000) + ledCount); // Invalid parameter

	DEBUG_PRINT("Adding group '");
	DEBUG_PRINT(grpId);
	DEBUG_PRINT("' at ");
	DEBUG_PRINT(ledFirst);
	DEBUG_PRINT(" with size ");
	DEBUG_PRINTLN(ledCount);

	// V1: new NeoGroup
	//NeoGroup *newGroup = new NeoGroup(grpId, ledFirst, ledCount, ledOffset);
	//neoGroups.push_back(newGroup);
	// V2: NeoGroup w/o new
	NeoGroup newGroup = NeoGroup(grpId, ledFirst, ledCount, ledOffset);
	neoGroups.push_back(newGroup);

	currFxNr.push_back(defaultFxNr);
	currColNr.push_back(defaultColNr);
	currFps.push_back(defaultFps);
	currGlitter.push_back(defaultGlitter);

	DEBUG_PRINT("GroupCount: ");
	DEBUG_PRINTLN(neoGroups.size());

	return neoGroups.size();
}

bool isGroupActive(int grpNr)
{
	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	return neoGroup->Active;
}

bool isGroupFadingOut(int grpNr)
{
	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	return !(neoGroup->Active) && (neoGroup->IsFadingOut());
}

int startGroup(int grpNr, bool runOnlyOnce = false)
{
	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	neoGroup->Start(runOnlyOnce);

	updateOledRequired = true;

	return grpNr;
}

int stopGroup(int grpNr, bool stopNow = false)
{
	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	neoGroup->Stop(stopNow);

	updateOledRequired = true;

	return grpNr;
}

int setGrpEffect(
	int grpNr,
	pattern pattern,
	int amountglitter = -1,
	int fps = 0,
	direction direction = FORWARD,
	mirror mirror = MIRROR0,
	wave wave = LINEAR,
	int speed = 1)
{
	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	neoGroup->Stop(true);

	//int fxGlitter = amountglitter <= 0 ? neoGroup->GetGlitter() : amountglitter;
	int fxGlitter = amountglitter <= 0 ? currGlitter.at(grpNr) : amountglitter;
	//int fxFps = fps <= 0 ? neoGroup->GetFps() : fps;
	int fxFps = fps > 0 ? fps : currFps.at(grpNr);

	uint16_t result = neoGroup->ConfigureEffect(pattern, fxGlitter, fxFps, direction, mirror, wave, speed);
	//neoGroup->Start();

	updateOledRequired = true;

	return result;
}

int setGrpColors(
	int grpNr,
	std::vector<CRGB> colors,
	bool clearFirst = true,
	bool generatePalette = true,
	bool crossFade = false)
{
	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	//neoGroup->Stop();
	uint16_t result = neoGroup->ConfigureColors(colors, clearFirst, generatePalette, crossFade);
	//neoGroup->Start();

	updateOledRequired = true;

	return result;
}

// [Event handling helper methods]
void StartStopEffect(int grpNr)
{
	int offsetGrpNr = groupRoomOffset + grpNr;
	DEBUG_PRINTLN("StartStopEffect ---------------------------------------------");
	DEBUG_PRINT("Fx: Starting/stopping group #");
	DEBUG_PRINT(offsetGrpNr);
	DEBUG_PRINT(": ");

	NeoGroup *neoGroup = &(neoGroups.at(offsetGrpNr));
	DEBUG_PRINT(neoGroup->Active);
	DEBUG_PRINT("->");
	DEBUG_PRINTLN(!(neoGroup->Active));
	bool stopNow = false;
#ifdef ADD_GROUP_FOR_ALL_ROOMS
	if (offsetGrpNr == groupNrAllRooms)
		stopNow = true; // Main group cannot be faded out, causes flickering
#endif
	if (neoGroup->Active)
		stopGroup(offsetGrpNr, stopNow);
	else
		startGroup(offsetGrpNr);
}

void SetEffect(int grpNr, int fxNr,
			   bool startFx, bool onlyOnce,
			   direction fxDirection = direction::FORWARD,
			   int amountGlitter = -1,
			   int targetFps = 0,
			   int speed = 1)
{
	DEBUG_PRINTLN("SetEffect ---------------------------------------------------");
	DEBUG_PRINTLN("Fx: Configuring LED effect #" + String(fxNr) + " for group #" + String(grpNr));

	if (fxNr == 0)
	{
		DEBUG_PRINTLN("Fx: Choosing random effect.");
		SetEffect(grpNr, random8(1, maxFxNr), startFx, onlyOnce);
		return;
	}

	String fxPatternName = "";
	pattern fxPattern = pattern::STATIC;
	int fxGlitter = currGlitter.at(grpNr);
	int fxFps = currFps.at(grpNr);
	mirror fxMirror = MIRROR0;
	wave fxWave = wave::LINEAR;

	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	switch (fxNr)
	{
	case fxNrWave:
		fxPatternName = "Wave";
		fxPattern = pattern::WAVE;
		fxMirror = mirror::MIRROR2;
		break;
	case fxNrDynamicWave:
		fxPatternName = "Dynamic Wave";
		fxPattern = pattern::DYNAMICWAVE;
		fxMirror = mirror::MIRROR1; // mirror::MIRROR0;
		break;
	case fxNrNoise:
		fxPatternName = "Noise";
		fxPattern = pattern::NOISE;
		fxMirror = mirror::MIRROR1; // mirror::MIRROR0;
		fxFps *= 2;					// double FPS looks better
		break;
	case fxNrConfetti:
		fxPatternName = "Confetti";
		fxPattern = pattern::CONFETTI;
		fxGlitter = 0;
		fxFps /= 2; // half FPS looks better
		break;
	case fxNrFade:
		fxPatternName = "Fade";
		fxPattern = pattern::FADE;
		fxFps /= 2; // half FPS looks better
		break;
	case fxNrComet:
		fxPatternName = "Comet";
		fxPattern = pattern::COMET;
		fxWave = wave::EASEINOUT;
		// fxWave = wave::SINUS;
		fxFps *= 3; //1.5; // faster FPS looks better
		// fxMirror = mirror::MIRROR0;
		fxMirror = mirror::MIRROR2;
		break;
	case fxNrOrbit:
		fxPatternName = "Orbit";
		fxPattern = pattern::COMET;
		// fxWave = wave::EASEINOUT;
		fxWave = wave::SINUS;
		fxFps *= 1.5; // faster FPS looks better
		// fxMirror = mirror::MIRROR0;
		fxMirror = mirror::MIRROR2;
		break;
	case fxNrFill:
		fxPatternName = "Fill";
		fxPattern = pattern::FILL;
		fxWave = wave::EASEINOUT;
		fxFps *= 1.5; // faster FPS looks better
		// fxMirror = mirror::MIRROR0;
		fxMirror = mirror::MIRROR2;
		break;
	default:
		fxPatternName = "Static";
		fxPattern = pattern::STATIC;
		fxMirror = mirror::MIRROR0;
		break;
	}
	if (targetFps > 0)
		fxFps = targetFps;
	DEBUG_PRINTLN("Fx: Changing effect to '" + String(fxPatternName) + "'");
	setGrpEffect(
		grpNr,
		fxPattern,
		fxGlitter,
		fxFps,
		fxDirection,
		fxMirror,
		fxWave,
		speed);
	if (startFx)
		startGroup(grpNr, onlyOnce);
	DEBUG_PRINTLN("SetEffect ---------------------------------------------------");
}

void InitColorNames()
{
	InitColorPalettes();
	/*
	for (std::map<String, std::vector<CRGB>>::const_iterator it = ColorPalettes.begin(); it != ColorPalettes.end(); ++it)
	{
		String key = it->first;
		ColorNames.push_back(key);
	}
	*/
}

void SetColors(int grpNr, int colNr)
{
	DEBUG_PRINTLN("SetColors --------------------------------------------------");
	DEBUG_PRINT("Col: Configuring LED colors #");
	DEBUG_PRINT(colNr);
	DEBUG_PRINT(" for group #");
	DEBUG_PRINTLN(grpNr);

	if (colNr == 0)
	{
		DEBUG_PRINTLN("Col: Choosing random color palette.");
		SetColors(grpNr, random8(1, maxColNr));
		return;
	}

	String palKey = ColorNames[colNr - 1];
	DEBUG_PRINT("Col: Changing color palette to '");
	DEBUG_PRINT(palKey);
	DEBUG_PRINTLN("'");
	if (ColorPalettes.find(palKey) != ColorPalettes.end())
	{
		std::vector<CRGB> colors = ColorPalettes.find(palKey)->second;
		setGrpColors(grpNr, colors, true, true, CROSSFADE_PALETTES);
	}
}

void NextEffect(int nextFx = -1)
{
	int offsetGrpNr = groupRoomOffset + activeGrpNr;
	if (nextFx < 0)
	{
		(currFxNr.at(offsetGrpNr))++;
	}
	else
	{
		currFxNr.at(offsetGrpNr) = nextFx;
	}
	if (currFxNr.at(offsetGrpNr) > maxFxNr)
	{
#ifdef ENABLE_RANDOM_FX
		currFxNr.at(offsetGrpNr) = 0;
#else
		currFxNr.at(offsetGrpNr) = 1;
#endif
	}
	DEBUG_PRINT("CONTROL: Button 'FX' pressed, changing effect number to: ");
	DEBUG_PRINTLN(currFxNr.at(offsetGrpNr));
	SetEffect(offsetGrpNr, currFxNr.at(offsetGrpNr), true, false);
}

void NextColor(int nextCol = -1)
{
	int offsetGrpNr = groupRoomOffset + activeGrpNr;
	if (nextCol < 0)
	{
		(currColNr.at(offsetGrpNr))++;
	}
	else
	{
		currColNr.at(offsetGrpNr) = nextCol;
	}
	if (currColNr.at(offsetGrpNr) > maxColNr)
	{
#ifdef ENABLE_RANDOM_FX
		currColNr.at(offsetGrpNr) = 0;
#else
		currColNr.at(offsetGrpNr) = 1;
#endif
	}
	DEBUG_PRINT("CONTROL: Button 'Colors' pressed, changing color number to: ");
	DEBUG_PRINTLN(currColNr.at(offsetGrpNr));
	SetColors(offsetGrpNr, currColNr.at(offsetGrpNr));
}

void FlashStatusLED(int grpNr, int count)
{
	NeoGroup *neoGroupStatus = &(neoGroups.at(groupNrStatus));
	for (int p = 0; p < count; p++) // flash 1 time
	{
		neoGroupStatus->SetPixel(mapRoomToStatusLed.at(grpNr), CRGB::White);
		FastLED.show();
		delay(100);
		neoGroupStatus->SetPixel(mapRoomToStatusLed.at(grpNr), CRGB::Black);
		FastLED.show();
		delay(100);
	}
}
#pragma endregion

// **************************************************
// *** OLED Display
// **************************************************
#pragma region OLED Display
//#ifdef i2cOLED
// [Update OLED Display]
void drawDisplay()
{
	int offsetGrpNr = groupRoomOffset + activeGrpNr;
	NeoGroup *neoGroup = &(neoGroups.at(offsetGrpNr));
	int fxNr = currFxNr[offsetGrpNr];
	String fxPatternName = "???";
	switch (fxNr)
	{
	case 1: // Wave
		fxPatternName = "Welle";
		break;
	case 2: // Dynamic Wave
		fxPatternName = "Dynamisch";
		break;
	case 3: // Noise
		fxPatternName = "Plasma";
		break;
	case 4: // confetti
		fxPatternName = "Konfetti";
		break;
	case 5: // Fade
		fxPatternName = "Blenden";
		break;
	default:
		fxPatternName = "Statisch";
		break;
	}
	int colNr = currColNr[offsetGrpNr];
	String fxColorName = ColorNames[colNr - 1];

	DEBUG_PRINTLN("----------------------------------------");
	DEBUG_PRINT("--- Updating OLED display: ");
#ifdef i2cOLED
	DEBUG_PRINTLN("ENABLED");
#else
	DEBUG_PRINTLN("DISABLED");
#endif
	DEBUG_PRINTLN("----------------------------------------");
	DEBUG_PRINTLN("--- " + neoGroup->GroupID);
	DEBUG_PRINTLN("--- " + fxPatternName);
	DEBUG_PRINTLN("--- " + fxColorName);
	DEBUG_PRINTLN("----------------------------------------");

#ifdef i2cOLED
	// clear the display
	display.clear();

	display.setColor(OLEDDISPLAY_COLOR::WHITE);
	display.setFont(Roboto_Condensed_32);
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.drawString(64, -2, neoGroup->GroupID);

	display.setColor(OLEDDISPLAY_COLOR::WHITE);
	display.setFont(Roboto_Condensed_12);
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.drawString(2, 32, fxPatternName);
	display.setTextAlignment(TEXT_ALIGN_RIGHT);
	display.drawString(126, 47, fxColorName);

	//display.drawHorizontalLine(0, 32, 128);
	if (neoGroup->Active)
	{
		display.setColor(OLEDDISPLAY_COLOR::INVERSE);
		display.fillRect(0, 0, 128, 32);
	}
	else
	{
		display.setColor(OLEDDISPLAY_COLOR::WHITE);
		display.drawRect(0, 0, 128, 32);
	}
	display.setColor(OLEDDISPLAY_COLOR::WHITE);
	display.drawRect(0, 0, 128, 64);

	// write the buffer to the display
	display.display();
#endif
}
//#endif
#pragma endregion

// **************************************************
// *** Event Handlers (Buttons, Tickers)
// **************************************************
#pragma region Event Handlers
// [I2C Expander Interrupt Service Routine]
void ISRgateway()
{
	//os_intr_lock();
	buttonPressedOnI2C = true;
	//expander.checkForInterrupt();
	//DEBUG_PRINTLN("PCF8574: change of buttons' state detected in ISRgateway.");
	//os_intr_unlock();
}

void changeToRoom(int roomNo = -1)
{
	if (areButtonsLocked())
		return;

	lockButtons();

	bool wasGroupAllRooms = ((groupRoomOffset + activeGrpNr) == groupNrAllRooms);

	DEBUG_PRINT("PCF8574: switching to room ");
	DEBUG_PRINT(roomNo + 1);
	DEBUG_PRINT("->");
	if (roomNo < 0)
	{
		activeGrpNr++;
		if (activeGrpNr >= groupRoomCount)
			activeGrpNr = 0;
		roomWasChanged = true;
	}
	else
	{
		roomWasChanged = (activeGrpNr != roomNo);
		activeGrpNr = constrain(roomNo, 0, groupRoomCount - 1);
	}
	DEBUG_PRINT(activeGrpNr);
	DEBUG_PRINTLN(".");

	int offsetGrpNr = groupRoomOffset + activeGrpNr;
	if (roomWasChanged)
	{
		// Stop group if switching from "all rooms" to single room group
		if (wasGroupAllRooms &&
			offsetGrpNr != groupNrAllRooms)
		{
			DEBUG_PRINTLN("PCF8574: Switching to other group, stopping group 'all rooms'.");
			stopGroup(groupNrAllRooms, true);
		}

		// Start group if switching from to "all rooms" group
		if (offsetGrpNr == groupNrAllRooms)
		{
			DEBUG_PRINTLN("PCF8574: Switching to group 'all rooms', starting immediately.");
			startGroup(groupNrAllRooms);
		}
	}
	else
	{
#ifdef BUTTON_TOGGLE_GROUP_ON_OFF
		DEBUG_PRINTLN("PCF8574: Same group, toggling group's on/off state.");
		// Toggle active state on/off
		if (isGroupActive(offsetGrpNr))
		{
			stopGroup(offsetGrpNr);
		}
		else
		{
			startGroup(offsetGrpNr);
		}
#endif
	}

	updateOledRequired = true;
}
void onButton1() { changeToRoom(mapI2cButtonToRoom.at(0)); }
void onButton2() { changeToRoom(mapI2cButtonToRoom.at(1)); }
void onButton3() { changeToRoom(mapI2cButtonToRoom.at(2)); }
void onButton4() { changeToRoom(mapI2cButtonToRoom.at(3)); }
void onButton5() { changeToRoom(mapI2cButtonToRoom.at(4)); }
void onButton6() { changeToRoom(mapI2cButtonToRoom.at(5)); }
void onButton7() { changeToRoom(mapI2cButtonToRoom.at(6)); }
void onButton8() { changeToRoom(mapI2cButtonToRoom.at(7)); }

// [Directly connected buttons]
void onButtonStartStop()
{
	buttonStartStopPressed = true;
}
void onButtonFx()
{
	buttonFxPressed = true;
	if (buttonFxPressed && buttonColPressed)
		bothButtonsPressed = true;
}
void onButtonCol()
{
	buttonColPressed = true;
	if (buttonFxPressed && buttonColPressed)
		bothButtonsPressed = true;
}

// [Button locking]
void lockButtons()
{
	buttonsLocked = true;
	buttonsLockedAt = millis();
}

bool areButtonsLocked()
{
	if (buttonsLocked)
	{
		uint32_t lockedAgo = buttonsLockedAt > 0 ? millis() - buttonsLockedAt : buttonLockDuration;
		if (lockedAgo < buttonLockDuration)
		{
			DEBUG_PRINT("Buttons STILL LOCKED, locked just ");
			DEBUG_PRINT(lockedAgo);
			DEBUG_PRINTLN("ms ago...");
			return true;
		}
		buttonsLocked = false;
	}
	//DEBUG_PRINTLN("Buttons not locked.");
	return false;
}
#pragma endregion

// **************************************************
// *** Application Setuo
// **************************************************
#pragma region Application Setup
void setup()
{
	Serial.begin(115200);

	DEBUG_PRINTLN("BOOT/SETUP ------------------------------------------------");
	DEBUG_PRINTLN("Setup: Setting up SmartCastle for Arduino");

	DEBUG_PRINTLN("Setup: directly attaching buttons");
	pinMode(BUTTON_PIN_START_STOP, INPUT_PULLUP);
	attachInterrupt(BUTTON_PIN_START_STOP, onButtonStartStop, FALLING);
	pinMode(BUTTON_PIN_FX, INPUT_PULLUP);
	attachInterrupt(BUTTON_PIN_FX, onButtonFx, FALLING);
	pinMode(BUTTON_PIN_COL, INPUT_PULLUP);
	attachInterrupt(BUTTON_PIN_COL, onButtonCol, FALLING);

#ifdef i2cOLED
	// Initialize OLED display
	display.init();
	display.flipScreenVertically();
	//drawDisplay();
#endif

	// Wire buttons and events (I2C or directly connected)
	DEBUG_PRINT("PCF8574: setting bus speed to ");
	DEBUG_PRINT(I2C_BUS_SPEED);
	DEBUG_PRINTLN(".");
#ifndef i2cOLED
	Wire.begin(I2C_SDA_PIN, I2C_SDC_PIN);
#endif
	Wire.setClock(I2C_BUS_SPEED);

	DEBUG_PRINT("PCF8574: use I2C address ");
	DEBUG_PRINT(I2C_EXPANDER_ADDRESS);
	DEBUG_PRINTLN(".");
	DEBUG_PRINTLN("PCF8574: start listening.");
	expander.begin(I2C_EXPANDER_ADDRESS);

	DEBUG_PRINTLN("PCF8574: setting PINs.");
	expander.pinMode(0, INPUT_PULLUP);
	expander.pinMode(1, INPUT_PULLUP);

	DEBUG_PRINTLN("PCF8574: attaching ISR handler.");
	//expander.enableInterrupt(I2C_INT_PIN, ISRgateway);
	pinMode(I2C_INT_PIN, INPUT_PULLUP);
	attachInterrupt(I2C_INT_PIN, ISRgateway, FALLING);

	DEBUG_PRINTLN("PCF8574: attaching button triggers.");
	// Must map according to kaypad layout!!!
	expander.attachInterrupt(0, onButton1, FALLING);
	expander.attachInterrupt(1, onButton2, FALLING);
	expander.attachInterrupt(2, onButton3, FALLING);
	expander.attachInterrupt(3, onButton4, FALLING);
	expander.attachInterrupt(4, onButton5, FALLING);
	expander.attachInterrupt(5, onButton6, FALLING);
	expander.attachInterrupt(6, onButton7, FALLING);
	expander.attachInterrupt(7, onButton8, FALLING);

#ifdef INCLUDE_WIFI
	//InitWifi();
#endif

	DEBUG_PRINTLN("FastLED: Initializing color palettes");
	InitColorNames();
	maxColNr = ColorNames.size();

	DEBUG_PRINTLN("FastLED: Initializing LED strip");
	initStrip(true, PLAY_DEMO);
	DEBUG_PRINT("FastLED: Amount of LEDs: ");
	DEBUG_PRINTLN(PIXEL_COUNT);

	DEBUG_PRINTLN("FastLED: Starting LED strip");
	startStrip();

	for (int i = 0; i < groupRoomCount; i++)
	{
		int offsetGrpNr = groupRoomOffset + i;
		DEBUG_PRINT("FastLED: Setting up and starting group #");
		DEBUG_PRINTLN(offsetGrpNr);
		bool startFx = true;
#ifdef DO_NOT_START_FX_ON_INIT
		startFx = false;
#endif
		bool isAllRoomsGroup = offsetGrpNr == groupNrAllRooms;
#ifdef START_WITH_RANDOM_COLOR_PALETTE
		// Random default color palette
		int rndCol = random8(0, ColorNames.size() - 1);
		currColNr[offsetGrpNr] = rndCol;
		SetColors(offsetGrpNr, rndCol);
#else
		int currCol = isAllRoomsGroup ? defaultColNrAll : defaultColNr;
		currColNr[offsetGrpNr] = currCol;
		SetColors(offsetGrpNr, currCol);
#endif
#ifdef ADD_GROUP_FOR_ALL_ROOMS
#ifndef GROUP_FOR_ALL_ROOMS_IS_ACTIVE
		if (isAllRoomsGroup)
			startFx = false;
#endif
#endif
		int currFx = isAllRoomsGroup ? defaultFxNrAll : defaultFxNr;
		currFxNr[offsetGrpNr] = currFx;
		SetEffect(offsetGrpNr, currFx, startFx, false);
		//startGroup(offsetGrpNr);
	}
	//activeGrpNr = 0;

	DEBUG_PRINT("FastLED: Active group #");
	DEBUG_PRINTLN(activeGrpNr);

	//#ifdef i2cOLED
	drawDisplay();
	//#endif
}
#pragma endregion

// **************************************************
// *** Application Loop
// **************************************************
#pragma region Application Loop
// Main loop
void loop()
{
	// Evaluate expander pins and execute attached callbacks
	if (buttonPressedOnI2C)
	{
		buttonPressedOnI2C = false;
		DEBUG_PRINTLN("PCF8574: change of buttons' state detected in ISRgateway.");
		expander.checkForInterrupt();
	}

	//DEBUG_PRINTLN("LOOP ------------------------------------------------------");
	bool btnStartStopReleased = buttonStartStopPressed && !digitalRead(BUTTON_PIN_START_STOP);
	bool btnFxReleased = buttonFxPressed && !digitalRead(BUTTON_PIN_FX);
	bool btnColReleased = buttonColPressed && !digitalRead(BUTTON_PIN_COL);
	if (bothButtonsPressed &&
		(btnFxReleased | btnColReleased) /*&&
		WiFi.status() != WL_CONNECTED*/
		)								 // Both buttons pressed
	{
		DEBUG_PRINTLN("Loop: both buttons pressed, entering WiFi-setup.");
#ifdef INCLUDE_WIFI
		InitWifi(false, true);
#endif
		buttonColPressed = false;
		buttonFxPressed = false;
		bothButtonsPressed = false;
	}
	else
	{
		if (buttonFxPressed && btnFxReleased) // Button was released
		{
			if (!areButtonsLocked())
			{
				lockButtons();
				DEBUG_PRINTLN("Loop: button 'FX' pressed and released.");
				NextEffect();

				FlashStatusLED(activeGrpNr, 1);
			}
			buttonFxPressed = false;
		}
		if (buttonColPressed && btnColReleased) // Button was released
		{
			if (!areButtonsLocked())
			{
				lockButtons();
				DEBUG_PRINTLN("Loop: button 'colors' pressed and releases.");
				NextColor();

				FlashStatusLED(activeGrpNr, 2);
			}
			buttonColPressed = false;
		}
		if (buttonStartStopPressed && btnStartStopReleased)
		{
			if (!areButtonsLocked())
			{
				lockButtons();
				DEBUG_PRINTLN("Loop: button 'Start/Stop' pressed and released.");
				StartStopEffect(activeGrpNr);
			}
			buttonStartStopPressed = false;
		}
	}

	// Update OLED display only if required
	if (updateOledRequired)
	{
		drawDisplay();
		updateOledRequired = false;
	}

	/*
	if (buttonsLocked) // unlock buttons after a lock time passed
		areButtonsLocked();
	*/

	if (!ledsStarted)
	{
		//DEBUG_PRINTLN("Loop: LEDs not started, leaving loop.");
		return;
	}

	if (roomWasChanged)
	{
		int offsetGrpNr = groupRoomOffset + activeGrpNr;
		// Visual feedback which room was changed
		NeoGroup *neoGroup = &(neoGroups.at(offsetGrpNr));
		for (int p = 0; p < 2; p++) // flash 2 times
		{
			neoGroup->FillPixels(0, neoGroup->LedCount, CRGB::White);
			FastLED.show();
			delay(100);
			neoGroup->FillPixels(0, neoGroup->LedCount, CRGB::Black);
			FastLED.show();
			delay(100);
		}
		roomWasChanged = false;
	}

	bool isActiveMainGrp =
		(groupNrAllRooms >= 0)
			? (&(neoGroups.at(groupNrAllRooms)))->Active
			: false;
#ifdef DEBUG_LOOP
/*
			DEBUG_PRINT("Loop: Main group active -> ");
			DEBUG_PRINTLN(isActiveMainGrp);
*/
#endif
	bool ledsUpdated = false;
	for (int i = 0; i < groupRoomCount; i++)
	{
		int offsetGrpNr = groupRoomOffset + i;
		NeoGroup *neoGroup = &(neoGroups.at(offsetGrpNr));

#ifdef ADD_GROUP_FOR_ALL_ROOMS
		if (isActiveMainGrp && offsetGrpNr != groupNrAllRooms)
		{
#ifdef DEBUG_LOOP
			DEBUG_PRINT("Loop: Skipping room from #");
			DEBUG_PRINTLN(i);
#endif
			continue; // Don't update other groups if main group (all LEDs) is active
		}
#endif

		if ((neoGroup->LedFirstNr + neoGroup->LedCount) <= PIXEL_COUNT)
		{
#ifdef DEBUG_LOOP
/*
			DEBUG_PRINT("Loop: Updating group ");
			DEBUG_PRINTLN(i);
*/
#endif
			ledsUpdated |= neoGroup->Update();
		}
	}

	// Cycle palette of group at corresponding status LED
	static unsigned long lastUpdate = 0;
	static unsigned long updateInterval = (1000 / 50); // 50 fps
	static uint8_t statusIdx = 0;
	static uint8_t currentBrightnessIdx = 0;
	if ((millis() - lastUpdate) > updateInterval)
	{
#ifdef DEBUG_LOOP
		DEBUG_PRINT("Updating status LEDS (idx:");
		DEBUG_PRINT(statusIdx);
		DEBUG_PRINT(")...");
#endif
		NeoGroup *neoGroupStatus = &(neoGroups.at(groupNrStatus));
		for (int statusForRoomNr = 0; statusForRoomNr < groupRoomCount; statusForRoomNr++)
		{
			int offsetGrpNr = groupRoomOffset + statusForRoomNr;
#ifdef DEBUG_LOOP
			DEBUG_PRINT(statusForRoomNr);
#endif
			NeoGroup *neoGroup = &(neoGroups.at(offsetGrpNr));
			uint8_t currentBrightness = statusBrightness;
			if (statusForRoomNr == activeGrpNr)
			{
				currentBrightness = quadwave8(currentBrightnessIdx);
				//currentBrightness = 15 + currentBrightness - (currentBrightness >> 4);
				currentBrightness = 7 + currentBrightness - (currentBrightness >> 5);
#ifdef DEBUG_LOOP
				DEBUG_PRINT("(active)");
#endif
			}
			if (!neoGroup->Active ||
				(isActiveMainGrp && offsetGrpNr != groupNrAllRooms)) // If "All Rooms" is active, show others as inactive
			{
				// lower brightness of status LED if FX for group is not running
				currentBrightness = currentBrightness >> 3;
#ifdef DEBUG_LOOP
				DEBUG_PRINT("(off)");
#endif
			}
#ifdef DEBUG_LOOP
			else
			{
				DEBUG_PRINT("(on)");
			}
#endif
			// no color cycling if group is not active
			CRGB statusColor =
				(neoGroup->Active)
					? neoGroup->GetColorFromPaletteAt(statusIdx, currentBrightness)
					: neoGroup->GetColorFromPaletteAt(0, currentBrightness);
#ifdef ADD_GROUP_FOR_ALL_ROOMS
			if (isActiveMainGrp && offsetGrpNr != groupNrAllRooms)
			{
				statusColor = CRGB(0x101010); // grey if inactive or "All rooms" group is active
			}
			if (statusColor.r == 0 && statusColor.g == 0 && statusColor.b == 0)
			{
				statusColor = CRGB(currentBrightness, currentBrightness, currentBrightness);
			}
#endif
			neoGroupStatus->SetPixel(mapRoomToStatusLed.at(statusForRoomNr), statusColor);
#ifdef DEBUG_LOOP
			DEBUG_PRINT("...");
#endif
		}
#ifdef DEBUG_LOOP
		DEBUG_PRINTLN("DONE");
#endif
		statusIdx++;
		currentBrightnessIdx += activeGroupBrightnessCycleSpeed;
		ledsUpdated = true;
		lastUpdate = millis();
	}

	if (ledsUpdated)
	{
#ifdef DEBUG_LOOP
		DEBUG_PRINTLN("Loop: Refreshing LEDs.");
#endif
		FastLED.show();
	}
}
#pragma endregion