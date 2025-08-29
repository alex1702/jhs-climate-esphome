import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = []

jhs_climate_ns = cg.esphome_ns.namespace("JHS")
JHSClimate = jhs_climate_ns.class_("JHSClimate")

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(JHSClimate),
        cv.Required("panel_tx_pin"): cv.int_,
        cv.Required("panel_rx_pin"): cv.int_,
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config["panel_tx_pin"], config["panel_rx_pin"])
    await cg.register_component(var, config)
