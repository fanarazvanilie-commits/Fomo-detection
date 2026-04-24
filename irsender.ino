#include <Arduino.h>

const uint8_t IR_LED_PIN = 3;   // change to your actual pin

// Pattern: ON, OFF, ON, OFF, ON
// Total = 40 + 25 + 40 + 25 + 40 = 170 ms
const uint16_t patternDurations[] = {40, 25, 40, 25, 40};
const size_t patternLength = sizeof(patternDurations) / sizeof(patternDurations[0]);

const uint16_t cooldownMs = 300;

void sendPattern() {
    bool ledState = HIGH;   // starts ON

    for (size_t i = 0; i < patternLength; i++) {
        digitalWrite(IR_LED_PIN, ledState);
        delay(patternDurations[i]);
        ledState = !ledState;
    }

    // make sure LED stays OFF after pattern
    digitalWrite(IR_LED_PIN, LOW);
}

void setup() {
    pinMode(IR_LED_PIN, OUTPUT);
    digitalWrite(IR_LED_PIN, LOW);

    Serial.begin(115200);
    delay(1000);

    Serial.println("Fixed IR blink pattern sender started");
    Serial.println("Pattern: 40ms ON, 25ms OFF, 40ms ON, 25ms OFF, 40ms ON");
    Serial.println("Cooldown: 300 ms");
    Serial.println("Total cycle: 470 ms");
}

void loop() {
    sendPattern();
    Serial.println("Sent");
    delay(cooldownMs);
}