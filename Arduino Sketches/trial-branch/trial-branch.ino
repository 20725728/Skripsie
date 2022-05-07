struct GGA {
  int UTC;
  float lat;
  char n_s;
  float longi;
  char  e_w;
  float knots;
  float course;
  int date;
  float magVar;
  char mag_e_w;
  char mode;
};

GGA pos;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
}

void loop() {
  receiveGPSdata();
  delay(5000);
}

void clearRXdata(){
  for(int i =0;i<100;i++){
    rxData[i] = 0;
  }
}

void receiveGPSdata() {
  bool msgReceived = false;
  while (!msgReceived) {

    while (rxByte != 10) {
      rxByte = Serial1.read();
      if (rxByte != -1) {
        rxData[rxCnt] = rxByte;
        rxCnt ++;
      }
    }
    Serial.write(rxData,rxCnt);
    if (rxData[5] == 'C') {
      Serial.write(rxData,rxCnt);
      updateStruct(rxData,rxCnt);
    }else if (rxData[5] == 'G'){
      msgReceived = true;
    }
    rxCnt = 0;
    rxByte = 0;
  }
}

int charToInt(char data[], int startIndex, int len){
  int output = 0;
  int power = len - 1;
  for(int i = startIndex;i<len+startIndex;i++){
    output += (data[i] - 48) * pow(10,power);
    power--;
  }
  return output;
}

float charToDecimals(char data[], int startIndex, int len){
  float output = 0;
  int power = 1;
  for(int i = startIndex;i < len+startIndex; i++){
    output += (data[i] - 48) / pow(10,power);
    power++;
  }
  return output;
}

void updateStruct(char data[], int cnt) {
  //assume fixed length variables in the message
  pos.UTC = charToInt(data,7,6);
  pos.lat = charToInt(data,20,4) + charToDecimals(data,25,4);
  pos.north = data[30];
  pos.longi = charToInt(data,32,5) + charToDecimals(data,38,4);
//  pos.east = data[41];
//  pos.posFix = charInt
//  pos.satUsed = data[44] + 48;
//  pos.HDOP = (data[46] + 48) + (data[48] + 48) / 10 + (data[48] + 48) / 100;
//  pos.alt = (data[50] + 48) * 10 + (data[51] + 48) + (data[53] + 48) / 10;
//  pos.altUnit = data[55];

  Serial.println("Struct:");
  Serial.print(pos.UTC);
  Serial.print(",");
  Serial.print(pos.lat);
  Serial.print(",");
  Serial.print(pos.north);
  Serial.print(",");
  Serial.println(pos.longi);
//  Serial.print(",");
//  Serial.print(pos.east);

}
