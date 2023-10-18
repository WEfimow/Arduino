
int dataPin = A3;
int clockPin = A2;
int strobePin = 24;

byte arr[8];

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(strobePin, OUTPUT);

  digitalWrite(strobePin, HIGH);
  digitalWrite(clockPin, HIGH);

  sendCommand(0x40);
  sendCommand(B10000001);

// clear buffer (obcas nefunguje :))
  digitalWrite(strobePin, LOW);
  send(0x00);
  for (int i = 0; i < 16; i++) {
    send(0x00);
  }
  digitalWrite(strobePin, HIGH);   

  putLine(0, B11111111);
  putLine(1, B10000001);
  putLine(2, B10111101);
  putLine(3, B10100101);
  putLine(4, B10100101);
  putLine(5, B10111101);
  putLine(6, B10000001);
  putLine(7, B11111111);
  draw();
  
}

void putPix(byte x, byte y, byte on) {
  bitWrite(arr[y], x, on);
}

void putLine(byte y, byte data) {
  arr[y] = data;
}

void inverse() {
   for (int i=0; i<8; i++) {
     byte val = arr[i];
     arr[i] = val ^ B11111111;
   }
}

void draw() {
  for (int i=0; i<8; i++) {
    int pos=B11000000;
    int data;
    int data2;
    for (int a=0; a<4; a++) {
      bitWrite(data, a, bitRead(arr[a], i));
    }
    for (int a=4; a<8; a++) {
      bitWrite(data2, a-4, bitRead(arr[a], i));
    }
    sendData((pos+i*2), data);
    sendData((pos+i*2+1), data2);
  }
  sendCommand(0x40);
  sendCommand(B10001000);
  //               ^ display on bit
  // display on, min brightness
}


void loop() {
//sendCommand(0x40);
// control command
//sendCommand(B10001000);
  //max brightness B10001111);
  //                     ^^^ brightness 

  delay(500);
  inverse();
  draw();
  //sendCommand(0x40);
  //sendCommand(B10000000);
  //                 ^ display off

  delay(500);
}





void sendCommand(byte cmd)
{
  digitalWrite(strobePin, LOW);
  send(cmd);
  digitalWrite(strobePin, HIGH);
} 

void send(byte data)
{
  for (int i = 0; i < 8; i++) {
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, data & 1 ? HIGH : LOW);
    data >>= 1;
    digitalWrite(clockPin, HIGH);
  }
} 


void sendData(byte address, byte data)
{
  sendCommand(0x42);
  digitalWrite(strobePin, LOW);
  send(0x00 | address);
  send(data);
  digitalWrite(strobePin, HIGH);
}
