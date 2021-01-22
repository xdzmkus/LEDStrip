#if true // && __has_include("my_data_sensitive.h")
#include "my_data_sensitive.h"
#else

#define WLAN_SSID           "your wifi name"
#define WLAN_PASS           "and password"
#define WLAN_HOSTNAME       "connect with hostname"

#define UPDATE_PATH         "/firmware"

#define MQTT_SERVER         "127.0.0.1"
#define MQTT_SERVERPORT     1883
#define MQTT_USERNAME       "your mqtt username"
#define MQTT_KEY            "and password"

#define MQTT_TOPIC_PUB      MQTT_USERNAME"/get/state"
#define MQTT_TOPIC_SUB1     MQTT_USERNAME"/set/effect"
#define MQTT_TOPIC_SUB2     MQTT_USERNAME"/set/action"

#define ON_CODE             6735
#define OFF_CODE            2344
#define NEXT_CODE           2747

static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDSzCCAjMCCQD2ahcfZAwXxDANBgkqhkiG9w0BAQsFADCBiTELMAkGA1UEBhMC
VVMxEzARBgNVBAgMCkNhbGlmb3JuaWExFjAUBgNVBAcMDU9yYW5nZSBDb3VudHkx
EDAOBgNVBAoMB1ByaXZhZG8xGjAYBgNVBAMMEXNlcnZlci56bGFiZWwuY29tMR8w
HQYJKoZIhvcNAQkBFhBlYXJsZUB6bGFiZWwuY29tMB4XDTE4MDMwNjA1NDg0NFoX
DTE5MDMwNjA1NDg0NFowRTELMAkGA1UEBhMCQVUxEzARBgNVBAgMClNvbWUtU3Rh
dGUxITAfBgNVBAoMGEludGVybmV0IFdpZGdpdHMgUHR5IEx0ZDCCASIwDQYJKoZI
hvcNAQEBBQADggEPADCCAQoCggEBAPVKBwbZ+KDSl40YCDkP6y8Sv4iNGvEOZg8Y
X7sGvf/xZH7UiCBWPFIRpNmDSaZ3yjsmFqm6sLiYSGSdrBCFqdt9NTp2r7hga6Sj
oASSZY4B9pf+GblDy5m10KDx90BFKXdPMCLT+o76Nx9PpCvw13A848wHNG3bpBgI
t+w/vJCX3bkRn8yEYAU6GdMbYe7v446hX3kY5UmgeJFr9xz1kq6AzYrMt/UHhNzO
S+QckJaY0OGWvmTNspY3xCbbFtIDkCdBS8CZAw+itnofvnWWKQEXlt6otPh5njwy
+O1t/Q+Z7OMDYQaH02IQx3188/kW3FzOY32knER1uzjmRO+jhA8CAwEAATANBgkq
hkiG9w0BAQsFAAOCAQEAnDrROGRETB0woIcI1+acY1yRq4yAcH2/hdq2MoM+DCyM
E8CJaOznGR9ND0ImWpTZqomHOUkOBpvu7u315blQZcLbL1LfHJGRTCHVhvVrcyEb
fWTnRtAQdlirUm/obwXIitoz64VSbIVzcqqfg9C6ZREB9JbEX98/9Wp2gVY+31oC
JfUvYadSYxh3nblvA4OL+iEZiW8NE3hbW6WPXxvS7Euge0uWMPc4uEcnsE0ZVG3m
+TGimzSdeWDvGBRWZHXczC2zD4aoE5vrl+GD2i++c6yjL/otHfYyUpzUfbI2hMAA
5tAF1D5vAAwA8nfPysumlLsIjohJZo4lgnhB++AlOg==
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEA9UoHBtn4oNKXjRgIOQ/rLxK/iI0a8Q5mDxhfuwa9//FkftSI
IFY8UhGk2YNJpnfKOyYWqbqwuJhIZJ2sEIWp2301OnavuGBrpKOgBJJljgH2l/4Z
uUPLmbXQoPH3QEUpd08wItP6jvo3H0+kK/DXcDzjzAc0bdukGAi37D+8kJfduRGf
zIRgBToZ0xth7u/jjqFfeRjlSaB4kWv3HPWSroDNisy39QeE3M5L5ByQlpjQ4Za+
ZM2yljfEJtsW0gOQJ0FLwJkDD6K2eh++dZYpAReW3qi0+HmePDL47W39D5ns4wNh
BofTYhDHfXzz+RbcXM5jfaScRHW7OOZE76OEDwIDAQABAoIBAQDKov5NFbNFQNR8
djcM1O7Is6dRaqiwLeH4ZH1pZ3d9QnFwKanPdQ5eCj9yhfhJMrr5xEyCqT0nMn7T
yEIGYDXjontfsf8WxWkH2TjvrfWBrHOIOx4LJEvFzyLsYxiMmtZXvy6YByD+Dw2M
q2GH/24rRdI2klkozIOyazluTXU8yOsSGxHr/aOa9/sZISgLmaGOOuKI/3Zqjdhr
eHeSqoQFt3xXa8jw01YubQUDw/4cv9rk2ytTdAoQUimiKtgtjsggpP1LTq4xcuqN
d4jWhTcnorWpbD2cVLxrEbnSR3VuBCJEZv5axg5ZPxLEnlcId8vMtvTRb5nzzszn
geYUWDPhAoGBAPyKVNqqwQl44oIeiuRM2FYenMt4voVaz3ExJX2JysrG0jtCPv+Y
84R6Cv3nfITz3EZDWp5sW3OwoGr77lF7Tv9tD6BptEmgBeuca3SHIdhG2MR+tLyx
/tkIAarxQcTGsZaSqra3gXOJCMz9h2P5dxpdU+0yeMmOEnAqgQ8qtNBfAoGBAPim
RAtnrd0WSlCgqVGYFCvDh1kD5QTNbZc+1PcBHbVV45EmJ2fLXnlDeplIZJdYxmzu
DMOxZBYgfeLY9exje00eZJNSj/csjJQqiRftrbvYY7m5njX1kM5K8x4HlynQTDkg
rtKO0YZJxxmjRTbFGMegh1SLlFLRIMtehNhOgipRAoGBAPnEEpJGCS9GGLfaX0HW
YqwiEK8Il12q57mqgsq7ag7NPwWOymHesxHV5mMh/Dw+NyBi4xAGWRh9mtrUmeqK
iyICik773Gxo0RIqnPgd4jJWN3N3YWeynzulOIkJnSNx5BforOCTc3uCD2s2YB5X
jx1LKoNQxLeLRN8cmpIWicf/AoGBANjRSsZTKwV9WWIDJoHyxav/vPb+8WYFp8lZ
zaRxQbGM6nn4NiZI7OF62N3uhWB/1c7IqTK/bVHqFTuJCrCNcsgld3gLZ2QWYaMV
kCPgaj1BjHw4AmB0+EcajfKilcqtSroJ6MfMJ6IclVOizkjbByeTsE4lxDmPCDSt
/9MKanBxAoGAY9xo741Pn9WUxDyRplww606ccdNf/ksHWNc/Y2B5SPwxxSnIq8nO
j01SmsCUYVFAgZVOTiiycakjYLzxlc6p8BxSVqy6LlJqn95N8OXoQ+bkwUux/ekg
gz5JWYhbD6c38khSzJb0pNXCo3EuYAVa36kDM96k1BtWuhRS10Q1VXk=
-----END RSA PRIVATE KEY-----
)EOF";

