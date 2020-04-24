#include <Adafruit_GPS.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <gps.h>

Adafruit_GPS GPS(&GPSSerial);


void setup() {
  // wait for hardware serial to appear
  while (!Serial);

  
  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand(PGCMD_ANTENNA);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);//set with 10HZ update rate
  delay(1000);
}
//the preset max and min boundary
double latitude_min=34.0000; double latitude_max=34.0217;
double longitude_min=118.2828; double longitude_max=119.2899;

void loop(){
 if (Serial.available()) {
    char c = Serial.read();
    GPS.write(c);
  }
   if (GPSECHO)
    if (c) Serial.print(c);
  //set the boundary
  if (GPS.fix){
	double latitude=Serial.print(GPS.latitude, 4);  
	double longitude=Serial.print(GPS.longitude,4);
	double altitude=Serial.println(GPS.altitude);
	
	
  }
  if (latitude<latitude_min||latitude>latitude_max||longitude<longitude_min||longitude>longitude_max)
	  bool warning_gps=true;
  
  

  struct gps_data{double latitude; double longitude; double altitude; bool warning_gps};
}
