import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, binary_sensor
from esphome.const import CONF_ID
from esphome import pins

MULTI_CONF = True

AUTO_LOAD = [ "binary_sensor", "climate"]

DEPENDENCIES = []

jhs_climate_ns = cg.esphome_ns.namespace("JHS")
# JHSClimate = jhs_climate_ns.class_("JHSClimate", cg.Component)
JHSClimate = jhs_climate_ns.class_("JHSClimate", climate.Climate, cg.Component)

CONF_AC_TX_PIN = 'ac_tx_pin'
CONF_AC_RX_PIN = 'ac_rx_pin'
CONF_PANEL_TX_PIN = 'panel_tx_pin'
CONF_PANEL_RX_PIN = 'panel_rx_pin'
CONF_WATER_FULL_SENSOR = 'water_full_sensor'

# CONFIG_SCHEMA = cv.Schema(
#     {
#         cv.GenerateID(): cv.declare_id(JHSClimate),
#         cv.Required(CONF_AC_TX_PIN): pins.gpio_output_pin_schema,
#         cv.Required(CONF_AC_RX_PIN): pins.gpio_input_pin_schema,
#         cv.Required(CONF_PANEL_TX_PIN): pins.gpio_output_pin_schema,
#         cv.Required(CONF_PANEL_RX_PIN): pins.gpio_input_pin_schema,
#     }
# )

CONFIG_SCHEMA = cv.All(
    climate.climate_schema(JHSClimate).extend(
        {
            cv.GenerateID(): cv.declare_id(JHSClimate),
            cv.Required(CONF_AC_TX_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_AC_RX_PIN): pins.gpio_input_pin_schema,
            cv.Required(CONF_PANEL_TX_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_PANEL_RX_PIN): pins.gpio_input_pin_schema,
        }
    )
)

# später diese schema beschreibung
# CONFIG_SCHEMA = climate.climate_schema(
#     {
#         cv.GenerateID(): cv.declare_id(JHSClimate),
#         cv.Required(CONF_AC_TX_PIN): pins.gpio_output_pin_schema,
#         cv.Required(CONF_AC_RX_PIN): pins.gpio_input_pin_schema,
#         cv.Required(CONF_PANEL_TX_PIN): pins.gpio_output_pin_schema,
#         cv.Required(CONF_PANEL_RX_PIN): pins.gpio_input_pin_schema,
#         cv.Required(CONF_WATER_FULL_SENSOR): binary_sensor.binary_sensor_schema(),
#     }
# )
# CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend(
# # CONFIG_SCHEMA = CONFIG_SCHEMA.extend(
#     {
#         cv.GenerateID(): cv.declare_id(JHSClimate),
#         cv.Required(CONF_AC_TX_PIN): pins.gpio_output_pin_schema,
#         cv.Required(CONF_AC_RX_PIN): pins.gpio_input_pin_schema,
#         cv.Required(CONF_PANEL_TX_PIN): pins.gpio_output_pin_schema,
#         cv.Required(CONF_PANEL_RX_PIN): pins.gpio_input_pin_schema,
#         cv.Required(CONF_WATER_FULL_SENSOR): binary_sensor.binary_sensor_schema(),
        
#     }
# )

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await climate.register_climate(var, config)
    
    water_full_sensor = await binary_sensor.new_binary_sensor(config[CONF_WATER_FULL_SENSOR])
    cg.add(var.set_water_full_sensor(water_full_sensor))

    ac_tx_pin = await cg.gpio_pin_expression(config[CONF_AC_TX_PIN])
    ac_rx_pin = await cg.gpio_pin_expression(config[CONF_AC_RX_PIN])
    panel_tx_pin = await cg.gpio_pin_expression(config[CONF_PANEL_TX_PIN])
    panel_rx_pin = await cg.gpio_pin_expression(config[CONF_PANEL_RX_PIN])
    cg.add(var.set_ac_tx_pin(ac_tx_pin))
    cg.add(var.set_ac_rx_pin(ac_rx_pin))
    cg.add(var.set_panel_tx_pin(panel_tx_pin))
    cg.add(var.set_panel_rx_pin(panel_rx_pin))
