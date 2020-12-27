#if true // && __has_include("my_data_sensitive.h")
#include "my_data_sensitive.h"
#else

#define WLAN_SSID		    "your wifi name"
#define WLAN_PASS		    "and password"
#define WLAN_HOSTNAME	    "connect with hostname"

#define MQTT_SERVER		    "127.0.0.1"
#define MQTT_SERVERPORT	    1883
#define MQTT_USERNAME	    "your mqtt username"
#define MQTT_KEY		    "and password"

#define MQTT_TOPIC_PUB      MQTT_USERNAME"/current/state"
#define MQTT_TOPIC_SUB1     MQTT_USERNAME"/new/effect"
#define MQTT_TOPIC_SUB2     MQTT_USERNAME"/new/onoff"

#define ON_CODE		        6735
#define OFF_CODE	        2344
#define PAUSE_CODE	        2747

#endif

#define LED_PIN D3    // D1 leds pin (connected to D5 on my NodeMCU 1.0 !!!)
#define BTN_PIN D6    // D6 button pin

/*********** WS2812B leds *******************/
#include <FastLED.h>
#define NUM_LEDS 256
#define CURRENT_LIMIT 8000
#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 20

uint16_t brightness = MAX_BRIGHTNESS / 2;

CRGB leds[NUM_LEDS];

/*********** LED Line Effects ***************/
#include "LEDLine.h"
LEDLine ledLine(leds, NUM_LEDS);

#include <Ticker.h>
#define EFFECT_DURATION_SEC 45
Ticker tickerEffects;
volatile boolean f_publishState = true;

/********** Touch button module *************/
#include <Denel_Button.h>
Denel_Button btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

/*********** WiFi Client ********************/
#include <ESP8266WiFi.h>

/*********** MQTT Server ********************/
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
//WiFiClientSecure client;  // use WiFiClientSecure for SSL

// Setup the MQTT client class by passing in the WiFi client
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);
//Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT);

Adafruit_MQTT_Publish girlandState = Adafruit_MQTT_Publish(&mqtt, MQTT_TOPIC_PUB);
Adafruit_MQTT_Subscribe girlandEffect = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB1, MQTT_QOS_1);
Adafruit_MQTT_Subscribe girlandOnOff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB2, MQTT_QOS_1);

/********************************************/

void handleTimer()
{
    if (ledLine.isRunning()) ledLine.setNextEffect();
    f_publishState = true;
}

void handleButtonEvent(const Denel_Button* button, BUTTON_EVENT eventType)
{
    switch (eventType)
    {
    case BUTTON_EVENT::Clicked:
        f_publishState = true;
        break;
    case BUTTON_EVENT::DoubleClicked:
        ledLine.setNextEffect();
        f_publishState = true;
        Serial.print(F("NEXT: ")); Serial.println(ledLine.getEffectName());
        break;
    case BUTTON_EVENT::RepeatClicked:
        brightness += MIN_BRIGHTNESS;
        if (brightness > MAX_BRIGHTNESS + MIN_BRIGHTNESS * 2) brightness = 0;
        FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
        Serial.print(F("BRIGHTNESS: ")); Serial.println(brightness);
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
        f_publishState = true;
        break;
    }
}

void newEffect_callback(char* data, uint16_t len)
{
    Serial.print(F("new effect requested: "));
    Serial.println(data);

    ledLine.setEffectByName(data);
}

void publishState()
{
    auto currentEffect = (ledLine.getEffectName() == nullptr || !ledLine.isRunning()) ? "OFF" : ledLine.getEffectName();

    Serial.print(F("Publish message: ")); Serial.println(currentEffect);

    if (!girlandState.publish(currentEffect))
    {
        Serial.println(F("Publish Message Failed"));
    }
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);       // Initialize the BUILTIN_LED pin as an output

    Serial.begin(115200);

    setup_LED();

    setup_WiFi();

    setup_MQTT();

    tickerEffects.attach(EFFECT_DURATION_SEC, handleTimer);

    btn.setEventHandler(handleButtonEvent);

    ledLine.resume();

    ledLine.setNextEffect();
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
        digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level but actually the LED is on; this is because it is active low on the ESP-01)
        delay(500);
        Serial.print(".");
        digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
        delay(200);
    }

    Serial.println("");
    Serial.println(F("WiFi connected"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
}

void setup_MQTT()
{
    girlandEffect.setCallback(newEffect_callback);
    mqtt.subscribe(&girlandEffect);

    girlandOnOff.setCallback(onoff_callback);
    mqtt.subscribe(&girlandOnOff);
}

void setup_LED()
{
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
    FastLED.clear(true);
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

    mqtt_loop();

    if (f_publishState)
    {
        f_publishState = false;

        publishState();
    }

    if (ledLine.paint())
    {
        FastLED.show();
    }
}