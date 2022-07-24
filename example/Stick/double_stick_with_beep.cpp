#include <Arduino.h>

#define PS2_1_X_PIN 4
#define PS2_1_Y_PIN 5
#define PS2_1_Y_BUTTON 6

#define PS2_2_X_PIN 7
#define PS2_2_Y_PIN 15
#define PS2_2_Y_BUTTON 16

#define BEEP_PIN 9

void setup_ps2();
void setup_beep();

void beep(uint8_t PIN, uint8_t time, uint32_t time_len);

void setup()
{
    Serial.begin(115200);
    Serial.println("setup ...");

    setup_ps2();
    setup_beep();

    Serial.println("setup ok");
}

void loop()
{
    uint16_t ps2_lx = analogRead(PS2_1_X_PIN);
    uint16_t ps2_ly = analogRead(PS2_1_Y_PIN);
    uint16_t ps2_lb = digitalRead(PS2_1_Y_BUTTON);
    uint16_t ps2_rx = analogRead(PS2_2_X_PIN);
    uint16_t ps2_ry = analogRead(PS2_2_Y_PIN);
    uint16_t ps2_rb = digitalRead(PS2_2_Y_BUTTON);

    Serial.print("lX: ");
    Serial.print(ps2_lx);
    Serial.print(" ");
    Serial.print("lY: ");
    Serial.print(ps2_ly);
    Serial.print(" ");
    Serial.print("lZ: ");
    Serial.println(ps2_lb);
    Serial.print("rX: ");
    Serial.print(ps2_rx);
    Serial.print(" ");
    Serial.print("rY: ");
    Serial.print(ps2_ry);
    Serial.print(" ");
    Serial.print("rZ: ");
    Serial.println(ps2_rb);

    if (ps2_lb == LOW || ps2_rb == LOW)
    {
        Serial.println("in if");
        beep(BEEP_PIN, 3, 50);
    }

}

void setup_ps2()
{
    // set ps2-1
    pinMode(PS2_1_X_PIN, INPUT); // ps2-1-x
    pinMode(PS2_1_Y_PIN, INPUT); // ps2-1-y
    pinMode(PS2_1_Y_BUTTON, INPUT_PULLUP); // ps2-1-button
    digitalWrite(PS2_1_Y_BUTTON, HIGH);

    // set ps2-2
    pinMode(PS2_2_X_PIN, INPUT); // ps2-2-x
    pinMode(PS2_2_Y_PIN, INPUT); // ps2-2-y
    pinMode(PS2_2_Y_BUTTON, INPUT_PULLUP); // ps2-2-button
    digitalWrite(PS2_2_Y_BUTTON, HIGH);
}

void setup_beep()
{
    // set bee
    pinMode(BEEP_PIN, OUTPUT);
    digitalWrite(BEEP_PIN, HIGH);
}

void beep(uint8_t PIN, uint8_t time, uint32_t time_len)
{
    Serial.print("beep: ");
    Serial.println(PIN);

    for (int i = 0; i < time; i++)
    {
        Serial.print("for: ");
        Serial.println(i);
        digitalWrite(PIN, LOW);
        delay(time_len);
        digitalWrite(PIN, HIGH);
        delay(time_len);
    }

}
