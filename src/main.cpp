
#include <limits.h>
#include <Arduino.h>

#include "ArdDmx.h"
#include "ArdProximityLed.h"

#define PROXIMITY1_TRIG_PIN 10
#define PROXIMITY1_ECHO_PIN 13

#define PROXIMITY2_TRIG_PIN 9
#define PROXIMITY2_ECHO_PIN 12

#define DMX_PIN 2

ArdProximityLed g_proximity1;
ArdProximityLed g_proximity2;
ArdDmx g_dmx;

void setup()
{
    // Proximity
    // ---------

    g_proximity1.trig_pin = PROXIMITY1_TRIG_PIN;
    g_proximity1.echo_pin = PROXIMITY1_ECHO_PIN;

    g_proximity2.trig_pin = PROXIMITY2_TRIG_PIN;
    g_proximity2.echo_pin = PROXIMITY2_ECHO_PIN;

    g_proximity1.hue_range[0] = 40;
    g_proximity1.hue_range[1] = 120;

    g_proximity2.hue_range[0] = 120;
    g_proximity2.hue_range[1] = 200;

    // DMX
    // ---

    // Parameters
    ArdDmxParameters params;
    params.num_leds = 6;
    params.pin = DMX_PIN;
    params.color_order = kArdDmxRGB;
    params.dmx_channel_offset = 10;
    params.dmx_channels_per_light = 10;
    // initialize
    eArdDmxStatus status = g_dmx.Initialize(params);
    if (status != kArdDmxSuccess)
    {
        do {
            delay(1000);
         } while (true);
    }
}

// *** REPLACE FROM HERE ***
// ---> here we declare the effect function <---

void loop() {
    // Set color values form proximity sensors
    CRGB light1 = g_proximity1.GetColorFromProximity();
    CRGB light2 = g_proximity2.GetColorFromProximity();
    g_dmx.SetPixel(0, light1.r, light1.g, light1.b);
    g_dmx.SetPixel(1, light2.r, light2.g, light2.b);
    g_dmx.ShowStrip();
    delay(10);

    // ---> here we call the effect function <---
}

// ---> here we define the effect function <---
// *** REPLACE TO HERE ***
