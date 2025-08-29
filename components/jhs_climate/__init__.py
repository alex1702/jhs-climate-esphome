import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]

jhs_climate_ns = cg.esphome_ns.namespace("JHS")
JHSClimate = jhs_climate_ns.class_("JHSClimate", cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(JHSClimate),
        cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
    }
).extend(cv.polling_component_schema("100ms"))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], await cg.get_variable(config["uart_id"]))
    await cg.register_component(var, config)
