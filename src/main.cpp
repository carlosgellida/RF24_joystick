#include <BasicLinearAlgebra.h>
using namespace BLA;
#include <RadioGamepad2.h>

Matrix<4, 1> qDeseado = {3, 3, 3, 3}; 

Matrix<4, 1> qCurrent = {3, 3, 3, 3}; 


void setup() {

  Serial.begin(115200);

  InitialiceRadio();
} // setup

void loop() {

  recieved = recieve(qCurrent); 

  sended = send(qDeseado) ; 

} // loop
