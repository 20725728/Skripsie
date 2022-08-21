  void saveGPSPoints(){
  if(validGPS == 'A'){
    String output = "<Time>"+String(pos.date)+" "+String(pos.UTC)+"</Time><Lat>"+String((long)(pos.latitudeDecimalDegrees/100000))+"."+String((long)abs((pos.latitudeDecimalDegrees%100000)))+"</Lat><Long>"+
    String((long)(pos.longitudeDecimalDegrees/100000))+"."+String((long)abs((pos.longitudeDecimalDegrees%100000)))+
    "</Long><Speed>"+String(pos.knots)+"</Speed><Bearing>"+String(pos.course)+"</Beariing>\n";
    File dataFile = SD.open(fileName, FILE_WRITE);
    if (dataFile) {
      dataFile.println(output);
      Serial.println(output);
      dataFile.close();
      // dataString outputs to the serial port
    }
    // If the file cannot be opened, the serial port outputs an error message
    else {
      Serial.println("error opening datalog");
    }
  }
}

void writeHeader(){
  String header = "GPS data "+String(pos.date)+"_"+String(pos.UTC)+".txt";
  Serial.print(fileName);
  File dataFile = SD.open(fileName, FILE_WRITE);
  dataFile.close();
  if(SD.exists(fileName)){
    Serial.write("File Does exists\n");
    dataFile = SD.open(fileName, FILE_WRITE);
  }else{
    Serial.write("File Does NOT exist\n");
  }
  if(dataFile){
    dataFile.println(header);
    dataFile.close();
  }else{
    Serial.println("Error opening datalog for header");
  }
}

void writeThrottle(){
  String throttleState = "<LEFT <DC "+String(leftSide.duty_Cycle)+" /DC><Input "+String(leftSide.analog)+" /Input><us "+String(RC*leftSide.duty_Cycle) + " /us>/LEFT>\t";
  throttleState += "<RIGHT <DC "+String(rightSide.duty_Cycle)+" /DC><Input "+String(rightSide.analog)+" /Input><us "+String(RC*rightSide.duty_Cycle) + " /us>/RIGHT>";
  File dataFile = SD.open("throttle.txt", FILE_WRITE);
  if(dataFile){
    dataFile.println(throttleState);
  }else{
    Serial.println("Error opening throttle file.");
  }
}
