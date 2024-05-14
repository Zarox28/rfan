// ----- LIBRARIES IMPORTATION-----
#include <IRremote.hpp>

// ----- PINS DEFINITION -----
#define IR_pin 7
#define relay_1 8
#define relay_2 9

// ----- RELAYS STATES -----
bool relay_1_state = true;
bool relay_2_state = true;

// ----- TIMERS -----
bool relay_1_timer_state = false;
bool relay_2_timer_state = false;
unsigned long previous_relay_1 = 0;
unsigned long previous_relay_2 = 0;
unsigned long time_relay_1 = 0;
unsigned long time_relay_2 = 0;

// ----- SETUP -----
void setup () {
  Serial.begin(9600);
  IrReceiver.begin(IR_pin, ENABLE_LED_FEEDBACK);
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  digitalWrite(relay_1, HIGH);
  digitalWrite(relay_2, HIGH);
}

// ----- TIMER SETTING -----
void Set_Timer (unsigned long time, int relay) {
  if (relay == 1) {
    digitalWrite(relay_1, LOW);
    time_relay_1 = time;
    previous_relay_1 = millis();
    relay_1_timer_state = true;
  }

  if (relay == 2) {
    digitalWrite(relay_2, LOW);
    time_relay_2 = time;
    previous_relay_2 = millis();
    relay_2_timer_state = true;
  }
}

// ----- MAIN LOOP -----
void loop () {
  if (relay_1_timer_state && (millis() - previous_relay_1 >= time_relay_1)) {
    digitalWrite(relay_1, HIGH);
    relay_1_timer_state = false;
  }

  if (relay_2_timer_state && (millis() - previous_relay_2 >= time_relay_2)) {
    digitalWrite(relay_2, HIGH);
    relay_2_timer_state = false;
  }

  if (IrReceiver.decode()) {
    switch (IrReceiver.decodedIRData.decodedRawData) {
      case 0xE916FF00  :
        digitalWrite(relay_1, HIGH);
        digitalWrite(relay_2, HIGH);

        relay_1_timer_state = false;
        relay_2_timer_state = false;

        relay_1_state = true;
        relay_2_state = true;
      break;

      case 0xF30CFF00 :
        digitalWrite(relay_1, relay_1_state ? LOW : HIGH);
        relay_1_state = !relay_1_state;
      break;

      case 0xE718FF00  :
        digitalWrite(relay_2, relay_2_state ? LOW : HIGH);
        relay_2_state = !relay_2_state;
      break;

      case 0xF708FF00  :
        if (!relay_1_timer_state) {
          Set_Timer(1800000, 1);
        }
      break;

      case 0xE31CFF00  :
        if (!relay_1_timer_state) {
          Set_Timer(3600000, 1);
        }
      break;

      case 0xA55AFF00  :
        if (!relay_1_timer_state) {
          Set_Timer(7200000, 1);
        }
      break;

      case 0xBD42FF00  :
        if (!relay_2_timer_state) {
          Set_Timer(1800000, 2);
        }
      break;

      case 0xAD52FF00  :
        if (!relay_2_timer_state) {
          Set_Timer(3600000, 2);
        }
      break;

      case 0xB54AFF00  :
        if (!relay_2_timer_state) {
          Set_Timer(7200000, 2);
        }
      break;
    }
  }

  IrReceiver.resume();
}