#include <BasicLinearAlgebra.h>
using namespace BLA;
#include <RadioGamepad.h>

Matrix<4, 1> qDeseado = {1, 1, 1, 1}; 

Matrix<4, 1> qCurrent = {1, 1, 1, 1}; 


void setup() {

  Serial.begin(115200);

  InitialiceRadio();
} // setup

void loop() {

  recieved = recieve(qCurrent); 

  sended = send(qDeseado) ; 

} // loop
