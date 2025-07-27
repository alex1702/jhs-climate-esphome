#pragma once

#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// AC Packet size (in bytes)
#define JHS_AC_PACKET_SIZE 13

// Panel Packet size (in bytes)
#define JHS_PANEL_PACKET_SIZE 13

// Struct to pass configuration to recv task
typedef struct {
    uint8_t ac_rx_pin;
    uint8_t panel_rx_pin;
} jhs_recv_task_config;

// Queues for received packets
extern volatile QueueHandle_t ac_rx_queue;
extern volatile QueueHandle_t panel_rx_queue;

// Starts the receive task and sets up ISR
void start_jhs_climate_recv_task(jhs_recv_task_config config);

