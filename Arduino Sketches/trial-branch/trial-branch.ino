struct GGA {
  int UTC;
  float lat;
  char n_s;
  float longi;
  char  e_w;
  float knots;
  float course;
  int date;
};

GGA pos;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
}

void loop() {
  int rx = Serial1.read();
  if(rx!=-1){
    Serial.write(rx);
  }
//  receiveGPSdata();
//  printStruct();
//  delay(5000);
}

void receiveGPSdata() {
  bool msgReceived = false;
  char rxData[100];
  int rxCnt = 0;
  int rxByte = 0;
  
  while (!msgReceived) {

    while (rxByte != 10) {
      rxByte = Serial1.read();
      if (rxByte != -1) {
        rxData[rxCnt] = rxByte;
        rxCnt ++;
      }
    }
    Serial.write(rxData,rxCnt);
//    if (rxData[5] == 'C') {
//      updateStructRMC(rxData,rxCnt);
//      msgReceived = true;
//    }
//    rxCnt = 0;
//    rxByte = 0;
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

void printStruct(){
//  String toString = "Struct:\n\tTime (UTC): "+pos.UTC+"\n\tLatitude: "+pos.lat+" "+pos.n_s+"\n\tLongitude: "+pos.longi+" "+pos.e_w+
//  "\n\tSpeed: "+pos.knots+" knots on bearing: "+pos.course+" degrees\n\tMagnetic Variation: "+pos.magVar+" "+pos.mag_e_w+"\n\tMode: "+pos.mode;
  Serial.println("Struct:");
  Serial.print("\tTime (UTC): ");
  Serial.print(pos.UTC);
  Serial.print("\n\tDate: ");
  Serial.print(pos.date);
  Serial.print("\n\tLatitude: ");
  Serial.print(pos.lat);
  Serial.print(" ");
  Serial.print(pos.n_s);
  Serial.print("\n\tLongitude: ");
  Serial.print(pos.longi);
  Serial.print(" ");
  Serial.print(pos.e_w);
  Serial.print("\n\tSpeed: ");
  Serial.print(pos.knots);
  Serial.print(" knots on bearing: ");
  Serial.print(pos.course);
  Serial.println(" degrees");

}

void updateStructRMC(char data[], int cnt) {
  //assume fixed length variables in the message
  pos.UTC = charToInt(data,7,6);
  pos.lat = charToInt(data,20,4) + charToDecimals(data,25,4);
  pos.n_s = data[30];
  pos.longi = charToInt(data,32,5) + charToDecimals(data,38,4);
  pos.e_w = data[43];
  pos.knots = charToInt(data,45,1) + charToDecimals(data,47,2);
  pos.course = charToInt(data,50,3) + charToDecimals(data,54,2);
  pos.date = charToInt(data,57,6);

}
