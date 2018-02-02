/*
* Project SmartCastle
* Description:
* Author:
* Date:
*/

// **************************************************
// *** Compiler Flags
// **************************************************
//#define INCLUDE_WIFI
#define ADD_GROUP_FOR_ALL_ROOMS
//#define SINGLE_ROOM
#define START_WITH_RANDOM_COLOR_PALETTE
//#define DEBUG_LOOP
//#define DO_NOT_START_FX_ON_INIT

// **************************************************
// *** Includes
// **************************************************
#include "SerialDebug.h"
#include "I2CInclude.h"
#include "FastLedInclude.h"
#include "ColorPalettes.h"
#include <Arduino.h>
//#include <ArduinoSTL.h>
#include <vector>
#include <map>
#include <string.h>
#include "NeoGroup.cpp"

#ifdef INCLUDE_WIFI
#include <ESP8266WiFi.h>	  //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>		  //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h> //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>	  //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#endif

// **************************************************
// *** Blynk settings
// **************************************************
/*
#define BLYNK_PRINT Serial
#define BLYNK_MAX_SENDBYTES 512 // Default is 128
#include <BlynkSimpleEsp8266.h>
// Device 'SmartCastle':
const char blynkAuth[] = "4abfe0577ae745aca3d5d5d9f37911b7";
*/

// Helper macro
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// **************************************************
// *** Variable and Constamts  Declarations
// **************************************************
#ifdef INCLUDE_WIFI
const String wifiApName = "AP_SmartCastle";
const int ConfigureAPTimeout = 10;
#endif

int globalBrightness = 96;

volatile bool buttonPressedOnI2C = false;

// Test 1: equally sized rooms
//const std::vector<int> groupRoomSizes = {16, 16, 16, 16, 16, 16, 16, 16};
// Test 2: differently sized rooms
const std::vector<int> groupRoomSizes = {4, 8, 12, 16, 16, 20, 24, 28};
int groupRoomTotalSize = 0;
int groupRoomCount = 0;
#ifdef ADD_GROUP_FOR_ALL_ROOMS
int groupRoomOffset = 3;
#else
int groupRoomOffset = 2;
#endif
int currGrpNr = groupRoomOffset;

// Static size
//struct CRGB leds[PIXEL_COUNT];
// Dynamic size:
struct CRGB *leds = NULL;
bool ledsInitialized = false;
bool ledsStarted = false;
const int maxFxNr = 5;
// 1: Wave, 2: Dynamic Wave, 3: Noise, 4: confetti, 5: Fade,
const int defaultFxNr = 1;
std::vector<int> currFxNr;
int maxColNr = 5;
const int defaultColNr = 1;
std::vector<int> currColNr;
const int defaultFps = 50; //25;
std::vector<int> currFps;
const int defaultGlitter = 0; //32;
std::vector<int> currGlitter;

//std::vector<NeoGroup *> neoGroups;
std::vector<NeoGroup> neoGroups;

static bool buttonStartStopPressed = false;
static bool buttonFxPressed = false;
static bool buttonColPressed = false;
static bool bothButtonsPressed = false;

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

/*
void InitBlynk()
{
	if (WiFi.status() != WL_CONNECTED)
		return;

	DEBUG_PRINTLN("BLYNK -----------------------------------------------------");
	DEBUG_PRINTLN("Blynk: authenticating");
	Blynk.config(blynkAuth);
	Blynk.connect();

	SendMenusToBlynk();
	SendStatusToBlynk();
}

void SendMenusToBlynk()
{
	if (WiFi.status() != WL_CONNECTED)
		return;

	DEBUG_PRINTLN("Blynk: assigning menu 'FX'");
	BlynkParamAllocated fxItems(128);
	fxItems.add("Zufällig");
	fxItems.add("Lauflicht");
	fxItems.add("Dynamisch");
	fxItems.add("Wasser");
	fxItems.add("Konfetti");
	fxItems.add("Farbwechsel");
	Blynk.setProperty(V2, "labels", fxItems);

	DEBUG_PRINTLN("Blynk: assigning menu 'Colors'");
	BlynkParamAllocated colItems(128);
	colItems.add("Zufällig");
	for (int i = 0; i < ColorNames.size(); i++)
	{
		colItems.add(ColorNames[i]);
	}
	Blynk.setProperty(V4, "labels", colItems);
}

void SendStatusToBlynk()
{
	if (WiFi.status() != WL_CONNECTED)
		return;

	DEBUG_PRINTLN("Blynk: sending current status");
	Blynk.virtualWrite(V0, ledsStarted);
	Blynk.virtualWrite(V2, currFxNr.at(currGrpNr) + 1);
	Blynk.virtualWrite(V4, currColNr.at(currGrpNr) + 1);
	Blynk.virtualWrite(V5, currGlitter.at(currGrpNr));
	Blynk.virtualWrite(V6, globalBrightness);
	Blynk.virtualWrite(V7, currFps.at(currGrpNr));
}
*/

