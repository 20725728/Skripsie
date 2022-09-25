  void saveGPSPoints(){
  if(validGPS == 'A'){
    String output = "Time#"+String(pos.date)+" "+String(pos.UTC)+"#DD#Lat>"+String(pos.latitudeDecimalDegrees,8)+"#Long#"+
    String(pos.longitudeDecimalDegrees,8)+
    "#DMS#Lat#"+String((long)pos.latitude)+"#Long#"+String((long)pos.longitude)+"#Speed#"+String(pos.knots)+"#Bearing#"+String(pos.course);
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
  }else{
    Serial.println("Error opening datalog for header");
  }
  dataFile.close();
  //Throttle Header
  header = "Throttle_data "+String(pos.date)+"_"+String(pos.UTC)+".txt";
  fileName = "throttle.txt";
  Serial.print(fileName);
  dataFile = SD.open(fileName, FILE_WRITE);
  dataFile.close();
 if(SD.exists(fileName)){
    Serial.write("File Does exists\n");
    dataFile = SD.open(fileName, FILE_WRITE);
  }else{
    Serial.write("File Does NOT exist\n");
  }
  if(dataFile){
    dataFile.println(header);
  }else{
    Serial.println("Error opening datalog for throttle header");
  }
  dataFile.close();
  //Nav Header
  header = "Nav_data "+String(pos.date)+"_"+String(pos.UTC)+".txt";
  fileName ="nav.txt";

  Serial.print(fileName);
  dataFile = SD.open(fileName, FILE_WRITE);
  dataFile.close();
  if(SD.exists(fileName)){
    Serial.write("File Does exists\n");
    dataFile = SD.open(fileName, FILE_WRITE);
  }else{
    Serial.write("File Does NOT exist\n");
  }
  if(dataFile){
    dataFile.println(header);
  }else{
    Serial.println("Error opening datalog for nav header");
  }
  dataFile.close();
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
  dataFile.close();
}

void writeNavigation(double dist, double bearing_error, double target_bearing){
  String navigationErrors ="Target\nLat#"+String(points[targetIndex].latitudeDecimalDegrees,8)+"#Long#"+String(points[targetIndex].longitudeDecimalDegrees,8)+"#Bearing#"+String(target_bearing,6)+
    "\nCurrent:\nLat#"+String(pos.latitudeDecimalDegrees,8)+"#Long#"+String(pos.longitudeDecimalDegrees,8)+"#Bearing#"+String(pos.course,6)+
    "\nErrors:\nDist#"+String(dist,6)+"#Bearing_Error#"+String(bearing_error,6)+
    "\nMotors:\nThrottle%#"+String(throttle_percentage,5)+"#Steering%#"+String(steering_percentage,5)+"#Left_DC#"+String(leftSide.duty_Cycle,5)+"#Right_DC#"+String(rightSide.duty_Cycle,5)+"\n\n\n";
    File dataFile = SD.open("nav.txt",FILE_WRITE);
    if(dataFile){
      dataFile.println(navigationErrors);

    }else{
      Serial.println("Error opening nav error file");
    }
    dataFile.close();
}