void navigate(){
  double distance;
  double bearing_error;
  double target_bearing;
  double maxBearing;
  double minBearing;

  GPSpt target = points[targetIndex];
  distance = GetDistance(pos.latitudeDecimalDegrees, pos.longitudeDecimalDegrees, target.latitudeDecimalDegrees, target.longitudeDecimalDegrees);
  target_bearing = GetBearing(pos.latitudeDecimalDegrees, pos.longitudeDecimalDegrees, target.latitudeDecimalDegrees, target.longitudeDecimalDegrees);
  bearing_error = target_bearing - pos.course;
  if(distance < throttle_down_distance){
    throttle_percentage = distance/(2*throttle_down_distance);
    if(distance < arrival_threshold){
      NextPoint();
    }
  }else{
    throttle_percentage = 100;
  }
  if(abs(bearing_error) < full_steer_angle){
    steering_percentage = abs(bearing_error)/full_steer_angle;
  }else{
    steering_percentage = 100;
  }
  maxBearing = max(pos.course, target_bearing);
  minBearing = min(pos.course, target_bearing);
  if((maxBearing == target_bearing) ^ ((maxBearing-minBearing)>180)){
    steer_right();
  }else{
    steer_left();
  }
  power_thrusters();
  if(Debug == 1){
    writeError(distance, bearing_error, target_bearing);  
  }else{
    writeNavigation(distance, bearing_error, target_bearing); 
  }
}



void steer_left(){
  int throttle_difference = 2*throttle_percentage;
  rightSide.duty_Cycle = 75 + (25*(throttle_percentage/100));
  leftSide.duty_Cycle = rightSide.duty_Cycle - (25 * ((throttle_difference * (steering_percentage/100))/100));
}

void steer_right(){
  int throttle_difference = 2*throttle_percentage;
  leftSide.duty_Cycle = 75 + (25*(throttle_percentage/100));
  rightSide.duty_Cycle = leftSide.duty_Cycle - (25* ((throttle_difference * (steering_percentage/100))/100));
}