int initStrip(bool doStart = false, bool playDemo = true)
{
	if (ledsInitialized)
	{
		return doStart ? startStrip() : PIXEL_COUNT;
	}

	DEBUG_PRINTLN("LEDStrip --------------------------------------------------");
	DEBUG_PRINTLN("Allocating memory for LED strip data.");
	leds = (struct CRGB *)malloc(PIXEL_COUNT * sizeof(struct CRGB));
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
		DEBUG_PRINTLN("Playing little demo effect.");
		for (int dot = 0; dot < PIXEL_COUNT; dot++)
		{
			leds[dot] = CHSV(random8(), 255, 255);
			FastLED.show();
			delay(10);
		}
		delay(500);
	}

#ifdef INCLUDE_WIFI
	InitWifi();
	/*
	if (InitWifi())
		InitBlynk();
	*/
#endif

	if (playDemo)
	{
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

	DEBUG_PRINTLN("Calculating groups.");
	groupRoomCount = groupRoomSizes.size();
	groupRoomTotalSize = 0;
	for (int i = 0; i < groupRoomSizes.size(); i++)
		groupRoomTotalSize += groupRoomSizes[i];

	DEBUG_PRINTLN("Adding special groups.");
	neoGroups.clear();
	// Group 0: all LEDs
	addGroup("  All LEDs group", 0, PIXEL_COUNT, 0);
	// Group 1: Status LEDs
	addGroup("  Status LED group", 0, groupRoomCount, 0);
#ifdef ADD_GROUP_FOR_ALL_ROOMS
	// Group 2: all rooms' LEDs
	addGroup("  All rooms LED group", groupRoomCount, groupRoomTotalSize, 0);
#endif

#ifdef SINGLE_ROOM
	DEBUG_PRINTLN("LED group for single room.");
	addGroup("Room 1", groupRoomCount, groupRoomTotalSize, 0);
#else
	DEBUG_PRINTLN("Adding groups for rooms.");
	int nextGroupStart = groupRoomCount; // Start after status LEDs
	for (int i = 0; i < groupRoomSizes.size(); i++)
	{
		String roomName = "Room " + String(i + 1);
		DEBUG_PRINT("  Adding '");
		DEBUG_PRINT(roomName);
		DEBUG_PRINT("' at ");
		DEBUG_PRINT(nextGroupStart);
		DEBUG_PRINT("  with size ");
		DEBUG_PRINTLN(groupRoomSizes[i]);
		addGroup(roomName, nextGroupStart, groupRoomSizes[i], 0);
		nextGroupStart += groupRoomSizes[i];
	}
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

int startGroup(int grpNr)
{
	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	neoGroup->Start();
	return 0;
}

int stopGroup(int grpNr, bool stopNow = false)
{
	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	neoGroup->Stop(stopNow);
	return 0;
}

int setGrpEffect(
	int grpNr,
	pattern pattern,
	uint16_t length = 0,
	int amountglitter = -1,
	uint8_t fps = 0,
	direction direction = FORWARD,
	mirror mirror = MIRROR0)
{
	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	neoGroup->Stop();

	//int fxGlitter = amountglitter <= 0 ? neoGroup->GetGlitter() : amountglitter;
	int fxGlitter = amountglitter <= 0 ? currGlitter.at(grpNr) : amountglitter;
	//uint8_t fxFps = fps <= 0 ? neoGroup->GetFps() : fps;
	uint8_t fxFps = fps <= 0 ? currFps.at(grpNr) : fps;

	uint16_t result = neoGroup->ConfigureEffect(pattern, length, fxGlitter, fxFps, direction, mirror);
	//neoGroup->Start();
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
	return result;
}

void StartStopEffect(int grpNr)
{
	DEBUG_PRINTLN("StartStopEffect ---------------------------------------------");
	DEBUG_PRINT("Fx: Configuring LED FX status for group #");
	DEBUG_PRINT(grpNr);
	DEBUG_PRINT(": ");

	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	DEBUG_PRINT(neoGroup->Active);
	DEBUG_PRINT("->");
	DEBUG_PRINT(!(neoGroup->Active));
	if (neoGroup->Active)
		stopGroup(grpNr);
	else
		startGroup(grpNr);
}

void SetEffect(int grpNr, int fxNr, bool startFx = false)
{
	DEBUG_PRINTLN("SetEffect ---------------------------------------------------");
	DEBUG_PRINT("Fx: Configuring LED effect #");
	DEBUG_PRINT(fxNr);
	DEBUG_PRINT(" for group #");
	DEBUG_PRINTLN(grpNr);

	if (fxNr == 0)
	{
		DEBUG_PRINTLN("Fx: Choosing random effect.");
		SetEffect(grpNr, random8(1, maxFxNr), startFx);
		return;
	}

	String fxPatternName = "";
	pattern fxPattern = pattern::STATIC;
	uint16_t fxLength = 255;
	int fxGlitter = currGlitter.at(grpNr);
	uint8_t fxFps = currFps.at(grpNr);
	mirror fxMirror = MIRROR0;

	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	switch (fxNr)
	{
	case 1: // Wave
		fxPatternName = "Wave";
		fxPattern = pattern::WAVE;
		fxLength = (neoGroup->LedCount * 1.5); // 48;
		fxMirror = mirror::MIRROR2;
		break;
	case 2: // Dynamic Wave
		fxPatternName = "Dynamic Wave";
		fxPattern = pattern::DYNAMICWAVE;
		fxMirror = mirror::MIRROR1; // mirror::MIRROR0;
		break;
	case 3: // Noise
		fxPatternName = "Noise";
		fxPattern = pattern::NOISE;
		fxMirror = mirror::MIRROR1; // mirror::MIRROR0;
		fxFps *= 2;					// double FPS looks better
		break;
	case 4: // confetti
		fxPatternName = "Confetti";
		fxPattern = pattern::CONFETTI;
		fxGlitter = 0;
		break;
	case 5: // Fade
		fxPatternName = "Fade";
		fxPattern = pattern::FADE;
		fxFps /= 2; // half FPS looks better
		break;
	default:
		fxPatternName = "Static";
		fxPattern = pattern::STATIC;
		fxMirror = mirror::MIRROR0;
		break;
	}
	DEBUG_PRINT("Fx: Changing effect to '");
	DEBUG_PRINT(fxPatternName);
	DEBUG_PRINTLN("'");
	setGrpEffect(
		grpNr,
		fxPattern,
		fxLength,
		fxGlitter,
		fxFps,
		direction::FORWARD,
		fxMirror);
	if (startFx)
		startGroup(grpNr);
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
	if (nextFx < 0)
	{
		(currFxNr.at(currGrpNr))++;
	}
	else
	{
		currFxNr.at(currGrpNr) = nextFx;
	}
	if (currFxNr.at(currGrpNr) > maxFxNr)
		currFxNr.at(currGrpNr) = 0;
	DEBUG_PRINT("CONTROL: Button 'FX' pressed, changing effect number to: ");
	DEBUG_PRINTLN(currFxNr.at(currGrpNr));
	SetEffect(currGrpNr, currFxNr.at(currGrpNr), true);
}

void NextColor(int nextCol = -1)
{
	if (nextCol < 0)
	{
		(currColNr.at(currGrpNr))++;
	}
	else
	{
		currColNr.at(currGrpNr) = nextCol;
	}
	if (currColNr.at(currGrpNr) > maxColNr)
		currColNr.at(currGrpNr) = 0;
	DEBUG_PRINT("CONTROL: Button 'Colors' pressed, changing color number to: ");
	DEBUG_PRINTLN(currColNr.at(currGrpNr));
	SetColors(currGrpNr, currColNr.at(currGrpNr));
}

/*
BLYNK_WRITE(V0) // Button "Power"
{
	int pinValue = param.asInt();
	DEBUG_PRINTLN("BLYNK -----------------------------------------------------");
	DEBUG_PRINT("Blynk-Button 'POWER' pressed: ");
	DEBUG_PRINTLN(pinValue);
	if (pinValue == 1)
	{
		if (!ledsStarted)
		{
			startStrip();
			startGroup(currGrpNr);
		}
	}
	else
	{
		if (ledsStarted)
		{
			stopGroup(currGrpNr);
			stopStrip();
		}
	}
}

BLYNK_WRITE(V1) // Button "FX"
{
	int pinValue = param.asInt();
	DEBUG_PRINTLN("BLYNK -----------------------------------------------------");
	DEBUG_PRINT("Blynk-Button 'FX' pressed: ");
	DEBUG_PRINTLN(pinValue);
	if (pinValue == 1)
	{
		NextEffect();
		Blynk.virtualWrite(V2, (currFxNr.at(currGrpNr)) + 1);
	}
}

BLYNK_WRITE(V2) // DropDown "FX"
{
	int pinValue = param.asInt() - 1;
	DEBUG_PRINTLN("BLYNK -----------------------------------------------------");
	DEBUG_PRINT("Blynk-DropDown 'FX' selected: ");
	DEBUG_PRINTLN(pinValue);
	NextEffect(pinValue);
}

BLYNK_WRITE(V3) // Button "Color"
{
	int pinValue = param.asInt();
	DEBUG_PRINTLN("BLYNK -----------------------------------------------------");
	DEBUG_PRINT("Blynk-Button 'Colors' pressed: ");
	DEBUG_PRINTLN(pinValue);
	if (pinValue == 1)
	{
		NextColor();
		Blynk.virtualWrite(V4, (currColNr.at(currGrpNr)) + 1);
	}
}

BLYNK_WRITE(V4) // DropDown "Colors"
{
	int pinValue = param.asInt() - 1;
	DEBUG_PRINTLN("BLYNK -----------------------------------------------------");
	DEBUG_PRINT("Blynk-DropDown 'Colors' selected: ");
	DEBUG_PRINTLN(pinValue);
	NextColor(pinValue);
}

BLYNK_WRITE(V5) // Slider "Glitter"
{
	int pinValue = param.asInt();
	DEBUG_PRINTLN("BLYNK -----------------------------------------------------");
	DEBUG_PRINT("Blynk-Slider 'Glitter' selected: ");
	DEBUG_PRINTLN(pinValue);
	currGlitter.at(currGrpNr) = pinValue;
	NeoGroup *neoGroup = &(neoGroups.at(currGrpNr));
	neoGroup->ChangeGlitter(currGlitter.at(currGrpNr));
}

BLYNK_WRITE(V6) // Slider "Brightness"
{
	int pinValue = param.asInt();
	DEBUG_PRINTLN("BLYNK -----------------------------------------------------");
	DEBUG_PRINT("Blynk-Slider 'Brightness' selected: ");
	DEBUG_PRINTLN(pinValue);
	globalBrightness = pinValue;
	FastLED.setBrightness(pinValue);
	FastLED.setDither(0);
}

BLYNK_WRITE(V7) // Slider "Speed"
{
	int pinValue = param.asInt();
	DEBUG_PRINTLN("BLYNK -----------------------------------------------------");
	DEBUG_PRINT("Blynk-Slider 'Speed' selected: ");
	DEBUG_PRINTLN(pinValue);
	currFps.at(currGrpNr) = pinValue;
	NeoGroup *neoGroup = &(neoGroups.at(currGrpNr));
	neoGroup->ChangeFps(currFps.at(currGrpNr));
}
BLYNK_APP_CONNECTED()
{
	SendMenusToBlynk();
	SendStatusToBlynk();
	//Blynk.syncAll();
}
*/

// **************************************************
// *** Event Handlers (Buttons, Tickers)
// **************************************************
// [I2C Expander Interrupt Service Routine]
void ISRgateway()
{
	//os_intr_lock();
	buttonPressedOnI2C = true;
	//expander.checkForInterrupt();
	//DEBUG_PRINTLN("PCF8574: change of buttons' state detected in ISRgateway.");
	//os_intr_unlock();
}

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

void changeToRoom(int roomNo = -1)
{
	DEBUG_PRINT("PCF8574: switching to room ");
	DEBUG_PRINT(roomNo + 1);
	DEBUG_PRINT("->");
	if (roomNo < 0)
	{
		currGrpNr++;
		if (currGrpNr >= (groupRoomCount + groupRoomOffset))
			currGrpNr = groupRoomOffset;
	}
	else
	{
		currGrpNr = constrain(groupRoomOffset + roomNo, groupRoomOffset, groupRoomOffset + groupRoomCount - 1);
	}
	DEBUG_PRINT(currGrpNr);
	DEBUG_PRINTLN(".");
}
void onButtonRoom1() { changeToRoom(0); }
void onButtonRoom2() { changeToRoom(1); }
void onButtonRoom3() { changeToRoom(2); }
void onButtonRoom4() { changeToRoom(3); }
void onButtonRoom5() { changeToRoom(4); }
void onButtonRoom6() { changeToRoom(5); }
void onButtonRoom7() { changeToRoom(6); }
void onButtonRoom8() { changeToRoom(7); }
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

	// Wire buttons and events (I2C or directly connected)
	DEBUG_PRINT("PCF8574: setting bus speed to ");
	DEBUG_PRINT(I2C_BUS_SPEED);
	DEBUG_PRINTLN(".");
	Wire.begin(I2C_SDA_PIN, I2C_SDC_PIN);
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
	expander.attachInterrupt(0, onButtonRoom5, FALLING);
	expander.attachInterrupt(1, onButtonRoom6, FALLING);
	expander.attachInterrupt(2, onButtonRoom7, FALLING);
	expander.attachInterrupt(3, onButtonRoom8, FALLING);
	expander.attachInterrupt(4, onButtonRoom1, FALLING);
	expander.attachInterrupt(5, onButtonRoom2, FALLING);
	expander.attachInterrupt(6, onButtonRoom3, FALLING);
	expander.attachInterrupt(7, onButtonRoom4, FALLING);

#ifdef INCLUDE_WIFI
	//InitWifi();
	/*
	if (InitWifi())
		InitBlynk();
	*/
#endif

	DEBUG_PRINTLN("FastLED: Initializing color palettes");
	InitColorNames();
	maxColNr = ColorNames.size();

	DEBUG_PRINTLN("FastLED: Initializing LED strip");
	initStrip(true, true);
	DEBUG_PRINT("FastLED: Amount of LEDs: ");
	DEBUG_PRINTLN(PIXEL_COUNT);

	DEBUG_PRINTLN("FastLED: Starting LED strip");
	startStrip();

	/*
	DEBUG_PRINTLN("FastLED: Setting up and starting single group");
	SetEffect(currGrpNr, defaultFxNr);
	SetColors(currGrpNr, defaultColNr);
	startGroup(currGrpNr);
*/
	for (int i = groupRoomOffset; i < groupRoomCount + groupRoomOffset; i++)
	{
		DEBUG_PRINT("FastLED: Setting up and starting group #");
		DEBUG_PRINTLN(i);
#ifdef DO_NOT_START_FX_ON_INIT
		SetEffect(i, defaultFxNr, false);
#else
		SetEffect(i, defaultFxNr, true);
#endif
#ifdef START_WITH_RANDOM_COLOR_PALETTE
		// Random default color palette
		int rndCol = random8(0, ColorNames.size() - 1);
		currColNr[i] = rndCol;
		SetColors(i, rndCol);
#else
		// Static default color palette
		SetColors(i, defaultColNr);
#endif
		//startGroup(i);
	}
	currGrpNr = groupRoomOffset;
}

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
		WiFi.status() != WL_CONNECTED*/) // Both buttons pressed
	{
		DEBUG_PRINTLN("Loop: both buttons pressed, entering WiFi-setup.");
#ifdef INCLUDE_WIFI
		InitWifi(false, true);
#endif
		buttonColPressed = false;
		buttonFxPressed = false;
		bothButtonsPressed = false;
#ifdef INCLUDE_WIFI
		/*
		if (InitWifi(false, true))
			InitBlynk();
		*/
#endif
	}
	else
	{
		if (buttonFxPressed && btnFxReleased) // Button was released
		{
			DEBUG_PRINTLN("Loop: button 'FX' pressed and released.");
			NextEffect();
			buttonFxPressed = false;
		}
		if (buttonColPressed && btnColReleased) // Button was released
		{
			DEBUG_PRINTLN("Loop: button 'colors' pressed and releases.");
			NextColor();
			buttonColPressed = false;
			/*
			// Visual feedback button was pressed
			for (int p = 0; p < 3; p++)
			{
				FastLED.setBrightness(0);
				FastLED.show();
				FastLED.delay(100);
				FastLED.setBrightness(globalBrightness);
				FastLED.show();
				FastLED.delay(100);
			}
			*/
		}
		if (buttonStartStopPressed && btnStartStopReleased)
		{
			DEBUG_PRINTLN("Loop: button 'Start/Stop' pressed and released.");
			StartStopEffect(currGrpNr);
			buttonStartStopPressed = false;
		}
	}

	if (!ledsStarted)
	{
		//DEBUG_PRINTLN("Loop: LEDs not started, leaving loop.");
		return;
	}

	bool isActiveMainGrp = false;
	bool ledsUpdated = false;
	for (int i = groupRoomOffset; i < groupRoomCount + groupRoomOffset; i++)
	{
		NeoGroup *neoGroup = &(neoGroups.at(i));
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

		if (i == groupRoomOffset - 1) // is all rooms' LED group?
			isActiveMainGrp = neoGroup->Active;
		if (i > (groupRoomOffset - 1) && isActiveMainGrp)
			break; // Don't update other groups if main group (all LEDs) is active
	}
