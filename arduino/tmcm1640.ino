#include "tmcm16xx.h"

TMCM16XX tmcm;  // define tmcm

const uint8_t ledPin = 13;

const uint8_t rtsPin = 2;     // hardware flow control pin (RTS)
const uint8_t preDelay = 1;   // define the RTS delay in ms before sending data (should be 5 > predelay >= 0)
const uint8_t postDelay = 0;  // define the RTS delay in ms after sending data (should be 0)

const unsigned char* sendMsg;       // define send array
const int* receiveMsg;              // define receive array
const uint8_t maxWaitReceive = 10;  // max wait time for receive message in ms

// >> SETUP
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(rtsPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(rtsPin, LOW);

  Serial.begin(115200);   // start serial console
  Serial1.begin(115200);  // start serial RS485 115200
  delay(1000);

  sendMsg = (const unsigned char*)malloc(9); // allocate memory for send array
  receiveMsg = (const int*)malloc(5);        // allocate memory for receive array

  TMCM16XX(); // init tmcm
  tmcm.setSerialReceiveId(1);  // set serial receive device id (probably usb=0, rs485=1)

  sendMsg = tmcm.setStop();
  setMessage(sendMsg);        // send message to serial
  receiveMsg = getMessage();  // receive and decode message
  printMessage(receiveMsg);   // print message
}

// >> LOOP
void loop() {
  digitalWrite(ledPin, HIGH);
  sendMsg = tmcm.setMoveRotateRight(300);
  setMessage(sendMsg);
  receiveMsg = getMessage();
  printMessage(receiveMsg);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(2000);

  digitalWrite(ledPin, HIGH);
  sendMsg = tmcm.setMoveRotateLeft(300);
  setMessage(sendMsg);
  receiveMsg = getMessage();
  printMessage(receiveMsg);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(2000);

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
  for (uint8_t i=0; i<maxWaitReceive; i++){   // wait loop
    if (Serial1.available() == 9){            // check for 9 bytes in buffer
      for (uint8_t j=0; j<9; j++){            // copy serial to buffer
        tempBuf[j] = Serial1.read();
      }
      return tmcm.decodeReceive(tempBuf);  // return and decode
    }
    else{
      delay(1);
    }
  }
  return tmcm.decodeReceive(tempBuf); // return empty buffer in case of error
}

// >> PRINT MESSAGE (for array definition look under "const int* TMCM16XX::decodeReceive()" function)
void printMessage(const int* rec){
  for (uint8_t i=0; i<5; i++){
    Serial.println(rec[i], HEX);
  }
  Serial.println();
}