#endif

#define BTN_PIN   16  // D0 - GPIO16 - touch button pin

#define LED_PIN_1 12  // D6 - GPIO12 - left strip
#define LED_PIN_2 13  // D7 - GPIO13 - center strip
#define LED_PIN_3 14  // D5 - GPIO14 - right strip

#define RELAY_PIN 15  // D8 - GPIO15 - relay pin

/*********** WS2812B leds *******************/
#include <FastLED.h>

#define NUM_LEDS_LEFT 8
#define NUM_LEDS_CENTER 8
#define NUM_LEDS_RIGHT 8

#define CURRENT_LIMIT 2000
#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 20

uint16_t brightness = MAX_BRIGHTNESS;

CRGB ledsLeft[NUM_LEDS_LEFT];
CRGB ledsCenter[NUM_LEDS_CENTER];
CRGB ledsRight[NUM_LEDS_RIGHT];

/*********** LED Line Effects ***************/
#include "LEDMultiLine.h"
LEDLine8leds ledLineLeft(ledsLeft, NUM_LEDS_LEFT);
LEDLine8leds ledLineCenter(ledsCenter, NUM_LEDS_CENTER);
LEDLine8leds ledLineRight(ledsRight, NUM_LEDS_RIGHT);

#include <Ticker.h>
#define EFFECT_DURATION_SEC 60
Ticker tickerEffects;

