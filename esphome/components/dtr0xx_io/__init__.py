import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import (
    CONF_ID,
    CONF_MODE,
    CONF_NUMBER,
    CONF_INVERTED,
    CONF_INPUT,
    CONF_OUTPUT,
)

CODEOWNERS = ["@kecaj"]
DEPENDENCIES = []
MULTI_CONF = True

dtr0xx_io_ns = cg.esphome_ns.namespace("dtr0xx_io")

dtr0xx_ioComponent = dtr0xx_io_ns.class_("dtr0xx_ioComponent", cg.PollingComponent)
dtr0xx_ioGPIOPin = dtr0xx_io_ns.class_(
    "dtr0xx_ioGPIOPin", cg.GPIOPin, cg.Parented.template(dtr0xx_ioComponent)
)

CONF_dtr0xx_io = "dtr0xx_io"
CONF_DINGTIAN_PL = "dingtian_pl_pin"
CONF_DINGTIAN_RCK = "dingtian_rck_pin"
CONF_SR_COUNT = "sr_count"
CONF_DINGTIAN_Q7 = "dingtian_q7_pin"
CONF_DINGTIAN_SDI = "dingtian_sdi_pin"
CONF_DINGTIAN_CLK = "dingtian_clk_pin"
# Whether this instance reads inputs (SN74HC165 side) at all. Boards that
# only use outputs can set this to false to avoid the relay buzz caused by
# needlessly toggling PL / polling on a timer - see
# https://github.com/kecajtop/dtr0xx_io/issues/22
CONF_USE_INPUT = "use_input"


def validate_dtr0xx_io_config(config):
    if config[CONF_USE_INPUT]:
        if CONF_DINGTIAN_Q7 not in config:
            raise cv.Invalid(
                f"'{CONF_DINGTIAN_Q7}' is required when '{CONF_USE_INPUT}' is true"
            )
        if CONF_DINGTIAN_PL not in config:
            raise cv.Invalid(
                f"'{CONF_DINGTIAN_PL}' is required when '{CONF_USE_INPUT}' is true"
            )
    return config


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.declare_id(dtr0xx_ioComponent),
            cv.Optional(CONF_DINGTIAN_Q7): pins.gpio_input_pin_schema,
            cv.Required(CONF_DINGTIAN_SDI): pins.gpio_output_pin_schema,
            cv.Required(CONF_DINGTIAN_CLK): pins.gpio_output_pin_schema,
            cv.Optional(CONF_DINGTIAN_PL): pins.gpio_output_pin_schema,
            cv.Optional(CONF_DINGTIAN_RCK): pins.gpio_output_pin_schema,
            cv.Optional(CONF_SR_COUNT, default=1): cv.int_range(min=1, max=256),
            cv.Optional(CONF_USE_INPUT, default=True): cv.boolean,
        }
        # update_interval is provided by this - only matters while use_input
        # is true, since update() is a no-op otherwise.
    ).extend(cv.polling_component_schema("100ms")),
    validate_dtr0xx_io_config,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    dingtian_sdi_pin = await cg.gpio_pin_expression(config[CONF_DINGTIAN_SDI])
    cg.add(var.set_dingtian_sdi_pin(dingtian_sdi_pin))
    dingtian_clk_pin = await cg.gpio_pin_expression(config[CONF_DINGTIAN_CLK])
    cg.add(var.set_dingtian_clk_pin(dingtian_clk_pin))

    cg.add(var.set_use_input(config[CONF_USE_INPUT]))

    if CONF_DINGTIAN_Q7 in config:
        dingtian_q7_pin = await cg.gpio_pin_expression(config[CONF_DINGTIAN_Q7])
        cg.add(var.set_dingtian_q7_pin(dingtian_q7_pin))
    if CONF_DINGTIAN_PL in config:
        dingtian_pl_pin = await cg.gpio_pin_expression(config[CONF_DINGTIAN_PL])
        cg.add(var.set_dingtian_pl_pin(dingtian_pl_pin))
    if CONF_DINGTIAN_RCK in config:
        dingtian_rck_pin = await cg.gpio_pin_expression(config[CONF_DINGTIAN_RCK])
        cg.add(var.set_dingtian_rck_pin(dingtian_rck_pin))

    cg.add(var.set_sr_count(config[CONF_SR_COUNT]))


def validate_mode(value):
    if not (value[CONF_INPUT] or value[CONF_OUTPUT]):
        raise cv.Invalid("Mode must be either input or output")
    if value[CONF_INPUT] and value[CONF_OUTPUT]:
        raise cv.Invalid("Mode must be either input or output")
    return value


dtr0xx_io_PIN_SCHEMA = pins.gpio_base_schema(
    dtr0xx_ioGPIOPin,
    cv.int_range(min=0, max=2047),
    modes=[CONF_INPUT, CONF_OUTPUT],
    mode_validator=validate_mode,
).extend(
    {
        cv.Required(CONF_dtr0xx_io): cv.use_id(dtr0xx_ioComponent),
    }
)


def dtr0xx_io_pin_final_validate(pin_config, parent_config):
    max_pins = parent_config[CONF_SR_COUNT] * 8
    if pin_config[CONF_NUMBER] >= max_pins:
        raise cv.Invalid(f"Pin number must be less than {max_pins}")
    if pin_config[CONF_MODE][CONF_INPUT] and not parent_config[CONF_USE_INPUT]:
        raise cv.Invalid(
            f"Pin {pin_config[CONF_NUMBER]} is configured as an input, but "
            f"'{CONF_USE_INPUT}' is false on the parent dtr0xx_io instance"
        )


@pins.PIN_SCHEMA_REGISTRY.register(
    CONF_dtr0xx_io, dtr0xx_io_PIN_SCHEMA, dtr0xx_io_pin_final_validate
)
async def dtr0xx_io_pin_to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_dtr0xx_io])

    cg.add(var.set_pin(config[CONF_NUMBER]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    cg.add(var.set_flags(pins.gpio_flags_expr(config[CONF_MODE])))
    return var
