#include "tmcm16xx.h"

TMCM16XX tmcm;  // define tmcm

const uint8_t rtsPin = 2;     // hardware flow control pin (RTS)
const uint8_t preDelay = 1;   // define the RTS delay in ms before sending data (should be 5 > predelay >= 0)
const uint8_t postDelay = 0;  // define the RTS delay in ms after sending data (should be 0)

const uint8_t* sendBuf; // define send array
const int32_t* recBuf; // define receive array
const uint8_t maxWaitReceive = 10; // max wait time for receive message in ms

// >> SETUP
void setup() {
  pinMode(rtsPin, OUTPUT);
  digitalWrite(rtsPin, LOW);

  Serial.begin(115200);   // start serial console
  Serial1.begin(115200);  // start serial RS485 115200
  delay(10);

  sendBuf = (const uint8_t*)malloc(9); // allocate memory for send array
  recBuf = (const int32_t*)malloc(5); // allocate memory for receive array

  TMCM16XX(); // init tmcm
  tmcm.setSerialReceiveId(1); // set serial receive device id (probably usb=0, rs485=1)

  sendBuf = tmcm.getAxisParameter(tmcm.POSITIONACTUAL); // define command for sendBuf
  setMessage(sendBuf); // send via serial
  recBuf = getMessage(); // receive via serial
  printMessage(recBuf); // print receive value
}

// >> LOOP
void loop() {

}

// >> SEND RS485 MESSAGE
void setMessage(const uint8_t* send){
  digitalWrite(rtsPin, HIGH);
  delay(preDelay);
  Serial1.write(send, 9);
  Serial1.flush();
  delay(postDelay);
  digitalWrite(rtsPin, LOW);
}

// >> RECEIVE RS485 MESSAGE
const int32_t* getMessage(){
  uint8_t tempBuf[9];
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
void printMessage(const int32_t* rec){
  Serial.print("val:");
  Serial.print(rec[4], DEC);
  Serial.print(", 0x");
  Serial.println(rec[4], HEX);
}
