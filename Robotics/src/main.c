#include <Arduino.h>
#include <stdbool.h>

// Pinuri pentru LED-uri
#define ledRGBRed  6
#define ledRGBGreen 5
#define ledL1  10
#define ledL2  9
#define ledL3 8
#define ledL4 7

// Pinuri pentru butoane
#define startButton  3
#define stopButton  2

// Variabile pentru starea sistemului
bool stationFree = true;
int chargeLevel = 0;
unsigned long lastBlinkTime = 0;
unsigned long stopPressStartTime = 0;
bool charging = false;
bool stopButtonLongPress = false;

// setarea culorii LED-ului RGB
void setRGBColor(bool red, bool green) {
  digitalWrite(ledRGBRed, red);
  digitalWrite(ledRGBGreen, green);
}

//  resetarea LED-urilor
void resetLEDs() {
  digitalWrite(ledL1, LOW);
  digitalWrite(ledL2, LOW);
  digitalWrite(ledL3, LOW);
  digitalWrite(ledL4, LOW);
}

// clipirea tuturor LED-urilor de mai multe ori
void blinkAllLEDs(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledL1, HIGH);
    digitalWrite(ledL2, HIGH);
    digitalWrite(ledL3, HIGH);
    digitalWrite(ledL4, HIGH);
    delay(500);
    digitalWrite(ledL1, LOW);
    digitalWrite(ledL2, LOW);
    digitalWrite(ledL3, LOW);
    digitalWrite(ledL4, LOW);
    delay(500);
  }
}

// oprir incarcare si resetarea sistemului
void stopCharging() {
  blinkAllLEDs(3); // Clipire de trei ori
  charging = false;
  stationFree = true;
  stopButtonLongPress = false;
  setRGBColor(false, true); // LED RGB verde 
  resetLEDs();
  chargeLevel = 0;
}

void setup() {
  // Configurarea pinurilor
  pinMode(ledRGBRed, OUTPUT);
  pinMode(ledRGBGreen, OUTPUT);
  pinMode(ledL1, OUTPUT);
  pinMode(ledL2, OUTPUT);
  pinMode(ledL3, OUTPUT);
  pinMode(ledL4, OUTPUT);
  pinMode(startButton, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);

  // Setarea stare initiala
  setRGBColor(false, true); // LED RGB verde 
  resetLEDs();
}

void loop() {
  // Verificare apăsare buton start
  if (digitalRead(startButton) == LOW && stationFree) {
    charging = true;
    stationFree = false;
    chargeLevel = 1;
    setRGBColor(true, false); // LED RGB roșu (stația este ocupată)
    lastBlinkTime = millis();
  }

  // Verificare apasare lunga but stop
  if (digitalRead(stopButton) == LOW) {
    if (stopPressStartTime == 0) {
      stopPressStartTime = millis();
    } else if (millis() - stopPressStartTime > 1000) {
      stopButtonLongPress = true;
    }
  } else {
    stopPressStartTime = 0;
  }

  // Gestionare oprire fortata
  if (stopButtonLongPress && charging) {
    stopCharging();
  }

  // Gestionare incarcare progresiv
  if (charging) {
    if (millis() - lastBlinkTime >= 3000) {
      lastBlinkTime = millis();

      // Actualizare stadiu de incarcare
      if (chargeLevel == 1) {
        digitalWrite(ledL1, HIGH);
        delay(500);
        digitalWrite(ledL1, LOW);
         delay(500);
         digitalWrite(ledL1, HIGH);
         delay(500);
         digitalWrite(ledL1, LOW);
         delay(500);
         digitalWrite(ledL1, HIGH);
      } else if (chargeLevel == 2) {
        digitalWrite(ledL2, HIGH);
        delay(500);
        digitalWrite(ledL2, LOW);
         delay(500);
         digitalWrite(ledL2, HIGH);
         delay(500);
         digitalWrite(ledL2, LOW);
         delay(500);
         digitalWrite(ledL2, HIGH);
      } else if (chargeLevel == 3) {
        digitalWrite(ledL3, HIGH);
        delay(500);
        digitalWrite(ledL3, LOW);
         delay(500);
         digitalWrite(ledL3, HIGH);
         delay(500);
         digitalWrite(ledL3, LOW);
         delay(500);
         digitalWrite(ledL3, HIGH);
      } else if (chargeLevel == 4) {
        digitalWrite(ledL4, HIGH);
        delay(500);
        digitalWrite(ledL4, LOW);
         delay(500);
         digitalWrite(ledL4, HIGH);
         delay(500);
         digitalWrite(ledL4, LOW);
         delay(500);
         digitalWrite(ledL4, HIGH);
        blinkAllLEDs(3);
        stopCharging(); // Oprire la finalizarea incarcarii
      }
      chargeLevel++;
    }
  }
}
