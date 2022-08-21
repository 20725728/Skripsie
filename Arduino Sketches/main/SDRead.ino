

void readGPSPoints(){
  File gpsFile = SD.open("GPS_points.txt");
  char pointsBuf[540];
  if(gpsFile){
    int fileLength = gpsFile.available();
    char numberPoints = gpsFile.read();
    gpsFile.read(pointsBuf, fileLength-1);
  }
}

void decodeGPSBuf(char buf[], int numberPoints){
  int buffpos = 0;
  for(int i = 0;i<numberPoints;i++){
    buffpos = i*28;
    points[i].orderPos = buf[buffpos];
    buffpos+=2;
    points[i].latitude = charToInt(buf, buffpos, buffpos+4) *10000;
    buffpos+=5;
    points[i].latitude += charToInt(buf, buffpos, buffpos+4);
    buffpos+=5;
    if(buf[buffpos] == 'S'){
      points[i].latitude *= -1;
    }
    buffpos+=2;
    points[i].longitude =  charToInt(buf, buffpos, buffpos+5) *10000;
    buffpos+=6;
    points[i].longitude +=  charToInt(buf, buffpos, buffpos+4);
    buffpos+=5;
    if( buf[buffpos] == 'W'){
      points[i].longitude *= -1;
    }
  }
}
