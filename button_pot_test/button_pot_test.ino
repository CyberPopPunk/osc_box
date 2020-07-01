/*
    LEGAL SHTUFF:
Copyright (2020) (CyberPopPunk)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    The Software may not be used by individuals, corporations, governments, or other groups for systems or activities that actively,
    knowingly and/or unknowlingly endanger, harm, or otherwise threaten the physical, mental, economic, or general well-being of 
    individuals or groups in violation of the United Nations Universal Declaration of Human Rights (https://www.un.org/en/universal-declaration-human-rights/).

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This license is derived from the MIT License, as amended to limit the impact of the unethical use of open source software.

  // OSC Box
  TODO:
  - DONE! 1) TEST BUTTONS
         2) ATTACH BUTTONS TO INTERRUPTS FOR EVENT DRIVEN
         2.5) BUTTONS PRESSED = ACTIONS
         2.75) debounce buttons with millis
  - DONE! 3) SETUP STATES FOR TOGGLE SWITCH - DONE!
         3.5)  SETUP POTENTIOMETER FOR TRANSMITTING VALUES
         4) WIFI setup over OSC
         5) JSON Config File
         6) Battery
         7) USB Charging
*/

byte POT_PIN = 36;
volatile bool toggle_state;
byte TOGGLE_PIN = 21;
const int BTN_LENGTH = 7;
byte BTN_PINS[] = {22, 17, 16, 39, 34, 4, 27 };

volatile byte BTNS_PRESSED[BTN_LENGTH];

void IRAM_ATTR handleInterrupt () {
  for (int i = 0; i < BTN_LENGTH; i++) {
    if (digitalRead(BTN_PINS[i])) {
      BTNS_PRESSED[i] = BTN_PINS[i];
    }
  }
}

void IRAM_ATTR toggleInterrupt() {

  //debounce with Millis
  int debounce_time = 200;
  static unsigned long prev_interrupt_time = 0;
  unsigned long curr_interrupt_time = millis();


  if (curr_interrupt_time - prev_interrupt_time > debounce_time) {
    toggle_state = !toggle_state;
  }

  //reset interrupt timer
  prev_interrupt_time = curr_interrupt_time;
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < BTN_LENGTH; i++) {
    pinMode(BTN_PINS[i], INPUT);
    attachInterrupt(digitalPinToInterrupt(BTN_PINS[i]), handleInterrupt, RISING);
    Serial.print("SETUP PIN #: ");
    Serial.println(BTN_PINS[i]);
  }
  attachInterrupt(digitalPinToInterrupt(TOGGLE_PIN), toggleInterrupt, CHANGE);
  Serial.print("BTN LENGTH: ");
  Serial.println(BTN_LENGTH);

  toggle_state = digitalRead(TOGGLE_PIN);
}

void loop() {

  for (int i = 0; i < BTN_LENGTH; i++) {
    if (digitalRead(BTN_PINS[i])) {
      Serial.print("BUTTON PRESSED: ");
      Serial.println(BTN_PINS[i]);
    }
  }
  if (toggle_state) {
    Serial.print(digitalRead(TOGGLE_PIN));
    Serial.println("   ------------------   ");
  }
  else {
    Serial.print("   ***************   ");
    Serial.print("POTENTIOMETER: ");
    Serial.println(analogRead(POT_PIN));
  }
}
