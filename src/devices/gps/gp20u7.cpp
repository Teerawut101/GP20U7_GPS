#include "gp20u7.h"
// TODO: Handle uninitialized _currentLocation

GP20U7::GP20U7(PlatformStream &s){
  _stream = &s;
}

GP20U7::GP20U7(PlatformStream *s){
  _stream = s;
}

Geolocation GP20U7::getGeolocation(){
    return _currentLocation;
}

int GP20U7::read(){
  long lat, lon;
  unsigned long fix_age;

  while(_stream->available()){
    int c = _stream->read();
    if(_gps.encode(c)){
      _gps.get_position(&lat, &lon, &fix_age);

      // Map tinygps values into Geolocation struct
      _currentLocation.latitude = lat / 1000000.0;
      _currentLocation.longitude = lon / 1000000.0;
      _latitude=lat / 1000000.0;
      _longitude=lon / 1000000.0;
      return 1;
    }
  }

  return 0;
}
//float TinyGPS::distance_between (float lat1, float long1, float lat2, float long2) 
float GP20U7::distance(float lat2, float long2)
{ 
 float lat1=_latitude;
 float long1=_longitude;
  // returns distance in meters between two positions, both specified 
  // as signed decimal-degrees latitude and longitude. Uses great-circle 
  // distance computation for hypothetical sphere of radius 6372795 meters.
  // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
  // Courtesy of Maarten Lamers
  float delta = radians(long1-long2);
  float sdlong = sin(delta);
  float cdlong = cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong); 
  delta = sq(delta); 
  delta += sq(clat2 * sdlong); 
  delta = sqrt(delta); 
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong); 
  delta = atan2(delta, denom); 
  return delta * 6372795; 

}
void GP20U7::begin(){
  #if defined (ARDUINO_PLATFORM)
    reinterpret_cast<HardwareSerial *>(_stream)->begin(9600);
  #endif
}
void GP20U7::begin(long b){
  #if defined (ARDUINO_PLATFORM)
    reinterpret_cast<HardwareSerial *>(_stream)->begin(b);
  #endif
}