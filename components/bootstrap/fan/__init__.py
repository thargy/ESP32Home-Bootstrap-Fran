import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan, output
from esphome.const import (
    CONF_OSCILLATION_OUTPUT,
    CONF_OUTPUT,
    CONF_DIRECTION_OUTPUT,
    CONF_OUTPUT_ID,
    CONF_SPEED,
    CONF_SPEED_COUNT,
    CONF_DURATION,
    CONF_INITIAL_VALUE,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
)
from .. import bootstrap_ns

CONF_USE_HIGHER = "use_higher"

BootstrapFan = bootstrap_ns.class_("BootstrapFan", cg.Component, fan.Fan)

def validate(config):
    if (CONF_INITIAL_VALUE in config and config[CONF_INITIAL_VALUE] > config[CONF_SPEED_COUNT]):
        raise cv.Invalid("The initial value cannot be greater than the speed count.")
    if (CONF_MAX_VALUE in config and config[CONF_MAX_VALUE] > config[CONF_SPEED_COUNT]):
        raise cv.Invalid("The maximum value cannot be greater than the speed count.")
    if (CONF_MIN_VALUE in config):
        if (config[CONF_MIN_VALUE] > config[CONF_SPEED_COUNT]):
            raise cv.Invalid("The minimum value cannot be greater than the speed count.")
        if (CONF_MAX_VALUE in config and config[CONF_MIN_VALUE] > config[CONF_MAX_VALUE]):
            raise cv.Invalid("The minimum value cannot be greater than the maximum value.")
    return config

CONFIG_SCHEMA = cv.All(fan.FAN_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(BootstrapFan),
        cv.Required(CONF_OUTPUT): cv.use_id(output.FloatOutput),
        cv.Optional(CONF_DURATION, default="1s"): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_INITIAL_VALUE): cv.int_range(min=1),
        cv.Optional(CONF_MIN_VALUE): cv.positive_int,
        cv.Optional(CONF_MAX_VALUE): cv.positive_int,
        cv.Optional(CONF_USE_HIGHER, default="true"): cv.boolean,
        cv.Optional(CONF_OSCILLATION_OUTPUT): cv.use_id(output.BinaryOutput),
        cv.Optional(CONF_DIRECTION_OUTPUT): cv.use_id(output.BinaryOutput),
        cv.Optional(CONF_SPEED): cv.invalid(
            "Configuring individual speeds is deprecated."
        ),
        cv.Optional(CONF_SPEED_COUNT, default=100): cv.int_range(min=1),
    }
).extend(cv.COMPONENT_SCHEMA), validate)

async def to_code(config):
    output_ = await cg.get_variable(config[CONF_OUTPUT])
    speed_count = config[CONF_SPEED_COUNT]
    initial_value = config[CONF_INITIAL_VALUE] if CONF_INITIAL_VALUE in config else speed_count
    minimum_value = config[CONF_MIN_VALUE] if CONF_MIN_VALUE in config else 0
    maximum_value = config[CONF_MAX_VALUE] if CONF_MAX_VALUE in config else speed_count
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID], output_, speed_count, int(config[CONF_DURATION].total_milliseconds), initial_value, minimum_value, maximum_value, bool(config[CONF_USE_HIGHER]))
    await cg.register_component(var, config)
    await fan.register_fan(var, config)

    if CONF_OSCILLATION_OUTPUT in config:
        oscillation_output = await cg.get_variable(config[CONF_OSCILLATION_OUTPUT])
        cg.add(var.set_oscillating(oscillation_output))

    if CONF_DIRECTION_OUTPUT in config:
        direction_output = await cg.get_variable(config[CONF_DIRECTION_OUTPUT])
        cg.add(var.set_direction(direction_output))