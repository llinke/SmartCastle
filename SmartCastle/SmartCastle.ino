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
#define FIRST_GROUP_IS_ALL_ROOMS
//#define START_WITH_RANDOM_COLOR_PALETTE
//#define ENABLE_RANDOM_FX
//#define ENABLE_RANDOM_COL
//#define DO_NOT_START_FX_ON_INIT
//#define DEBUG_LOOP

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
// *** Variable and Constamts  Declarations
// **************************************************
#ifdef INCLUDE_WIFI
const String wifiApName = "AP_SmartCastle";
const int ConfigureAPTimeout = 10;
#endif

const uint8_t globalBrightness = 96;
const uint8_t statusBrightness = 64;
const uint8_t activeGroupBrightnessCycleSpeed = 3;

const int buttonLockDuration = 200; // ignore buttons for X ms, prevent prelling
static volatile bool buttonsLocked = false;
static volatile int buttonsLockedAt = 0;

static volatile bool buttonPressedOnI2C = false;
static volatile bool roomWasChanged = false;

#ifdef FIRST_GROUP_IS_ALL_ROOMS
// Test 0: equally sized rooms (one less)
const std::vector<int> groupRoomSizes = {16, 16, 16, 32, 16, 16, 16};
#else
// Test 1: equally sized rooms
//const std::vector<int> groupRoomSizes = {16, 16, 16, 16, 16, 16, 16, 16};
// Test 2: differently sized rooms
//const std::vector<int> groupRoomSizes = {4, 8, 12, 16, 16, 20, 24, 28};
const std::vector<int> groupRoomSizes = {8, 24, 10, 22, 12, 20, 14, 18};
#endif

const std::vector<int> mapRoomToStatusLed = {3, 2, 1, 0, 4, 5, 6, 7};
const std::vector<int> mapI2cButtonToRoom = {4, 5, 6, 7, 3, 2, 1, 0};

int groupRoomTotalSize = 0;
int groupRoomCount = 0;
int groupRoomOffset = 1;
int groupNrAllLeds = 0;
int groupNrAllRooms = -1;
int groupNrStatus = 1;
const int groupStatusCount = 8;
int activeGrpNr = groupRoomOffset;

// Static size
//struct CRGB leds[PIXEL_COUNT];
// Dynamic size:
struct CRGB *leds = NULL;
bool ledsInitialized = false;
bool ledsStarted = false;
// 1: Wave, 2: Dynamic Wave, 3: Noise, 4: Confetti, 5: Fade,
const int maxFxNr = 5;
const int defaultFxNr = 1;
std::vector<int> currFxNr;
int maxColNr = 1; // will be dynamically assigned once palettes are generated
const int defaultColNr = 1;
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
#pragma region Event Handlers
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
#ifdef FIRST_GROUP_IS_ALL_ROOMS
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
#ifdef FIRST_GROUP_IS_ALL_ROOMS
	addGroup("Schloss", groupStatusCount, groupRoomTotalSize, 0);
	groupNrAllRooms = neoGroups.size() - 1;
#endif
	groupRoomOffset = groupNrAllRooms >= 0 ? groupNrAllRooms : neoGroups.size();
	activeGrpNr = 0;

	DEBUG_PRINTLN("Adding groups for rooms.");
	int nextGroupStart = groupStatusCount; // Start after status LEDs
	for (int i = 0; i < groupRoomSizes.size(); i++)
	{
		String roomName = "Raum " + String(i + 1);
		DEBUG_PRINT("  Adding '");
		DEBUG_PRINT(roomName);
		DEBUG_PRINT("' at ");
		DEBUG_PRINT(nextGroupStart);
		DEBUG_PRINT("  with size ");
		DEBUG_PRINTLN(groupRoomSizes[i]);
		addGroup(roomName, nextGroupStart, groupRoomSizes[i], 0);
		nextGroupStart += groupRoomSizes[i];
	}

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

	updateOledRequired = true;

	return 0;
}

