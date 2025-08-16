#include "ina231.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ina231 {

static const char *const TAG = "ina231";

void INA231Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up INA231...");
  uint8_t devices = this->INA.begin(this->max_current_, this->shunt_resistance_, 0, INA231);  // Device 0 on I2C bus, specify INA231 type
  if (devices == 0) {
    ESP_LOGE(TAG, "INA231 not found!");
    this->mark_failed();
    return;
  }
  this->INA.setBusConversion(3320);    // Adjust as needed (from library docs)
  this->INA.setShuntConversion(3320);  // Adjust as needed
  this->INA.setAveraging(16);          // Optional averaging
  this->INA.setMode(INA_MODE_SHUNT_AND_BUS_CONTINUOUS);  // Continuous mode
}

void INA231Component::dump_config() {
  ESP_LOGCONFIG(TAG, "INA231:");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Bus Voltage", this->bus_voltage_sensor_);
  LOG_SENSOR("  ", "Shunt Voltage", this->shunt_voltage_sensor_);
  LOG_SENSOR("  ", "Current", this->current_sensor_);
  LOG_SENSOR("  ", "Power", this->power_sensor_);
}

void INA231Component::update() {
  float bus_mv = this->INA.getBusMilliVolts(0) / 1000.0f;  // Device 0
  float shunt_uv = this->INA.getShuntMicroVolts(0) / 1000000.0f;
  float current_ma = this->INA.getBusMicroAmps(0) / 1000000.0f;  // Library provides current in microAmps
  float power_mw = this->INA.getBusMicroWatts(0) / 1000000.0f;

  if (this->bus_voltage_sensor_ != nullptr) this->bus_voltage_sensor_->publish_state(bus_mv);
  if (this->shunt_voltage_sensor_ != nullptr) this->shunt_voltage_sensor_->publish_state(shunt_uv);
  if (this->current_sensor_ != nullptr) this->current_sensor_->publish_state(current_ma);
  if (this->power_sensor_ != nullptr) this->power_sensor_->publish_state(power_mw);
}

}  // namespace ina231
}  // namespace esphome
