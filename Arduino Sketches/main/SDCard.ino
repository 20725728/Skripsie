#include <SD.h>

// CS pin is pin 53,
const int chipSelect = 53; //Set CS interface

void initialize_SD(){
  Serial.print("Initializing SD card..."); //Serial port output data
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) { //If communication from the CS port fails with the SD card, the serial port outputs information: Card failed, or not present
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized."); //SD card communication success, serial output card information initialized.
  writeHeader();
}

void saveGPSPoints(){
  String output = "<Time>"+String(pos.date)+" "+String(pos.UTC)+"</Time><Lat>"+String(pos.lat)+"."+String(pos.latDecimal)+" "+String(pos.n_s)+"</Lat><Long>"+String(pos.longi)+"."+String(pos.longDecimal)+" "+String(pos.e_w)+"</Long><Speed>"+String(pos.knots)+"</Speed><Bearing>"+String(pos.course)+"</Beariing>\n";
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(output);
    Serial.println(output);
    dataFile.close();
    // dataString outputs to the serial port
  }
  // If the file cannot be opened, the serial port outputs an error message
  else {
    Serial.println("error opening datalog.txt");
  }
}

void writeHeader(){
  String header = "GPS data";
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if(dataFile){
    dataFile.println(header);
    dataFile.close();
  }else{
    Serial.println("Error opening datalog.txt for header");
  }
}
