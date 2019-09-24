/*
  PWM music beeper - PWM controllable music beeper
  Created by Dima Tsvetkov, 2019-09-22.
  GNU General public license V3
*/

#define PWM_PIN 3             // RX's PWM pin
#define BEEPER_PIN 4          // Beeper's pin on Arduino

#define TEMPO_NOTE 4          // 1/4 note BPM
#define BPM 240               // 1/4-note beats per minute
#define PAUSE_LENGTH 100      // Pause to separate beeps in milliseconds
#define REST_LENGTH 8         // Length of the rest note, 8 = Eighth rest

const byte melody[] { // Darude - Sandstorm
// 4/4 common time signature
// 0 = eighth rest
// 8 = 1/8 note / quaver
// 4 = 1/4 note / crotchet
// 2 = 1/2 note / minim

// |- - - -   - - - -| |- - - -   - - - -|
    8,8,8,8,  2,        0,0,0,0,  0,0,4,
    8,8,8,8,  2,        0,0,0,0,  0,0,0,0,

    8,8,8,8,  2,        8,8,8,8,  2,
    8,8,8,8,  2,        8,8,8,8,  2,

    8,8,8,8,  8,8,8,8,  8,8,8,8,  8,8,8,8,

    2,        2,        0,0,0,0,  0,0,0,0,

    8,8,8,8,  4,8,8,    8,8,8,8,  4,8,8,
    8,8,8,8,  4,8,8,    8,8,8,8,  4,8,8,
    8,8,8,8,  4,8,8,    8,8,8,8,  4,8,8,
    8,8,8,8,  4,8,8,    8,8,8,8,  4,8,8,
    8,8,8,8,  4,8,8,    8,8,8,8,  4,8,8,
    8,8,8,8,  4,8,8,    8,8,8,8,  4,8,8,
    8,8,8,8,  4,8,8,    8,8,8,8,  4,8,8,
    8,8,8,8,  4,8,8,    8,8,8,8,  2,
    2,        0,0,0,0,  0,0,0,0,  0,0,0,0,
    100 // end mark, do not remove
  };

bool debug = true;

int currentNote = 0;
bool addAfterNotePause = false;
unsigned long timer = 0;
int noteLength = 0;

void setup() {
  pinMode(BEEPER_PIN, OUTPUT);
  pinMode(PWM_PIN, INPUT);
  if(debug) Serial.begin(115200);
}

void runNote() {
  if((millis() - timer) > noteLength) {
    if(addAfterNotePause) { // after note pause
      noteLength = PAUSE_LENGTH;
      addAfterNotePause = false;
      digitalWrite(BEEPER_PIN, LOW);
      timer = millis();
    }
    else if(melody[currentNote] > 0) {
      noteLength = (60000/(BPM/TEMPO_NOTE)/melody[currentNote])-PAUSE_LENGTH;
      addAfterNotePause = true;
      digitalWrite(BEEPER_PIN, HIGH);
      timer = millis();
      if(debug) {
          Serial.print(currentNote);
          Serial.print(": note "); Serial.print(melody[currentNote]);
          Serial.print(", bpm: "); Serial.print(BPM);
          Serial.print(", length: "); Serial.print(noteLength);
          Serial.print(", timer: "); Serial.println(timer);
      }
      currentNote = currentNote+1;
    }
    else {
      noteLength = 60000/(BPM/TEMPO_NOTE)/REST_LENGTH;
      digitalWrite(BEEPER_PIN, LOW);
      timer = millis();
      currentNote = currentNote+1;
    }
    if(melody[currentNote] == 100) currentNote = 0; // start from the beginning when reached to the end
  }
}

void loop() {
 
  if(pulseIn(PWM_PIN, HIGH) > 2000) { // for normal arduino 1300, for attiny85 with updated bootloader 2000
    runNote();
  }
  else {
    digitalWrite(BEEPER_PIN, LOW);
    timer = 0;
    currentNote = 0;
    noteLength = 0;
  }

}
