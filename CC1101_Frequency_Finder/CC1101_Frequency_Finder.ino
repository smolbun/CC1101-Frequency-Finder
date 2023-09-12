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
  ELECHOUSE_cc1101.setRxBW(100.0);    // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
  ELECHOUSE_cc1101.setModulation(2);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
  return true;
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
const int threshold = -75;  // Adjust threshold
const uint8_t freqCount = 5;
const float freq[freqCount] = { 310.0, 315.0, 318.0, 390.0, 433.92 };

void loop() {
  static uint8_t i = 0;
  uint32_t lastTime = millis();
  int rssi = 0;
  ELECHOUSE_cc1101.setMHZ(freq[i]);
  ELECHOUSE_cc1101.SetRx();
  while (millis() - lastTime < 15) {  // listen for 15ms
    delay(5);
    rssi = ELECHOUSE_cc1101.getRssi();
    if (rssi > threshold) {
      Serial.printf("%.2f MHz RSSI: %d\n", freq[i], rssi);
      delay(400);
      break;
    }
  }

  i++;
  i = (i > freqCount - 1) ? 0 : i;
  ELECHOUSE_cc1101.setSidle();
}
