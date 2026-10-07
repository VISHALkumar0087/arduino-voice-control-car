# Arduino Remote & Voice Control Car

Hardware-software robotic vehicle controlled from a smartphone over Bluetooth, with button control and real-time voice commands.

## Components
- Arduino Uno
- HC-05 Bluetooth module
- L298N motor driver + 2 DC gear motors (or a 4-wheel chassis)
- 7.4 V battery pack (motors) and a 9 V or USB supply for the Arduino
- Phone with a Bluetooth controller or voice app, for example "Arduino Bluetooth Voice Control"

## Wiring
| From | To |
|---|---|
| HC-05 TX | Arduino D2 |
| HC-05 RX | Arduino D3, through a voltage divider (1k + 2k) |
| HC-05 VCC / GND | 5 V / GND |
| L298N ENA, IN1, IN2 | D5, D6, D7 |
| L298N IN3, IN4, ENB | D8, D9, D10 |
| L298N 12 V | battery +, and connect the grounds of battery, L298N and Arduino |

## Commands
| Mode | Input | Action |
|---|---|---|
| Buttons | `F` `B` `L` `R` `S` | forward, back, left, right, stop |
| Buttons | `0`-`9` | speed |
| Voice | `*forward#`, `*back#`, `*left#`, `*right#`, `*stop#` | same actions |
| Voice | `*faster#`, `*slower#` | change speed |

## Upload
1. Open `car_control.ino` in the Arduino IDE.
2. Select the board (Arduino Uno) and the port, then click Upload. Unplug the HC-05 RX/TX wires while uploading if it fails.
3. Pair the phone with HC-05 (password `1234` or `0000`), then open the app and connect.

## How it works
The sketch reads characters from Bluetooth. Single letters are button commands. Text between `*` and `#` is a voice command, and keyword matching turns it into motor directions. The `drive()` function sets the four L298N input pins and the PWM speed.

## Tech
Arduino (C/C++), Bluetooth (HC-05), hardware interfacing, PWM motor control
