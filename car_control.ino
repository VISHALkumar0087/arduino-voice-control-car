/*
  Arduino Remote & Voice Control Car
  Board: Arduino Uno | Motor driver: L298N | Bluetooth: HC-05

  A phone app sends commands over Bluetooth:
    - Button mode:  single characters  F B L R S  (and 0-9 for speed)
    - Voice mode:   text between * and #, for example "*forward#"
      (apps like "Arduino Bluetooth Voice Control" send this format)

  Wiring (see README):
    HC-05 TX -> D2,  HC-05 RX -> D3 (through a voltage divider)
    L298N: ENA D5, IN1 D6, IN2 D7, IN3 D8, IN4 D9, ENB D10
*/
#include <SoftwareSerial.h>

SoftwareSerial bt(2, 3);  // RX, TX

const int ENA = 5, IN1 = 6, IN2 = 7;
const int IN3 = 8, IN4 = 9, ENB = 10;

int speedValue = 200;      // 0-255 (PWM)
String voiceBuffer = "";
bool inVoice = false;

void setup() {
  Serial.begin(9600);      // debug output on the USB serial monitor
  bt.begin(9600);          // HC-05 default baud rate
  int pins[] = {ENA, IN1, IN2, IN3, IN4, ENB};
  for (int p : pins) pinMode(p, OUTPUT);
  stopCar();
  Serial.println("Car ready");
}

void loop() {
  while (bt.available()) {
    char c = bt.read();

    if (c == '*') { inVoice = true; voiceBuffer = ""; continue; }  // voice text starts
    if (c == '#' && inVoice) {                                      // voice text ends
      inVoice = false;
      handleVoice(voiceBuffer);
      continue;
    }
    if (inVoice) { voiceBuffer += c; continue; }

    handleButton(c);
  }
}

// ---- command handlers ----
void handleButton(char c) {
  switch (c) {
    case 'F': forward();  break;
    case 'B': backward(); break;
    case 'L': left();     break;
    case 'R': right();    break;
    case 'S': stopCar();  break;
    default:
      if (c >= '0' && c <= '9') {            // speed 0..9 -> 0..255
        speedValue = map(c - '0', 0, 9, 0, 255);
        Serial.print("Speed: "); Serial.println(speedValue);
      }
  }
}

void handleVoice(String text) {
  text.trim();
  text.toLowerCase();
  Serial.print("Voice: "); Serial.println(text);

  if (text.indexOf("forward") >= 0 || text.indexOf("go") >= 0)       forward();
  else if (text.indexOf("back") >= 0 || text.indexOf("reverse") >= 0) backward();
  else if (text.indexOf("left") >= 0)                                  left();
  else if (text.indexOf("right") >= 0)                                 right();
  else if (text.indexOf("stop") >= 0)                                  stopCar();
  else if (text.indexOf("faster") >= 0) speedValue = min(255, speedValue + 40);
  else if (text.indexOf("slower") >= 0) speedValue = max(80, speedValue - 40);
}

// ---- motor control ----
void drive(bool l1, bool l2, bool r1, bool r2) {
  digitalWrite(IN1, l1); digitalWrite(IN2, l2);
  digitalWrite(IN3, r1); digitalWrite(IN4, r2);
  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
}

void forward()  { drive(HIGH, LOW,  HIGH, LOW);  }
void backward() { drive(LOW,  HIGH, LOW,  HIGH); }
void left()     { drive(LOW,  HIGH, HIGH, LOW);  }  // spin left: left wheels back, right forward
void right()    { drive(HIGH, LOW,  LOW,  HIGH); }
void stopCar()  { drive(LOW,  LOW,  LOW,  LOW);  }
