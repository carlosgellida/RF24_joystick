#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <BasicLinearAlgebra.h>

using namespace BLA;

// instantiate an object for the nRF24L01 transceiver
RF24 radio(14, 12); // using pin 7 for the CE pin, and pin 8 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = {"1Node", "2Node"};
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// Used to control whether this node is sending or receiving
bool role = false;  // true = TX role, false = RX role

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
float payload = 0.0;

unsigned long timming ; 

bool sended, recieved; 

Matrix<4, 1> qDeseado = {1, 1, 1, 1}; 

Matrix<4, 1> qCurrent = {1, 1, 1, 1}; 

/*String arr2str(Matrix<4, 1> arr){
  String str ;
  str = String(arr(0)) + ',' + String(arr(1)) + ',' + String(arr(2)) + ',' + String(arr(3));
  return str; 
}

Matrix<4, 1> str2arr(String str){
  Matrix<4, 1> arr ; 
  int j=0; 
  for(int i=0; i < 4; i++){
    String str2 ; 
    while(true){
      if(str[j] != ',' && (j < 16)){
        //Serial.println(str[j]); 
        str2 = str2 + str[j] ; 
        j++ ; 
      } else {
        j++ ; 
        break ; 
      }
    } //while
    //Serial.println(str2); 
    arr(i) = str2.toFloat() ; 
  } //for
  return arr; 
}*/

void InitialiceRadio() {
      // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  radioNumber = 0; 

  radio.setPALevel(RF24_PA_MAX);  // RF24_PA_MAX is default.

  radio.setRetries(1, 5); 

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(qDeseado)); // float datatype occupies 4 bytes

  radio.setChannel(2475); 

  radio.setDataRate(RF24_2MBPS); 

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0

  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1

  // additional setup specific to the node's role
  if (role) {
    radio.stopListening();  // put radio in TX mode
  } else {
    radio.startListening(); // put radio in RX mode
  }
  timming = micros(); 
}

bool send(Matrix<4, 1> qDeseado){
  bool sended = false; 
  if (role) {
    //String qDeseadoString = arr2str(qDeseado); 
    // This device is a TX node
    payload += 0.01;   
    //unsigned long start_timer = micros();                    // start the timer
    //bool report = radio.write(&qDeseadoString, sizeof(qDeseadoString));      // transmit & save the report
    bool report = radio.write(&qDeseado, sizeof(qDeseado)); 

    if (report) {
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("current time = "));
      Serial.print(timming);                 // print the timer result
      Serial.print(F(" us. Sent: "));
      //Serial.println(qDeseadoString);                               // print payload sent
      Serial << qDeseado << '\n'; 
      role = !role  ;
      radio.startListening();                                    // increment float payload
      timming = micros(); 
      sended = true; 
    } else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
      role = !role; 
      InitialiceRadio();
      radio.startListening();
      //timming = micros();
      //Sí no puedes enviar comienza a escuchar
    }

  } 
  return sended; 
} 


bool recieve(Matrix<4, 1> &qCurrent){
  bool recieved = false; 
  if(!role) {
    //String qCurrenString ; 
    // This device is a RX node
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
    
      radio.read(&qCurrent, bytes);            // fetch payload from FIFO
      //qCurrent = str2arr(qCurrenString); 
      Serial.print(F("Received "));
      Serial.print(bytes);                    // print the size of the payload
      Serial.print(F(" Bytes on pipe "));
      Serial.print(pipe);                     // print the pipe number
      Serial.print(F(": "));
      //Serial.println(qCurrenString); 
      Serial << qCurrent << '\n'; 
      role = !role  ;
      recieved = true; 
      radio.stopListening();
    }
  } // role
  return recieved; 
}

void setup() {

  Serial.begin(115200);

  InitialiceRadio();
} // setup

void loop() {

  recieved = recieve(qCurrent); 

  sended = send(qDeseado) ; 

} // loop