volatile boolean f_publishState = true;

/********** Touch button module *************/
#include <Denel_Button.h>
Denel_Button btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

/*********** WiFi ***************************/
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServerSecure httpServer(443);
ESP8266HTTPUpdateServerSecure httpUpdater;
BearSSL::X509List chain(serverCert);
BearSSL::PrivateKey pk(serverKey);

/*********** MQTT Server ********************/
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <WiFiClient.h>

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;  // use WiFiClientSecure for SSL

// Setup the MQTT client class by passing in the WiFi client
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);

Adafruit_MQTT_Publish   wallLampState  = Adafruit_MQTT_Publish(&mqtt, MQTT_TOPIC_PUB);

Adafruit_MQTT_Subscribe wallLampEffect = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB1, MQTT_QOS_1);
Adafruit_MQTT_Subscribe wallLampAction = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC_SUB2, MQTT_QOS_1);

/********************************************/

void turnOnLeds()
{
    tickerEffects.attach(EFFECT_DURATION_SEC, changeEffect);

    ledLineLeft.resume();
    ledLineCenter.resume();
    ledLineRight.resume();

    digitalWrite(RELAY_PIN, HIGH);

    f_publishState = true;
}

void turnOffLeds()
{
    tickerEffects.detach();

    ledLineLeft.pause();
    ledLineCenter.pause();
    ledLineRight.pause();

    digitalWrite(RELAY_PIN, LOW);

    f_publishState = true;
}

void changeEffect()
{
    tickerEffects.detach();

    ledLineLeft.setNextEffect();
    ledLineCenter.setNextEffect();
    ledLineRight.setNextEffect();

    tickerEffects.attach(EFFECT_DURATION_SEC, changeEffect);

    f_publishState = true;
}

