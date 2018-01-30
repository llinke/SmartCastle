/*
* Project SmartCastle
* Description:
* Author:
* Date:
*/

#include "FastLedInclude.h"
#include "ColorPalettes.h"
#include <Arduino.h>
//#include <ArduinoSTL.h>
#include <vector>
#include <map>
#include "NeoGroup.cpp"

#include <ESP8266WiFi.h>	  //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>		  //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h> //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>	  //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

/*
#define BLYNK_PRINT Serial
#define BLYNK_MAX_SENDBYTES 512 // Default is 128
#include <BlynkSimpleEsp8266.h>
// Device 'SmartCastle':
const char blynkAuth[] = "4abfe0577ae745aca3d5d5d9f37911b7";
*/
const String wifiApName = "AP_SmartCastle";

//SYSTEM_MODE(AUTOMATIC);

const int ConfigureAPTimeout = 10;

//int ByteReceived;

// Helper macro
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Static size
//struct CRGB leds[PIXEL_COUNT];
// Dynamic size:
struct CRGB *leds = NULL;
int pixelCount = PIXEL_COUNT;
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
int currGrpNr = 2;

//std::vector<NeoGroup *> neoGroups;
std::vector<NeoGroup> neoGroups;
int groupCount = 0;

int globalBrightness = 64;

static bool buttonFxPressed = false;
static bool buttonColPressed = false;
static bool bothButtonsPressed = false;

