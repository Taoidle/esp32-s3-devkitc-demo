#include <SPI.h>
#include <FS.h>
#include <SD.h>

// VCC 5V

#define SD_MISO     13
#define SD_MOSI     11
#define SD_SCLK     12
#define SD_CS       10


void setup() {
    Serial.begin(115200);

    SPIClass spi = SPIClass(HSPI);
    spi.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);


    if (!SD.begin(SD_CS, spi)) {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) {
        Serial.println("MMC");
    } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    Serial.println("initialisation done.");
}

void loop() {

}
