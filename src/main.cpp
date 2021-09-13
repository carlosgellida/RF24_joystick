#include <SPI.h>
#include "printf.h"
#include <RadioGamepad.h>

void setup() {

  Serial.begin(115200);

  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  initializeRadio(); 
} // setup

void loop() {
  Qd = "4, 4, 4, 4";

  bool sended = sendRF(); 

  bool recieved = recieveRF(); 
} // loop
