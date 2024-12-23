/* button_control.cpp */
#include <Arduino.h>
#include "button_control.h"

#define DEBOUNCE_DELAY 50 // Debounce delay in milliseconds

// Button debounce state management structure
struct ButtonState {
  unsigned long lastDebounceTime;  // Timestamp for debounce handling
  bool lastStableState;           // Last stable state (HIGH/LOW)
  bool currentState;              // Current input state
  ButtonCallback callback;        // Callback function for button press
};

// Array to manage button states for up to 20 pins
static ButtonState buttonStates[20];

unsigned short ButtonInit(unsigned short pin, ButtonCallback callback) {
  pinMode(pin, INPUT_PULLUP); // Set button as input with pull-up mode
  buttonStates[pin].lastDebounceTime = 0;
  buttonStates[pin].lastStableState = HIGH; // Default state is HIGH (not pressed)
  buttonStates[pin].currentState = HIGH;
  buttonStates[pin].callback = callback; // Register callback function
  return 0;
}

unsigned short ButtonRead(void) {
  for (unsigned short pin = 0; pin < 20; pin++) {
    if (buttonStates[pin].callback == nullptr) {
      continue; // Skip pins without registered callbacks
    }

    // Read the current button state
    bool reading = digitalRead(pin);

    // Debounce handling
    if (reading != buttonStates[pin].currentState) {
      buttonStates[pin].lastDebounceTime = millis(); // Record timestamp
    }

    // Update stable state after debounce delay
    if ((millis() - buttonStates[pin].lastDebounceTime) > DEBOUNCE_DELAY) {
      if (reading != buttonStates[pin].lastStableState) {
        buttonStates[pin].lastStableState = reading; // Update stable state

        // Trigger callback on button press
        if (reading == LOW && buttonStates[pin].callback != nullptr) {
          buttonStates[pin].callback();
        }
      }
    }

    buttonStates[pin].currentState = reading; // Update current state
  }
  return 0;
}
