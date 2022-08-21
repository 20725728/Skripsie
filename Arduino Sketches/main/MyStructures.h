struct GPSpt{
  int orderPos;
  long latitude;
  long latitudeDecimalDegrees;
  long longitude;
  long longitudeDecimalDegrees;
};

struct GPSrx {
  int date;
  int UTC;
  long latitude;
  long latitudeDecimalDegrees;
  long longitude;
  long longitudeDecimalDegrees;
  float knots;
  float course;
};

struct PWMControl {
  int analog = 505;             //the middle for both of the POTs
  float duty_Cycle = 0.75;      //75% is neutral.
};
