// #include "jhs_recv_task.h"
// #include <cstring>
// #include <freertos/FreeRTOS.h>
#include "jhs_recv_task.h"
#include <cstring>
#include "driver/gpio.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// volatile QueueHandle_t ac_rx_queue;
// volatile QueueHandle_t panel_rx_queue;

QueueHandle_t ac_rx_queue;
QueueHandle_t panel_rx_queue;

static TaskHandle_t interrupt_task;

// static volatile unsigned long ac_rx_last_falling_edge_time = 0;
static volatile uint64_t ac_rx_last_falling_edge_time = 0;
static volatile unsigned int ac_rx_bits_from_start = 0;
static volatile uint8_t ac_rx_packet[JHS_AC_PACKET_SIZE];

// static void IRAM_ATTR jhs_ac_rx_isr()
// {
//     unsigned long length = micros() - ac_rx_last_falling_edge_time;
//     ac_rx_last_falling_edge_time = micros();
//     if (length > 20 && length < 32 * 250)
//     {
//         if (length < 2 * 250 + 280)
//         {
//             // zero
//             ac_rx_bits_from_start++;
//         }
//         else if (length < 4 * 250 + 250)
//         {
//             // set bit in packet to one
//             ac_rx_packet[ac_rx_bits_from_start / 8] |= (1 << (7 - ac_rx_bits_from_start % 8));
//             ac_rx_bits_from_start++;
//         }
//         else
//         {
//             // start
//             ac_rx_bits_from_start = 0;
//             // clear packet
//             for (int i = 0; i < JHS_AC_PACKET_SIZE; i++)
//             {
//                 ac_rx_packet[i] = 0;
//             }
//         }
//         if (ac_rx_bits_from_start == JHS_AC_PACKET_SIZE * 8)
//         {
//             ac_rx_bits_from_start = 0;

//             xQueueSend(ac_rx_queue, (void *) ac_rx_packet, 0);
//         }
//     }
// }

static void IRAM_ATTR jhs_ac_rx_isr(void* arg)
{
    uint64_t now = esp_timer_get_time();
    unsigned long length = now - ac_rx_last_falling_edge_time;
    ac_rx_last_falling_edge_time = now;

    if (length > 20 && length < 32 * 250)
    {
        if (length < 2 * 250 + 280)
        {
            // zero
            ac_rx_bits_from_start++;
        }
        else if (length < 4 * 250 + 250)
        {
            // set bit in packet to one
            ac_rx_packet[ac_rx_bits_from_start / 8] |= (1 << (7 - ac_rx_bits_from_start % 8));
            ac_rx_bits_from_start++;
        }
        else
        {
            // start
            ac_rx_bits_from_start = 0;
            // clear packet
            for (int i = 0; i < JHS_AC_PACKET_SIZE; i++)
            {
                ac_rx_packet[i] = 0;
            }
        }
        if (ac_rx_bits_from_start == JHS_AC_PACKET_SIZE * 8)
        {
            ac_rx_bits_from_start = 0;

            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xQueueSendFromISR(ac_rx_queue, (void *)ac_rx_packet, &xHigherPriorityTaskWoken);
            if (xHigherPriorityTaskWoken)
                portYIELD_FROM_ISR();
        }
    }
}

static volatile unsigned long panel_rx_last_falling_edge_time = 0;
static volatile unsigned int panel_rx_bits_from_start = 0;
static volatile uint8_t panel_rx_packet[JHS_PANEL_PACKET_SIZE];

// static void IRAM_ATTR jhs_panel_rx_isr()
// {
//     unsigned long length = micros() - panel_rx_last_falling_edge_time;
//     panel_rx_last_falling_edge_time = micros();
//     if (length > 20 && length < 32 * 250)
//     {
//         if (length < 2 * 250 + 280)
//         {
//             // zero
//             panel_rx_bits_from_start++;
//         }
//         else if (length < 4 * 250 + 250)
//         {
//             // set bit in packet to one
//             panel_rx_packet[panel_rx_bits_from_start / 8] |= (1 << (7 - panel_rx_bits_from_start % 8));
//             panel_rx_bits_from_start++;
//         }
//         else
//         {
//             // start
//             panel_rx_bits_from_start = 0;
//             // clear packet
//             for (int i = 0; i < JHS_PANEL_PACKET_SIZE; i++)
//             {
//                 panel_rx_packet[i] = 0;
//             }
//         }
//         if (panel_rx_bits_from_start == JHS_PANEL_PACKET_SIZE * 8)
//         {
//             panel_rx_bits_from_start = 0;

