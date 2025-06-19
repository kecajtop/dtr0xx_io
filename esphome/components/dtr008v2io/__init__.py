import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import spi
from esphome.const import (
    CONF_ID,
    CONF_MODE,
    CONF_NUMBER,
    CONF_INVERTED,
    CONF_INPUT,
    CONF_OUTPUT,
)

DEPENDENCIES = ["spi"]
MULTI_CONF = True

dtr008v2io_ns = cg.esphome_ns.namespace("dtr008v2io")

dtr008v2ioComponent = dtr008v2io_ns.class_("dtr008v2ioComponent", cg.Component)
dtr008v2ioGPIOPin = dtr008v2io_ns.class_(
    "dtr008v2ioGPIOPin", cg.GPIOPin, cg.Parented.template(dtr008v2ioComponent)
)

CONF_dtr008v2io = "dtr008v2io"
CONF_OE_PIN = "oe_pin"

DTR008V2IO_PINS = 8

CONFIG_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ID): cv.declare_id(dtr008v2ioComponent),
        cv.Required(CONF_OE_PIN): pins.gpio_output_pin_schema,
    }
).extend(cv.COMPONENT_SCHEMA).extend(spi.spi_device_schema(cs_pin_required=True))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await spi.register_spi_device(var, config)
    oe_pin = await cg.gpio_pin_expression(config[CONF_OE_PIN])
    cg.add(var.set_oe_pin(oe_pin))

def validate_mode(value):
    if value[CONF_INPUT] == value[CONF_OUTPUT]:
        raise cv.Invalid("Mode must be either input or output")
    return value


dtr008v2io_PIN_SCHEMA = pins.gpio_base_schema(
    dtr008v2ioGPIOPin,
    cv.int_range(min=0, max=DTR008V2IO_PINS-1),
    modes=[CONF_INPUT, CONF_OUTPUT],
    mode_validator=validate_mode,
    invertable=True,
).extend(
    {
        cv.Required(CONF_dtr008v2io): cv.use_id(dtr008v2ioComponent),
    }
)

def dtr008v2io_pin_final_validate(pin_config, parent_config):
    if pin_config[CONF_NUMBER] >= DTR008V2IO_PINS:
        raise cv.Invalid(f"Pin number must be less than {DTR008V2IO_PINS}")


@pins.PIN_SCHEMA_REGISTRY.register(CONF_dtr008v2io, dtr008v2io_PIN_SCHEMA, dtr008v2io_pin_final_validate)
async def dtr008v2io_pin_to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_dtr008v2io])

    cg.add(var.set_pin(config[CONF_NUMBER]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    cg.add(var.set_flags(pins.gpio_flags_expr(config[CONF_MODE])))

    if config[CONF_MODE][CONF_INPUT]:
        parent = await cg.get_variable(config[CONF_dtr008v2io])
        cg.add(parent.set_use_inputs())

    return var
