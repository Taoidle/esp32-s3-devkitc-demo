#include <Arduino.h>
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define RGB_R_PIN 37
#define RGB_G_PIN 36
#define RGB_B_PIN 35

#define BUTTON_K1 2

#define PS2_1_X_PIN 4
#define PS2_1_Y_PIN 5
#define PS2_1_Y_BUTTON 6

#define PS2_2_X_PIN 7
#define PS2_2_Y_PIN 15
#define PS2_2_Y_BUTTON 16

#define BEEP_PIN 9


void setup_led();
void setup_button();
void setup_ps2();
void setup_beep();

void beep(uint8_t PIN, uint8_t time, uint32_t time_len);
void turn_rgb(uint8_t R_PIN, uint8_t G_PIN, uint8_t B_PIN, uint32_t time_len);

void task_button_rgb(void *parameter);
void task_stick_beep(void *parameter);

int red_val = 255;
int green_val = 0;
int blue_val = 0;

void setup()
{
    Serial.begin(115200);

    setup_led();
    setup_button();
    setup_ps2();
    setup_beep();

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);//关闭低电压检测,避免无限重启

    xTaskCreatePinnedToCore(task_stick_beep, "Task_1", 10240, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(task_button_rgb, "Task_2", 10240, NULL, 1, NULL, 1);

}

void loop()
{
}

void task_button_rgb(void *parameter)
{
    (void) parameter;

    for (;;)
    {
        Serial.print("Task Button RGB running on core ");
        Serial.println(xPortGetCoreID());

        if (digitalRead(BUTTON_K1) == LOW)
        {
            turn_rgb(RGB_R_PIN, RGB_G_PIN, RGB_B_PIN, 5);
        }
    }
}

void task_stick_beep(void *parameter)
{
    (void) parameter;

    for (;;)
    {
        Serial.print("Task Stick Beep running on core ");
        Serial.println(xPortGetCoreID());

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
            beep(BEEP_PIN, 3, 50);
        }
    }
}

void setup_led()
{
    pinMode(RGB_R_PIN, OUTPUT);
    pinMode(RGB_G_PIN, OUTPUT);
    pinMode(RGB_B_PIN, OUTPUT);
}

void setup_button()
{
    pinMode(BUTTON_K1, INPUT_PULLUP);
    digitalWrite(BUTTON_K1, HIGH);
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
        vTaskDelay(time_len);
        digitalWrite(PIN, HIGH);
        vTaskDelay(time_len);
    }
}

void turn_rgb(uint8_t R_PIN, uint8_t G_PIN, uint8_t B_PIN, uint32_t time_len)
{
    for (byte i = 0; i < 255; i++)
    {
        red_val--;
        green_val++;
        analogWrite(R_PIN, red_val);
        analogWrite(G_PIN, green_val);
        vTaskDelay(time_len);
    }
    for (byte i = 0; i < 255; i++)
    {
        green_val--;
        blue_val++;
        analogWrite(G_PIN, green_val);
        analogWrite(B_PIN, blue_val);
        vTaskDelay(time_len);
    }
    for (byte i = 0; i < 255; i++)
    {
        blue_val--;
        red_val++;
        analogWrite(B_PIN, blue_val);
        analogWrite(R_PIN, red_val);
        vTaskDelay(time_len);
    }
}
