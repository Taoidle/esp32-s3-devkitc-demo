#include <Esp.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include <IRrecv.h>
#include <IRac.h>
#include <IRtext.h>
#include <IRutils.h>

const uint16_t kIrLed = 8;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

const uint16_t kRecvPin = 14;
const uint32_t kBaudRate = 115200;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 15;
const uint8_t kTolerancePercentage = kTolerance;

IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;

// Example of data captured by IRrecvDumpV2.ino
uint16_t rawData[67] = {9000, 4500, 650, 550, 650, 1650, 600, 550, 650, 550,
                        600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650,
                        600, 550, 650, 1650, 650, 1650, 650, 550, 600, 1650,
                        650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550,
                        650, 550, 650, 550, 600, 550, 650, 550, 650, 550,
                        650, 1650, 600, 550, 650, 1650, 650, 1650, 650, 1650,
                        650, 1650, 650, 1650, 650, 1650, 600};
// Example Samsung A/C state captured from IRrecvDumpV2.ino
uint8_t samsungState[kSamsungAcStateLength] = {
        0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
        0x01, 0xE2, 0xFE, 0x71, 0x40, 0x11, 0xF0};

TaskHandle_t task_1;
TaskHandle_t task_2;

void esp_ir_send(void *parameter);
void esp_ir_recv(void *parameter);


void setup()
{
    irsend.begin();
    Serial.begin(115200, SERIAL_8N1);

    irrecv.setTolerance(kTolerancePercentage);
    irrecv.enableIRIn();

    xTaskCreatePinnedToCore(esp_ir_send, "Task_1", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, 1, &task_1, 0);
    xTaskCreatePinnedToCore(esp_ir_recv, "Task_1", CONFIG_ESP_MINIMAL_SHARED_STACK_SIZE, NULL, 1, &task_2, 1);
}

void loop()
{
}


void esp_ir_send(void *parameter)
{
    (void) parameter;

    for (;;)
    {
        Serial.println("NEC");
        irsend.sendNEC(0x00FFE01FUL);
        delay(500);
        Serial.println("Sony");
        irsend.sendSony(0xa90, 12, 2);  // 12 bits & 2 repeats
        delay(500);
        Serial.println("a rawData capture from IRrecvDumpV2");
        irsend.sendRaw(rawData, 67, 38);  // Send a raw data capture at 38kHz.
        delay(500);
        Serial.println("a Samsung A/C state from IRrecvDumpV2");
        irsend.sendSamsungAC(samsungState);
        delay(500);
    }
}

void esp_ir_recv(void *parameter)
{
    (void) parameter;

    for (;;)
    {
        if (irrecv.decode(&results))
        {
            uint32_t now = millis();
            Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
            // Check if we got an IR message that was to big for our capture buffer.
            if (results.overflow)
                Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
            // Display the library version the message was captured with.
            Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_STR "\n");
            // Display the tolerance percentage if it has been change from the default.
            if (kTolerancePercentage != kTolerance)
                Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
            // Display the basic output of what we found.
            Serial.print(resultToHumanReadableBasic(&results));
            // Display any extra A/C info if we have it.
            String description = IRAcUtils::resultAcToString(&results);
            if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);
            yield();  // Feed the WDT as the text output can take a while to print.

#if LEGACY_TIMING_INFO
            // Output legacy RAW timing info of the result.
        Serial.println(resultToTimingInfo(&results));
        yield();  // Feed the WDT (again)
#endif  // LEGACY_TIMING_INFO
            // Output the results as source code
            Serial.println(resultToSourceCode(&results));
            Serial.println();    // Blank line between entries
            yield();             // Feed the WDT (again)
        }
    }
}