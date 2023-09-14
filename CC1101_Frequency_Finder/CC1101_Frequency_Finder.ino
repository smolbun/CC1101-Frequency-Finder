#include <Arduino.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <SPI.h>

#define SCK_PIN 18
#define MISO_PIN 16
#define MOSI_PIN 19
#define SS_PIN 17

bool initcc1101() {
#ifdef ARDUINO_ARCH_RP2040
  SPI.setRX(MISO_PIN);
  SPI.setCS(SS_PIN);
  SPI.setSCK(SCK_PIN);
  SPI.setTX(MOSI_PIN);
#endif
  ELECHOUSE_cc1101.setSpiPin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  if (!ELECHOUSE_cc1101.getCC1101()) {
    return false;
  }

  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.setRxBW(80.0);    // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
  ELECHOUSE_cc1101.setModulation(2);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
  return true;
}

int highestRssi = 0;           
uint8_t highestRssiPosition = -1;  

void getHighestRssi(int arr[], int size) {
  highestRssi = arr[0];     
  highestRssiPosition = 0; 

  for (int i = 1; i < size; i++) {
    if (arr[i] > highestRssi) {
      highestRssi = arr[i];     
      highestRssiPosition = i; 
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Started!");
  while (!initcc1101()) {
    Serial.println("CC1101 init failed");
    delay(1000);
  }
}
const int threshold = -85;  // Adjust threshold
const uint8_t freqCount = 8;
const float freq[freqCount] = { 310.0, 315.0, 318.0, 330.0, 390.0, 433.0, 433.92, 868.35 };
int rssi[freqCount] = { 0 };

void loop() {
  static uint8_t i = 0;
  uint32_t lastTime = millis();
  ELECHOUSE_cc1101.setMHZ(freq[i]);
  ELECHOUSE_cc1101.SetRx();
  while (millis() - lastTime < 15) {  // listen for 15ms
    delay(5);
    rssi[i] = ELECHOUSE_cc1101.getRssi();
  }

  i++;
  if (i > freqCount - 1) {
    getHighestRssi(rssi, freqCount);
    if (highestRssiPosition != -1 && highestRssi != 0) {
      if (highestRssi > threshold) {
        Serial.printf("%.2f MHz RSSI: %d\n", freq[highestRssiPosition], highestRssi);
        delay(400);
      }
      highestRssi = 0;
      highestRssiPosition = -1;
    }
    i = 0;
  }
  ELECHOUSE_cc1101.setSidle();
}