void adjustBrightness()
{
    brightness += MIN_BRIGHTNESS;
    if (brightness > MAX_BRIGHTNESS + MIN_BRIGHTNESS * 2)
    {
        brightness = 0;
    }
    FastLED.setBrightness(constrain(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
}

void handleButtonEvent(const Denel_Button* button, BUTTON_EVENT eventType)
{
    switch (eventType)
    {
    case BUTTON_EVENT::Clicked:
        changeEffect();
        break;
    case BUTTON_EVENT::DoubleClicked:
        turnOnLeds();
        break;
    case BUTTON_EVENT::RepeatClicked:
        adjustBrightness();
        break;
    case BUTTON_EVENT::LongPressed:
        turnOffLeds();
        break;
    default:
        break;
    }
}

void setAction_callback(uint32_t x)
{
    switch (x)
    {
    case ON_CODE:
        turnOnLeds();
        break;
    case OFF_CODE:
        turnOffLeds();
        break;
    case NEXT_CODE:
        changeEffect();
        break;
    default:
        f_publishState = true;
        break;
    }
}

void setEffect_callback(char* data, uint16_t len)
{
    bool success = false;

    success |= ledLineLeft.setEffectByName(data);
    success |= ledLineCenter.setEffectByName(data);
    success |= ledLineRight.setEffectByName(data);

    if (success)
    {
        tickerEffects.detach();
    }
}

void publishState()
{
    String currentEffect = (ledLineLeft.getEffectName() == nullptr || !ledLineLeft.isRunning()) ? "OFF" : ledLineLeft.getEffectName();
    currentEffect += " ";
    currentEffect += (ledLineCenter.getEffectName() == nullptr || !ledLineCenter.isRunning()) ? "OFF" : ledLineCenter.getEffectName();
    currentEffect += " ";
    currentEffect += (ledLineRight.getEffectName() == nullptr || !ledLineRight.isRunning()) ? "OFF" : ledLineRight.getEffectName();

    wallLampState.publish(currentEffect.c_str());
}

void setup_WiFi()
{
    Serial.println();
    Serial.print(F("Connecting to "));
    Serial.println(WLAN_SSID);

    WiFi.mode(WIFI_STA);                  // Set the ESP8266 to be a WiFi-client
    WiFi.hostname(WLAN_HOSTNAME);
    WiFi.begin(WLAN_SSID, WLAN_PASS);

    for (uint8_t s = 0; (WiFi.status() != WL_CONNECTED) && (s < 20); s++)
    {
        digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level but actually the LED is on; this is because it is active low on the ESP-01)
        delay(500);
        Serial.print(".");
        digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
        delay(200);
    }
    if (!WiFi.isConnected())
    {
        Serial.println("Connection Failed! Rebooting...");
        ESP.restart();
    }

    Serial.println("");
    Serial.println(F("WiFi connected"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
}

void setup_OTA()
{
    configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    MDNS.begin(WLAN_HOSTNAME);

    httpServer.getServer().setRSACert(&chain, &pk);
    httpUpdater.setup(&httpServer, UPDATE_PATH, MQTT_USERNAME, MQTT_KEY);
    httpServer.begin();

    MDNS.addService("https", "tcp", 443);
}

void setup_MQTT()
{
    wallLampEffect.setCallback(setEffect_callback);
    mqtt.subscribe(&wallLampEffect);

    wallLampAction.setCallback(setAction_callback);
    mqtt.subscribe(&wallLampAction);
}

void setup_LED()
{
    FastLED.addLeds<WS2812B, LED_PIN_1, GRB>(ledsLeft, NUM_LEDS_LEFT);
    FastLED.addLeds<WS2812B, LED_PIN_2, GRB>(ledsCenter, NUM_LEDS_CENTER);
    FastLED.addLeds<WS2812B, LED_PIN_3, GRB>(ledsRight, NUM_LEDS_RIGHT);
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

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);        // Initialize the BUILTIN_LED pin as an output
    pinMode(RELAY_PIN, OUTPUT);          // Initialize the RELAY pin as an output
    pinMode(BTN_PIN, INPUT_PULLDOWN_16); // Re-Initialize the BUTTON pin as an input pulldown

    digitalWrite(RELAY_PIN, LOW);        // turn off leds

    Serial.begin(115200);

    setup_WiFi();

    setup_OTA();

    setup_MQTT();

    setup_LED();

    btn.setEventHandler(handleButtonEvent);
}

void loop()
{
    MDNS.update();

    httpServer.handleClient();

    mqtt_loop();

    btn.check();

    if (f_publishState)
    {
        f_publishState = false;

        publishState();
    }

    bool refresh = false;

    refresh |= ledLineLeft.refresh();
    refresh |= ledLineCenter.refresh();
    refresh |= ledLineRight.refresh();

    if (refresh)
    {
        FastLED.show();
    }
}