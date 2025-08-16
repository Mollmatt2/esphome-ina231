import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ADDRESS,
    CONF_UPDATE_INTERVAL,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
    UNIT_AMPERE,
    UNIT_VOLT,
    UNIT_WATT,
)

DEPENDENCIES = ["i2c"]

ina231_ns = cg.esphome_ns.namespace("ina231")
INA231Component = ina231_ns.class_("INA231Component", cg.PollingComponent, i2c.I2CDevice)

CONF_SHUNT_RESISTANCE = "shunt_resistance"
CONF_MAX_CURRENT = "max_current"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(INA231Component),
            cv.Optional(CONF_ADDRESS, default=0x40): i2c.i2c_address,
            cv.Required(CONF_SHUNT_RESISTANCE): cv.resistance,
            cv.Required(CONF_MAX_CURRENT): cv.current,
            cv.Optional("bus_voltage"): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional("shunt_voltage"): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                accuracy_decimals=4,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional("current"): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                accuracy_decimals=3,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional("power"): sensor.sensor_schema(
                unit_of_measurement=UNIT_WATT,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x40))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_shunt_resistance(config[CONF_SHUNT_RESISTANCE]))
    cg.add(var.set_max_current(config[CONF_MAX_CURRENT]))

    if "bus_voltage" in config:
        sens = await sensor.new_sensor(config["bus_voltage"])
        cg.add(var.set_bus_voltage_sensor(sens))
    if "shunt_voltage" in config:
        sens = await sensor.new_sensor(config["shunt_voltage"])
        cg.add(var.set_shunt_voltage_sensor(sens))
    if "current" in config:
        sens = await sensor.new_sensor(config["current"])
        cg.add(var.set_current_sensor(sens))
    if "power" in config:
        sens = await sensor.new_sensor(config["power"])
        cg.add(var.set_power_sensor(sens))

    # Add the Zanduino/INA library (use the latest version or pin to a release)
    cg.add_library("github://Zanduino/INA.git", None)  # Or specify version like "2.3.0" if available