//             xQueueSend(panel_rx_queue, (void *) panel_rx_packet, 0);
//         }
//     }
// }

static void IRAM_ATTR jhs_panel_rx_isr(void* arg)
{
    uint64_t now = esp_timer_get_time();
    unsigned long length = now - panel_rx_last_falling_edge_time;
    panel_rx_last_falling_edge_time = now;

    if (length > 20 && length < 32 * 250)
    {
        if (length < 2 * 250 + 280)
        {
            // zero
            panel_rx_bits_from_start++;
        }
        else if (length < 4 * 250 + 250)
        {
            // set bit in packet to one
            panel_rx_packet[panel_rx_bits_from_start / 8] |= (1 << (7 - panel_rx_bits_from_start % 8));
            panel_rx_bits_from_start++;
        }
        else
        {
            // start
            panel_rx_bits_from_start = 0;
            // clear packet
            for (int i = 0; i < JHS_PANEL_PACKET_SIZE; i++)
            {
                panel_rx_packet[i] = 0;
            }
        }
        if (panel_rx_bits_from_start == JHS_PANEL_PACKET_SIZE * 8)
        {
            panel_rx_bits_from_start = 0;

            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xQueueSendFromISR(panel_rx_queue, (void *)panel_rx_packet, &xHigherPriorityTaskWoken);
            if (xHigherPriorityTaskWoken)
                portYIELD_FROM_ISR();
        }
    }
}

// static void jhs_recv_task_func(void *arg)
// {
//     jhs_recv_task_config *config_ptr = (jhs_recv_task_config *)arg;

//     pinMode(config_ptr->ac_rx_pin, INPUT);
//     pinMode(config_ptr->panel_rx_pin, INPUT_PULLDOWN);
//     attachInterrupt(config_ptr->ac_rx_pin, jhs_ac_rx_isr, FALLING);
//     attachInterrupt(config_ptr->panel_rx_pin, jhs_panel_rx_isr, FALLING);

//     free(config_ptr);
//     vTaskDelete(NULL);
// }

static void jhs_recv_task_func(void *arg)
{
    jhs_recv_task_config *config_ptr = (jhs_recv_task_config *)arg;

    // AC RX Pin Setup
    gpio_config_t ac_conf = {};
    ac_conf.pin_bit_mask = (1ULL << config_ptr->ac_rx_pin);
    ac_conf.mode = GPIO_MODE_INPUT;
    ac_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    ac_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    ac_conf.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&ac_conf);

    // Panel RX Pin Setup
    gpio_config_t panel_conf = {};
    panel_conf.pin_bit_mask = (1ULL << config_ptr->panel_rx_pin);
    panel_conf.mode = GPIO_MODE_INPUT;
    panel_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    panel_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    panel_conf.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&panel_conf);

    // Install ISR service and add handlers
    gpio_install_isr_service(0);
    gpio_isr_handler_add(config_ptr->ac_rx_pin, jhs_ac_rx_isr, NULL);
    gpio_isr_handler_add(config_ptr->panel_rx_pin, jhs_panel_rx_isr, NULL);

    delete config_ptr;
    vTaskDelete(NULL);
}

void start_jhs_climate_recv_task(jhs_recv_task_config config)
{
    jhs_recv_task_config *config_ptr = new jhs_recv_task_config(config);
    ac_rx_queue = xQueueCreate(32, JHS_AC_PACKET_SIZE);
    panel_rx_queue = xQueueCreate(32, JHS_PANEL_PACKET_SIZE);
    xTaskCreatePinnedToCore(jhs_recv_task_func, "jhs_recv_task", 2048, config_ptr, 5, &interrupt_task, 0);
}