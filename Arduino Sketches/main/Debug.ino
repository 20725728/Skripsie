void writeError(double dist, float bearing_error, float target_bearing){
  String navigationErrors ="Target\nLat#"+String(points[targetIndex].latitudeDecimalDegrees,8)+"#Long#"+String(points[targetIndex].longitudeDecimalDegrees,8)+"#Bearing#"+String(target_bearing,6)+
    "\nCurrent:\nLat#"+String(pos.latitudeDecimalDegrees,8)+"#Long#"+String(pos.longitudeDecimalDegrees,8)+"#Bearing#"+String(pos.course,6)+
    "\nErrors:\nDist#"+String(dist,6)+"#Bearing_Error#"+String(bearing_error,6)+
    "\nMotors:\nThrottle%#"+String(throttle_percentage)+"#Steering%#"+String(steering_percentage)+"#Left_DC#"+String(leftSide.duty_Cycle,5)+"#Right_DC#"+String(rightSide.duty_Cycle,5)+"\n\n\n";
  Serial.print(navigationErrors);
}

void receiveGPSPoint(){
  int rx;
  points[addPointsCnt].latitudeDecimalDegrees = 0;
  points[addPointsCnt].longitudeDecimalDegrees = 0;
  for(int i =0;i<7;i++){
    do{
      rx = Serial.read();
    }while(rx==-1);
    points[addPointsCnt].latitudeDecimalDegrees += (rx-48)*(pow(10,(1-i)));
  }
  points[addPointsCnt].latitudeDecimalDegrees *= -1;
  do{
    rx = Serial.read();
  }while(rx==-1);
  for(int i =0;i<7;i++){
    do{
      rx = Serial.read();
    }while(rx==-1);
    points[addPointsCnt].longitudeDecimalDegrees += (rx-48)*(pow(10,(1-i)));
  }
  Serial.println("\tLAT: "+String(points[addPointsCnt].latitudeDecimalDegrees,8)+"\tLONG: "+String(points[addPointsCnt].longitudeDecimalDegrees,8));
}

void receiveAllGPSPoints(){
  int rx;
  Serial.print("How many points are to be entered? :\n");
  do{
    rx = Serial.read();
  }while(rx==-1);
  int numPoints = rx - 48;
  for(int i = 0;i<numPoints;i++){
    Serial.print("\nEnter point "+String(i)+":\n");
    receiveGPSPoint();
  }
}