#ifdef INCLUDE_WIFI
/*
	else
	{
		static int lastBlynkUpdate = 0;
		if ((millis() - lastBlynkUpdate) > 50)
		{
			lastBlynkUpdate = millis();
			//DEBUG_PRINTLN("Loop: Blynk.Run()");
			if (WiFi.status() == WL_CONNECTED)
			{
				Blynk.run();
			}
		}
	}
	*/
#endif

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
		NeoGroup *neoGroupStatus = &(neoGroups.at(1));
		for (int i = 0; i < groupRoomCount; i++)
		{
#ifdef DEBUG_LOOP
			DEBUG_PRINT(i);
#endif
			NeoGroup *neoGroup = &(neoGroups.at(i + groupRoomOffset));
			uint8_t currentBrightness = 128;
			if (i == (currGrpNr - groupRoomOffset))
			{
				currentBrightness = quadwave8(currentBrightnessIdx);
				currentBrightness = 15 + currentBrightness - (currentBrightness >> 4);
#ifdef DEBUG_LOOP
				DEBUG_PRINT("(active)");
#endif
			}
			if (!neoGroup->Active)
			{
				// lower brightness of status LED if FX for group is not running
				currentBrightness = currentBrightness >> 2;
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
			CRGB statusColor = neoGroup->GetColorFromPaletteAt(statusIdx, currentBrightness);
			neoGroupStatus->SetPixel(i, statusColor);
#ifdef DEBUG_LOOP
			DEBUG_PRINT(i);
			DEBUG_PRINT("...");
#endif
		}
#ifdef DEBUG_LOOP
		DEBUG_PRINTLN("DONE");
#endif
		statusIdx++;
		currentBrightnessIdx += 3;
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
