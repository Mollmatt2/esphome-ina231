#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"
#include <INA.h>  // From Zanduino/INA library

namespace esphome {
namespace ina231 {

class INA231Component : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_bus_voltage_sensor(sensor::Sensor *bus_voltage_sensor) { bus_voltage_sensor_ = bus_voltage_sensor; }
  void set_shunt_voltage_sensor(sensor::Sensor *shunt_voltage_sensor) { shunt_voltage_sensor_ = shunt_voltage_sensor; }
  void set_current_sensor(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
  void set_power_sensor(sensor::Sensor *power_sensor) { power_sensor_ = power_sensor; }
  void set_shunt_resistance(float shunt_resistance) { shunt_resistance_ = shunt_resistance; }
  void set_max_current(float max_current) { max_current_ = max_current; }

  void setup() override;
  void dump_config() override;
  void update() override;

 protected:
  sensor::Sensor *bus_voltage_sensor_{nullptr};
  sensor::Sensor *shunt_voltage_sensor_{nullptr};
  sensor::Sensor *current_sensor_{nullptr};
  sensor::Sensor *power_sensor_{nullptr};
  float shunt_resistance_;
  float max_current_;
  INA_Class INA;  // Instance from the library
};

}  // namespace ina231
}  // namespace esphome
