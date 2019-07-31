#include "pitches.h"

int led = 4;
int pinoSom = 14;
int melody[] = { NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  tocaSom();
  delay(500);
 // pisca();
}

void tocaSom() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(pinoSom, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
     pisca(noteDuration/4);
    delay(pauseBetweenNotes);
    noTone(pinoSom);
  
  }
}

  void pisca(int tempo) {
    digitalWrite(led, HIGH);
    delay(tempo);
    digitalWrite(led, LOW);
  }
