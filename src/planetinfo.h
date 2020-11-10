#pragma once

#include <string>

class planet_info {
public:
  planet_info() = default;
  virtual ~planet_info() = default;

  void set_ut(double ut);
  double get_ut();

  void set_planetid(int id);
  inline double get_longitude() { return data.longitude; }

  const std::string get_planet_name();

  void calc();

private:
  typedef struct {
    double longitude;
    double latitude;
    double distance;
    double spd_longitude;
    double spd_latitude;
    double spd_distance;
  } planet_eph_data;

  double ut;
  int id;
  planet_eph_data data;
};
