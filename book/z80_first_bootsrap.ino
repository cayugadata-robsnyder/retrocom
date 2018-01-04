#include <TimerOne.h>

/***********************************************************
 * Z80 First Bootstrap
 * (Expirement One from "Digital Electronics for the Retro Computing Enthusiast"
 * 
 * Copyright (c) 2018 Rob Snyder / Cayuga Data Technologies LLC, Ithaca, NY
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *  
 *    http://www.apache.org/licenses/LICENSE-2.0
 *    
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * This program performs the basic functions necessary to get a Z80 CPU 
 * to run; specifically, it generates the initial reset and the ongoing
 * clock pulses the Z80 requires.
 */

// Pin hooked to Z80 PIN 26
const int RESET_PIN = 3;

// Pin hooked to Z80 PIN 6 (CLK)
const int CLOCK_PIN = 2;

// Used to keep track of the state of the clock (on/off)
volatile bool CLOCK_OUT = 0;

// Our clock frequency, measured in half cycles
const long CLOCK_INTERVAL =  25000; // (0.025 seconds, 40 half-cycles, effectively 20hz)

void setup() {
  
  // Set up the timer and reset pins; turn both off, which for RESET is actually high 
  // (RESET is active low)
  
  pinMode(RESET_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  digitalWrite(CLOCK_PIN, LOW);
  
  // start with interrupts off, then schedule the clock interrupt
  noInterrupts();
  Timer1.initialize(CLOCK_INTERVAL);
  Timer1.attachInterrupt(toggle_clock);
  
}

void toggle_clock() {
  // This is the interrupt routine that toggles the clock pulse on and off
  if (CLOCK_OUT == 0) {
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    CLOCK_OUT = 1;
  } else {
    digitalWrite(CLOCK_PIN, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    CLOCK_OUT = 0;
  }
}

void loop() {

  // wait for a moment for things to settle
  delay(250);
  
  // activate reset
  digitalWrite(RESET_PIN, LOW);

  // start the clock
  interrupts();

  // Delay for reset
  delay(100);

  // Turn off reset and let the Z80 run
  digitalWrite(RESET_PIN, HIGH);

  while (true) { }

}
