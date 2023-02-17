#ifndef ARD_PROXIMITY_LED_H
#define ARD_PROXIMITY_LED_H

#include <Arduino.h>
#include <FastLED.h>

class ArdProximityLed
{
   public:
    ArdProximityLed() = default;

    /**
     * @brief Trigger pin
     */
    uint8_t trig_pin = 0;
    /**
     * @brief Echo pin
     */
    uint8_t echo_pin = 0;
    /**
     * @brief Speed of sound in cm/us
     *
     * Speed of sound at 20 degrees C in air is 344 m/s
     */
    double speed = 0.0344;
    /**
     * @brief Distance range.
     */
    double distance_range[2] = {50.0, 300.0};
    /**
     * @brief Hue output range.
     */
    uint8_t hue_range[2] = {40, 120};
    /**
     * @brief Saturation output range.
     */
    uint8_t sat_range[2] = {200, 255};
    /**
     * @brief Value output range.
     */
    uint8_t val_range[2] = {80, 200};

    /**
     * @brief Set pins as input or output.
     */
    void Setup();

    /**
     * @brief Measure proximity in centimeters.
     *
     * @return Proximity in centimeters.
     */
    double MeasureProximity() const;

    /**
     * @brief Generate color from proximity sensor.
     *
     * @return Color output.
     */
    CRGB GetColorFromProximity() const;

    /**
     * @brief Scale distance to value.
     *
     * @param distance     Input distance.
     * @param min_Value    Minimum output value associated with minimum distance.
     * @param max_Value    Maximum output value associated with minimum distance.
     * @param min_distance Minimum distance.
     * @param max_distance Maximum distance.
     * @return             Output value.
     */
    static uint8_t ProximityDistanceToValue(double distance, const uint8_t min_value, const uint8_t max_value,
                                            const double min_distance, const double max_distance);
};

void ArdProximityLed::Setup()
{
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
}

inline double ArdProximityLed::MeasureProximity() const
{
    // Send pulse at get time it takes to receive
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    const uint32_t duration = pulseIn(echo_pin, HIGH);
    // convert speed to distance in centimeters
    const double distance_cm = static_cast<double>(duration) * speed / 2.0;
    return distance_cm;
}

inline CRGB ArdProximityLed::GetColorFromProximity() const
{
    // Get distance and scale
    const double distance = MeasureProximity();
    const uint8_t hue = ProximityDistanceToValue(distance, hue_range[0], hue_range[1],
                                                 distance_range[0], distance_range[1]);
    const uint8_t sat = ProximityDistanceToValue(distance, sat_range[0], sat_range[1],
                                                 distance_range[0], distance_range[1]);
    const uint8_t val = ProximityDistanceToValue(distance, val_range[0], val_range[1],
                                                 distance_range[0], distance_range[1]);
    // convert to RGB
    return CRGB().setHSV(hue, sat, val);
}

inline uint8_t ArdProximityLed::ProximityDistanceToValue(
    double distance, const uint8_t min_value, const uint8_t max_value,
    const double min_distance, const double max_distance)
{
    if (distance < min_distance)
    {
        return min_value;
    }
    else if (distance > max_distance)
    {
        return max_value;
    }
    else if ((max_distance - min_distance) < 1.0e-6f)
    {
        return min_value;
    }
    else
    {
        const double slope = (static_cast<double>(max_value) - static_cast<double>(min_value)) /
                             (max_distance - min_distance);
        return static_cast<uint8_t>((distance - min_distance) * slope + 0.5) + min_value;
    }
}

#endif
