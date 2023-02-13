
#include <Arduino.h>
/*
 * Pretend your DMX lights are neopixels using DFRobot shield and 4 channel
 * lights + DMX_master.ino library
 * Lee Wilkins November 2018
 * https://github.com/LeeRobot/DMX-Pretends-To-Be-Neopixel
 */

/*
  DMX_Master.ino - Example code for using the Conceptinetics DMX library
  Copyright (c) 2013 W.A. van der Meeren <danny@illogic.nl>.  All right reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Conceptinetics.h>
//
// CTC-DRA-10-R2 ISOLATED DMX-RDM SHIELD JUMPER INSTRUCTIONS
//
// Set !EN (right hand side) when uploading
// Set EN (left hand side) or no jumper when running
//
// Set DE jumper
// Set TX-uart jumper
// Set RX-uart jumper
//
// If you are controlling lights for example:
// Channel 1 is red first lamp
// Channel 2 is green
// Channel 3 is blue
// Channel 4 is white
//
// Channel spacing is set to 10
// So for second lamp:
// Channel 11 is red
// Channel 12 is green
// Channel 13 is blue
// Channel 14 is white
//
// Third lamp Channel 21 is red, and so on.

#define CHANNEL_SPACING 10       // 10, 20, 30....  light chanels. This will work for 4 or 8 channel lights
#define DMX_MASTER_CHANNELS 512 // has to be
#define RXEN_PIN 2
#define NUM_LAMPS 3

int index = 0;
struct pixelBuffer
{ // Struct for buffer
    int lamp;
    int red;
    int green;
    int blue;
    int white;
};
pixelBuffer pBuff[NUM_LAMPS];
 // array of buffer pieces the size of the amount of NUM_LAMPS
// A little macro that takes a percentage and chnages it to 0-255
//
#define MAX_VALUE 255
#define PER_TO_VAL(percent) ((MAX_VALUE / 100) * percent)
typedef enum DmxCh
{            // Modifiers for each channel,
    RED = 1, // red is the first, no modifier
    GREEN = 2,
    BLUE = 3,
    WHITE = 4 // white is the last, add 3 to wherever you start

} DmxCh;

// direct set a color
void set_rgb_value(int lampNumber, int red, int green, int blue, int white);
void sequenceBuffer();
void sequenceDirect();
void clearBuffer();

DMX_Master dmx_master(DMX_MASTER_CHANNELS, RXEN_PIN);
void setup()
{
    dmx_master.enable();
    int startChannel = 1;
    int endChannel = DMX_MASTER_CHANNELS;
    int byteValue = PER_TO_VAL(0);
    dmx_master.setChannelRange(startChannel, endChannel, byteValue);
}

void loop()
{
    //   set_rgb_value(1, 0, 255, 0,0); /// direct buffer style setting a pixel
    sequenceDirect(); // Direct! GO ZZgo NAO
    //   sequenceBuffer(); // Buff buffer
}

void setPixel(int lampNumber, int red, int green, int blue, int white)
{
    pBuff[index].lamp = lampNumber;
    pBuff[index].red = red;
    pBuff[index].green = green;
    pBuff[index].blue = blue;
    pBuff[index].white = white;
}

void showPixels()
{
    for (int i = 0; i < index; i++)
    {
        int channelNumber = pBuff[i].lamp * CHANNEL_SPACING;
        // Set each of the colors
        dmx_master.setChannelValue(channelNumber + RED, pBuff[i].red);
        dmx_master.setChannelValue(channelNumber + GREEN, pBuff[i].green);
        dmx_master.setChannelValue(channelNumber + BLUE, pBuff[i].blue);
        dmx_master.setChannelValue(channelNumber + WHITE, pBuff[i].white);
    }
    clearBuffer();
}

void clearBuffer()
{
    for (int i = 0; i < index + 1; i++)
    {
        pBuff[i].lamp = i;
        pBuff[i].red = 0;
        pBuff[i].green = 0;
        pBuff[i].blue = 0;
        pBuff[i].white = 0;
    }
    index = 0;
}

// direct set a color
void set_rgb_value(int lampNumber, int red, int green, int blue, int white)
{
    int channelNumber = lampNumber * CHANNEL_SPACING;
    // Set each of the colors
    dmx_master.setChannelValue(channelNumber + RED, red);
    dmx_master.setChannelValue(channelNumber + GREEN, green);
    dmx_master.setChannelValue(channelNumber + BLUE, blue);
    dmx_master.setChannelValue(channelNumber + WHITE, white);
}


void sequenceBuffer() {
  for (int i = 0; i < 255; i++) {
    for (int j = 0; j < NUM_LAMPS; j++) {
      setPixel(j, i, 255, 0, 0);
    }
    showPixels();
    delay(5);
  }
  for (int i = 255; i > 0; i--) {
    for (int j = 0; j < NUM_LAMPS; j++) {
      setPixel(j, i, 255, 0, 0);
    }
    showPixels();
    delay(5);
  }
}
void sequenceDirect() { //  using no buffer, direct control style
  for (int i = 0; i < 255; i++) {
    for (int j = 0; j < NUM_LAMPS; j++) {
      set_rgb_value(j, i, 255, 0, 0);
    }
    delay(5);
  }
  for (int i = 255; i > 0; i--) {
    for (int j = 0; j < NUM_LAMPS; j++) {
      set_rgb_value(j, i, 255, 0, 0);
    }
    delay(5);
  }
}