bool InitWifi(bool useWifiCfgTimeout = true, bool forceReconnect = false)
{
	Serial.println("WIFI ------------------------------------------------------");
	if (!forceReconnect && WiFi.status() == WL_CONNECTED)
	{
		Serial.println("WiFi: already connected...");
		return true; // Is already connected...
	}

	if (ledsInitialized)
	{
		FastLED.clear(true);
		fill_solid(leds, pixelCount, CRGB::Blue);
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
	Serial.println("WiFi Manager trying to connect...");
	if (useWifiCfgTimeout)
	{
		Serial.print("You have ");
		Serial.print(ConfigureAPTimeout);
		Serial.println(" seconds for configuration if required.");
		wifiManager.setConfigPortalTimeout(ConfigureAPTimeout);
	}
	bool connected = wifiManager.autoConnect(wifiApName.c_str());
	//or use this for auto generated name ESP + ChipID
	//wifiManager.autoConnect();
	//if you get here you have connected to the WiFi
	if (ledsInitialized)
	{
		fill_solid(leds, pixelCount, connected ? CRGB::Green : CRGB::Red);
		FastLED.show();
	}
	if (connected)
		Serial.println("Wifi is connected...yay!!!");
	else
		Serial.println("!!! WIFI NOT CONNECTED !!!");
	delay(5000);

	return connected;
}

/*
void InitBlynk()
{
	if (WiFi.status() != WL_CONNECTED)
		return;

	Serial.println("BLYNK -----------------------------------------------------");
	Serial.println("Blynk: authenticating");
	Blynk.config(blynkAuth);
	Blynk.connect();

	SendMenusToBlynk();
	SendStatusToBlynk();
}

void SendMenusToBlynk()
{
	if (WiFi.status() != WL_CONNECTED)
		return;

	Serial.println("Blynk: assigning menu 'FX'");
	BlynkParamAllocated fxItems(128);
	fxItems.add("Zufällig");
	fxItems.add("Lauflicht");
	fxItems.add("Dynamisch");
	fxItems.add("Wasser");
	fxItems.add("Konfetti");
	fxItems.add("Farbwechsel");
	Blynk.setProperty(V2, "labels", fxItems);

	Serial.println("Blynk: assigning menu 'Colors'");
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

	Serial.println("Blynk: sending current status");
	Blynk.virtualWrite(V0, ledsStarted);
	Blynk.virtualWrite(V2, currFxNr.at(currGrpNr) + 1);
	Blynk.virtualWrite(V4, currColNr.at(currGrpNr) + 1);
	Blynk.virtualWrite(V5, currGlitter.at(currGrpNr));
	Blynk.virtualWrite(V6, globalBrightness);
	Blynk.virtualWrite(V7, currFps.at(currGrpNr));
}
*/

int initStrip(int ledCount, bool doStart = false, bool playDemo = true)
{
	if (ledsInitialized)
	{
		return doStart ? startStrip() : pixelCount;
	}

	Serial.println("LEDStrip --------------------------------------------------");
	Serial.println("Allocating memory for LED strip data.");
	leds = (struct CRGB *)malloc(ledCount * sizeof(struct CRGB));
	Serial.println("Assigning LEDs to FastLED.");
	FastLED.addLeds<PIXEL_TYPE, PIXEL_PIN>(leds, ledCount);
	//FastLED.setMaxPowerInVoltsAndMilliamps(5,3000);
	FastLED.setBrightness(globalBrightness);
	FastLED.setDither(0);
	FastLED.clear(true);
	FastLED.show();

	pixelCount = ledCount;
	ledsInitialized = true;

	if (playDemo)
	{
		Serial.println("Playing little demo effect.");
		for (int dot = 0; dot < ledCount; dot++)
		{
			leds[dot] = CHSV(random8(), 255, 255);
			FastLED.show();
			delay(10);
		}
		delay(500);
	}

	InitWifi();
	/*
	if (InitWifi())
		InitBlynk();
	*/

	if (playDemo)
	{
		Serial.println("Fading away demo effect.");
		for (int fade = 0; fade < 20; fade++)
		{
			fadeToBlackBy(leds, ledCount, 20);
			FastLED.show();
			delay(50);
		}
		Serial.println("Clearing LEDs.");
		FastLED.clear(true);
		FastLED.show();
	}

	Serial.println("Adding special groups.");
	neoGroups.clear();
	// Group 0: all LEDs
	addGroup("All LEDs", 0, pixelCount, 0);
	// Group 1: Status LEDs
	addGroup("Status LEDs", 0, 8, 0);
	// Group 2: all LEDs
	addGroup("All Castle LEDs", 8, pixelCount - 8, 0);

	/*
	Serial.println("Adding groups for room.");
	addGroup("Room 1", 8, 8, 0);
	addGroup("Room 2", 8, 8, 0);
	addGroup("Room 3", 8, 8, 0);
	addGroup("Room 4", 8, 8, 0);
	addGroup("Room 5", 8, 8, 0);
	addGroup("Room 6", 8, 8, 0);
	addGroup("Room 7", 8, 8, 0);
	addGroup("Room 8", 8, 8, 0);
	*/

	return doStart ? startStrip() : pixelCount;
}

int startStrip()
{
	if (!ledsInitialized)
		return -1;

	ledsStarted = true;
	return pixelCount;
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
	if ((ledFirst >= pixelCount) ||
		(ledCount <= 0) ||
		(ledFirst + ledCount) > pixelCount)
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

void SetEffect(int grpNr, int fxNr, bool startFx = false)
{
	Serial.println("SetEffect ---------------------------------------------------");
	Serial.print("Fx: Configuring LED effect #");
	Serial.print(fxNr);
	Serial.print(" for group #");
	Serial.println(grpNr);

	if (fxNr == 0)
	{
		Serial.println("Fx: Choosing random effect.");
		SetEffect(grpNr, random8(1, maxFxNr), startFx);
		return;
	}

	String fxPatternName = "";
	pattern fxPattern = pattern::STATIC;
	uint16_t fxLength = 255;
	int fxGlitter = currGlitter.at(grpNr);
	uint8_t fxFps = currFps.at(grpNr);
	mirror fxMirror = MIRROR0;

	switch (fxNr)
	{
	case 1: // Wave
		fxPatternName = "Wave";
		fxPattern = pattern::WAVE;
		fxLength = (pixelCount * 1.5); // 48;
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
	Serial.print("Fx: Changing effect to '");
	Serial.print(fxPatternName);
	Serial.println("'");
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
	Serial.println("SetColors --------------------------------------------------");
	Serial.print("Col: Configuring LED colors #");
	Serial.print(colNr);
	Serial.print(" for group #");
	Serial.println(grpNr);

	if (colNr == 0)
	{
		Serial.println("Col: Choosing random color palette.");
		SetColors(grpNr, random8(1, maxColNr));
		return;
	}

	String palKey = ColorNames[colNr - 1];
	Serial.print("Col: Changing color palette to '");
	Serial.print(palKey);
	Serial.println("'");
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
	Serial.print("CONTROL: Button 'FX' pressed, changing effect number to: ");
	Serial.println(currFxNr.at(currGrpNr));
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
	Serial.print("CONTROL: Button 'Colors' pressed, changing color number to: ");
	Serial.println(currColNr.at(currGrpNr));
	SetColors(currGrpNr, currColNr.at(currGrpNr));
}

/*
BLYNK_WRITE(V0) // Button "Power"
{
	int pinValue = param.asInt();
	Serial.println("BLYNK -----------------------------------------------------");
	Serial.print("Blynk-Button 'POWER' pressed: ");
	Serial.println(pinValue);
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
	Serial.println("BLYNK -----------------------------------------------------");
	Serial.print("Blynk-Button 'FX' pressed: ");
	Serial.println(pinValue);
	if (pinValue == 1)
	{
		NextEffect();
		Blynk.virtualWrite(V2, (currFxNr.at(currGrpNr)) + 1);
	}
}

BLYNK_WRITE(V2) // DropDown "FX"
{
	int pinValue = param.asInt() - 1;
	Serial.println("BLYNK -----------------------------------------------------");
	Serial.print("Blynk-DropDown 'FX' selected: ");
	Serial.println(pinValue);
	NextEffect(pinValue);
}

BLYNK_WRITE(V3) // Button "Color"
{
	int pinValue = param.asInt();
	Serial.println("BLYNK -----------------------------------------------------");
	Serial.print("Blynk-Button 'Colors' pressed: ");
	Serial.println(pinValue);
	if (pinValue == 1)
	{
		NextColor();
		Blynk.virtualWrite(V4, (currColNr.at(currGrpNr)) + 1);
	}
}

BLYNK_WRITE(V4) // DropDown "Colors"
{
	int pinValue = param.asInt() - 1;
	Serial.println("BLYNK -----------------------------------------------------");
	Serial.print("Blynk-DropDown 'Colors' selected: ");
	Serial.println(pinValue);
	NextColor(pinValue);
}

BLYNK_WRITE(V5) // Slider "Glitter"
{
	int pinValue = param.asInt();
	Serial.println("BLYNK -----------------------------------------------------");
	Serial.print("Blynk-Slider 'Glitter' selected: ");
	Serial.println(pinValue);
	currGlitter.at(currGrpNr) = pinValue;
	NeoGroup *neoGroup = &(neoGroups.at(currGrpNr));
	neoGroup->ChangeGlitter(currGlitter.at(currGrpNr));
}

BLYNK_WRITE(V6) // Slider "Brightness"
{
	int pinValue = param.asInt();
	Serial.println("BLYNK -----------------------------------------------------");
	Serial.print("Blynk-Slider 'Brightness' selected: ");
	Serial.println(pinValue);
	globalBrightness = pinValue;
	FastLED.setBrightness(pinValue);
	FastLED.setDither(0);
}

BLYNK_WRITE(V7) // Slider "Speed"
{
	int pinValue = param.asInt();
	Serial.println("BLYNK -----------------------------------------------------");
	Serial.print("Blynk-Slider 'Speed' selected: ");
	Serial.println(pinValue);
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

void setup()
{
	Serial.begin(115200);

	InitColorNames();
	maxColNr = ColorNames.size();

	pinMode(BUTTON_PIN_FX, INPUT_PULLUP);
	attachInterrupt(BUTTON_PIN_FX, onButtonFx, FALLING);
	pinMode(BUTTON_PIN_COL, INPUT_PULLUP);
	attachInterrupt(BUTTON_PIN_COL, onButtonCol, FALLING);

	//InitWifi();
	/*
	if (InitWifi())
		InitBlynk();
	*/

	Serial.println("BOOT/SETUP ------------------------------------------------");
	Serial.println("Setup: Setting up SmartCastle for Arduino");
	Serial.println("Setup: Initializing LED strip");
	initStrip(pixelCount, true, true);
	Serial.print("Setup: Amount of LEDs: ");
	Serial.println(pixelCount);

	Serial.println("Setup: Starting LED strip");
	startStrip();

	SetEffect(currGrpNr, defaultFxNr);
	SetColors(currGrpNr, defaultColNr);
	startGroup(currGrpNr);
}

// Main loop
void loop()
{
	//Serial.println("LOOP ------------------------------------------------------");
	bool btnFxReleased = buttonFxPressed && !digitalRead(BUTTON_PIN_FX);
	bool btnColReleased = buttonColPressed && !digitalRead(BUTTON_PIN_COL);
	if (bothButtonsPressed &&
		(btnFxReleased | btnColReleased) /*&&
		WiFi.status() != WL_CONNECTED*/) // Both buttons pressed
	{
		Serial.println("Loop: both buttons pressed, entering WiFi-setup.");
		InitWifi(false, true);
		buttonColPressed = false;
		buttonFxPressed = false;
		bothButtonsPressed = false;
		/*
		if (InitWifi(false, true))
			InitBlynk();
		*/
	}
	else
	{
		if (buttonFxPressed && btnFxReleased) // Button was released
		{
			Serial.println("Loop: button 'FX' pressed and released.");
			NextEffect();
			buttonFxPressed = false;
		}
		if (buttonColPressed && btnColReleased) // Button was released
		{
			Serial.println("Loop: button 'colors' pressed and releases.");
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
	}

	if (!ledsStarted)
	{
		//Serial.println("Loop: LEDs not started, leaving loop.");
		return;
	}

	bool isActiveMainGrp = false;
	bool ledsUpdated = false;
	for (int i = 2; i < neoGroups.size(); i++)
	{
		NeoGroup *neoGroup = &(neoGroups.at(i));
		if (neoGroup->LedCount <= pixelCount)
		{
			//Serial.print("Loop: Updating group ");
			//Serial.println(i);
			ledsUpdated |= neoGroup->Update();
		}

		if (i == 2)
			isActiveMainGrp = neoGroup->Active;
		if (i > 2 && isActiveMainGrp)
			break; // Don't update other groups if main group (all LEDs) is active
	}
	if (ledsUpdated)
	{
		//Serial.println("Loop: Refreshing LEDs.");
		FastLED.show();
	}
	/*
	else
	{
		static int lastBlynkUpdate = 0;
		if ((millis() - lastBlynkUpdate) > 50)
		{
			lastBlynkUpdate = millis();
			//Serial.println("Loop: Blynk.Run()");
			if (WiFi.status() == WL_CONNECTED)
			{
				Blynk.run();
			}
		}
	}
	*/
	//pixelCount = count; //PIXEL_COUNT;
	groupCount = neoGroups.size() - 3; // Don't count main groups (all LEDs)
}