int stopGroup(int grpNr, bool stopNow = false)
{
	NeoGroup *neoGroup = &(neoGroups.at(grpNr));
	neoGroup->Stop(stopNow);

	updateOledRequired = true;

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
#ifdef FIRST_GROUP_IS_ALL_ROOMS
	if (offsetGrpNr == groupNrAllRooms)
		stopNow = true; // Main group cannot be faded out, causes flickering
#endif
	if (neoGroup->Active)
		stopGroup(offsetGrpNr, stopNow);
	else
		startGroup(offsetGrpNr);
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
		fxFps /= 2; // half FPS looks better
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
	SetEffect(offsetGrpNr, currFxNr.at(offsetGrpNr), true);
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
	DEBUG_PRINTLN("--- Updating OLED display.");
	DEBUG_PRINTLN("----------------------------------------");
	DEBUG_PRINTLN("--- " + neoGroup->GroupID);
	DEBUG_PRINTLN("--- " + fxPatternName);
	DEBUG_PRINTLN("--- " + fxColorName);
	DEBUG_PRINTLN("----------------------------------------");

#ifdef i2cOLED
	// clear the display
	display.clear();

	display.setColor(OLEDDISPLAY_COLOR::WHITE);
	display.setFont(Nimbus_Sans_L_Regular_Condensed_32);
	display.setTextAlignment(TEXT_ALIGN_CENTER);
	display.drawString(64, -2, neoGroup->GroupID);

	display.setColor(OLEDDISPLAY_COLOR::WHITE);
	display.setFont(Dialog_plain_12);
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
	if (!areButtonsLocked())
	{
		lockButtons();

		DEBUG_PRINT("PCF8574: switching to room ");
		DEBUG_PRINT(roomNo + 1);
		DEBUG_PRINT("->");
		if (roomNo < 0)
		{
			activeGrpNr++;
			if (activeGrpNr >= groupRoomCount)
				activeGrpNr = 0;
		}
		else
		{
			activeGrpNr = constrain(roomNo, 0, groupRoomCount - 1);
		}
		DEBUG_PRINT(activeGrpNr);
		DEBUG_PRINTLN(".");
		roomWasChanged = true;

		updateOledRequired = true;
	}
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
	initStrip(true, true);
	DEBUG_PRINT("FastLED: Amount of LEDs: ");
	DEBUG_PRINTLN(PIXEL_COUNT);

	DEBUG_PRINTLN("FastLED: Starting LED strip");
	startStrip();

	for (int i = 0; i < groupRoomCount; i++)
	{
		int offsetGrpNr = groupRoomOffset + i;
		DEBUG_PRINT("FastLED: Setting up and starting group #");
		DEBUG_PRINTLN(offsetGrpNr);
#ifdef DO_NOT_START_FX_ON_INIT
		bool startFx = false;
#else
		bool startFx = true;
#endif
#ifdef FIRST_GROUP_IS_ALL_ROOMS
		if (offsetGrpNr == groupNrAllRooms)
			startFx = false;
#endif
		SetEffect(offsetGrpNr, defaultFxNr, startFx);
#ifdef START_WITH_RANDOM_COLOR_PALETTE
		// Random default color palette
		int rndCol = random8(0, ColorNames.size() - 1);
		currColNr[offsetGrpNr] = rndCol;
		SetColors(offsetGrpNr, rndCol);
#else
		// Static default color palette
		SetColors(offsetGrpNr, defaultColNr);
#endif
		//startGroup(offsetGrpNr);
	}
	activeGrpNr = 0;

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
		WiFi.status() != WL_CONNECTED*/) // Both buttons pressed
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

	bool isActiveMainGrp = false;
	bool ledsUpdated = false;
	for (int i = 0; i < groupRoomCount; i++)
	{
		int offsetGrpNr = groupRoomOffset + i;
		NeoGroup *neoGroup = &(neoGroups.at(offsetGrpNr));

#ifdef FIRST_GROUP_IS_ALL_ROOMS
		if (groupNrAllRooms >= 0 && offsetGrpNr == groupNrAllRooms) // is all rooms' LED group?
		{
			isActiveMainGrp = neoGroup->Active;
#ifdef DEBUG_LOOP
/*
			DEBUG_PRINT("Loop: Main group active -> ");
			DEBUG_PRINTLN(isActiveMainGrp);
*/
#endif
		}
		if (isActiveMainGrp && offsetGrpNr > groupNrAllRooms)
		{
#ifdef DEBUG_LOOP
			DEBUG_PRINT("Loop: Skipping room from #");
			DEBUG_PRINTLN(i);
#endif
			break; // Don't update other groups if main group (all LEDs) is active
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
#ifdef FIRST_GROUP_IS_ALL_ROOMS
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
}
#pragma endregion