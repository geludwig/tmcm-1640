#include "tmcm16xx.h"

TMCM16XX tmcm;  // define tmcm

const uint8_t rtsPin = 2; // hardware flow control pin (RTS)
const char preDelay = 1;  // define the RTS delay in ms before sending data (should be > 0)
const char postDelay = 0; // define the RTS delay in ms after sending data (should be 0)

const unsigned char* sendMsg;   // define send array
const int* receiveMsg;          // define receive array
const char maxWaitReceive = 20; // max wait time for receive message in ms

// >> SETUP
void setup() {
  pinMode(rtsPin, OUTPUT);  // define rtsPin as output

  Serial.begin(9600);   // start serial console
  Serial1.begin(9600);  // start serial RS485
  delay(200);

  sendMsg = (const unsigned char*)malloc(9); // allocate memory for send array
  receiveMsg = (const int*)malloc(5);        // allocate memory for receive array

  TMCM16XX(); // init tmcm
  tmcm.setSerialReceiveId(1);  // set serial receive device id (probably usb=0, rs485=1)

  sendMsg = tmcm.setMoveRotateRight(100); // init sendMsg with command (here rotate right with speed 100)
  setMessage(sendMsg);                    // send message to serial
  receiveMsg = getMessage();              // receive and decode message
  printMessage(receiveMsg);               // print message
}

// >> LOOP
void loop() {

}

// >> SEND RS485 MESSAGE
void setMessage(const unsigned char* send){
  digitalWrite(rtsPin, HIGH);
  delay(preDelay);
  Serial1.write(send, 9);
  Serial1.flush();
  delay(postDelay);
  digitalWrite(rtsPin, LOW);
}

// >> RECEIVE RS485 MESSAGE
const int* getMessage(){
  char tempBuf[9];

  for (char i=0; i<maxWaitReceive; i++){  // wait loop
    if (Serial1.available() == 9){        // check for 9 bytes in buffer
      for (char j=0; j<9; j++){           // copy serial to buffer
        tempBuf[j] = Serial1.read();
      }
      return tmcm.decodeReceive(tempBuf); // return and decode
    }
    else{
      delay(1);
    }
  }
  return tmcm.decodeReceive(tempBuf);     // return empty buffer in case of error
}

// >> PRINT MESSAGE (for array definition look under "const int* TMCM16XX::decodeReceive()" function)
void printMessage(const int* rec){
  for (char i=0; i<5; i++){
    Serial.print(rec[i]);
    Serial.print(" ");
  }
  Serial.println();
}
