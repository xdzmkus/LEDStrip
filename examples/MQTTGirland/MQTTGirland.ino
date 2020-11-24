#include "data_sensitive.h"
#include "LEDLine.h"

#define ANALOG_PIN A0 // RTC battery voltage input
#define LED_PIN D1    // D1 leds pin (mapped to D5 on NodeMCU !!!)
#define BTN_PIN D0    // D0 button pin

/*********** WiFi Access Point **************/
#include <ESP8266WiFi.h>

#define WLAN_SSID       _WLAN_SSID_
#define WLAN_PASS       _WLAN_PASS_
#define WLAN_HOSTNAME   _WLAN_HOSTNAME_

/*********** MQTT Server ********************/
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define MQTT_SERVER      _MQTT_SERVER_
#define MQTT_SERVERPORT  _MQTT_SERVERPORT_
#define MQTT_USERNAME    _MQTT_USERNAME_
#define MQTT_KEY         _MQTT_KEY_

#define MQTT_TOPIC_PUB MQTT_USERNAME"/current/state"
#define MQTT_TOPIC_SUB1 MQTT_USERNAME"/new/effect"
#define MQTT_TOPIC_SUB2 MQTT_USERNAME"/new/onoff"

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
//WiFiClientSecure client;  // use WiFiClientSecure for SSL

// Setup the MQTT client class by passing in the WiFi client
//Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT);

Adafruit_MQTT_Publish girlandState = Adafruit_MQTT_Publish(&mqtt, MQTT_TOPIC_PUB);
Adafruit_MQTT_Subscribe girlandEffect = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB1, MQTT_QOS_1);
Adafruit_MQTT_Subscribe girlandOnOff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB2, MQTT_QOS_1);

#include <Denel.h>
using namespace denel;
Button btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

/*********** RTC DS1307 module **************/
#include <RTClib.h>
// D1 - SCL; 
// D2 - SDA
RTC_DS1307 rtc;

/*********** WS2812B leds *******************/
#include <FastLED.h>
#define NUM_LEDS 396
#define CURRENT_LIMIT 16000
#define MAX_BRIGHTNESS 300
#define MIN_BRIGHTNESS 30

CRGB leds[NUM_LEDS];

LEDLine ledLine(leds, NUM_LEDS);

uint16_t brightness = MIN_BRIGHTNESS;

void handleButtonEvent(const Button* button, BUTTON_EVENT eventType)
{
    switch (eventType)
    {
    case BUTTON_EVENT::Clicked:
        publishState();
        break;
    case BUTTON_EVENT::DoubleClicked:
        ledLine.setNextEffect();
        ledLine.resume();
        Serial.print(F("NEXT: ")); Serial.println(ledLine.getEffectName());
        break;
    case BUTTON_EVENT::RepeatClicked:
        brightness -= 20;
        if (brightness > MAX_BRIGHTNESS)
            brightness = MAX_BRIGHTNESS;
        FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, 255));
        FastLED.show();
        break;
    case BUTTON_EVENT::LongPressed:
        ledLine.pause();
        FastLED.clear(true);
        Serial.println(F("OFF"));
        break;
    default:
        break;
    }
}

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT);       // Initialize the BUILTIN_LED pin as an output

    Serial.begin(115200);

    setup_WiFi();

    setup_MQTT();

    setup_LED();

    setup_RTC();

    btn.setEventHandler(handleButtonEvent);
}

void setup_WiFi()
{
    Serial.println();
    Serial.print(F("Connecting to ")); Serial.println(WLAN_SSID);

    WiFi.mode(WIFI_STA);                  // Set the ESP8266 to be a WiFi-client
    WiFi.hostname(WLAN_HOSTNAME);
    WiFi.begin(WLAN_SSID, WLAN_PASS);

    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level but actually the LED is on; this is because it is active low on the ESP-01)
        delay(500);
        Serial.print(".");
        digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
        delay(200);
    }

    Serial.println("");
    Serial.println(F("WiFi connected"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
}

void setup_RTC()
{
    Serial.println("");
    Serial.println(F("RTC init..."));

    if (!rtc.begin())
    {
        Serial.println(F("Couldn't find RTC"));
        abort();
    }

    if (!rtc.isrunning())
    {
        Serial.println(F("RTC is NOT running!"));
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    DateTime now = rtc.now();

    Serial.print(F("Current date & time: "));
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
}

void setup_LED()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.setBrightness(brightness);
    FastLED.clear(true);
}

void setup_MQTT()
{
    girlandEffect.setCallback(newEffect_callback);
    mqtt.subscribe(&girlandEffect);
    
    girlandOnOff.setCallback(onoff_callback);
    mqtt.subscribe(&girlandOnOff);
}

void onoff_callback(uint32_t x)
{
    Serial.print(F("on/off with code: "));
    Serial.println(x);

    switch (x)
    {
    case OFF_CODE:
        FastLED.clear(true);
        ledLine.pause();
        break;
    case PAUSE_CODE:
        ledLine.pause();
        break;
    case ON_CODE:
        ledLine.resume();
        break;
    default:
        publishState();
        break;
    }
}

void newEffect_callback(char* data, uint16_t len)
{
    Serial.print(F("new effect requested: "));
    Serial.println(data);

    if (ledLine.setEffectByName(data))
        ledLine.resume();
}

void publishState()
{
    auto batteryLevel = analogRead(ANALOG_PIN);
    auto currentEffect = ledLine.getEffectName();
    auto currentTime = rtc.now().secondstime();

    Serial.print(F("Battery level: ")); Serial.println(batteryLevel);
    Serial.print(F("Current effect: ")); Serial.println(currentEffect);
    Serial.print(F("Time: ")); Serial.println(currentTime);

    String state = "STATE: ";
    state += currentTime;
    state += ' ';
    state += batteryLevel;
    state += ' ';
    state += currentEffect;

    Serial.print(F("Publish message: ")); Serial.println(state.c_str());

    if (!girlandState.publish(state.c_str()))
    {
        Serial.println(F("Publish Message Failed"));
    }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void mqtt_loop()
{
    if (!mqtt.connected())
    {
        uint8_t ret(mqtt.connect());

        Serial.println(F("Connecting to MQTT... "));
        if (ret != 0)
        {
            Serial.println(mqtt.connectErrorString(ret));
            Serial.println(F("Retry MQTT connection ..."));
            mqtt.disconnect();
            return;
        }
        else
        {
            Serial.println(F("MQTT Connected!"));
        }
    }

    mqtt.processPackets(10);
}

void loop()
{
    btn.check();

    if (ledLine.paint())
    {
        FastLED.show();
    }

    mqtt_loop();
}
