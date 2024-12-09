#pragma once
#include "esp_wifi.h"

void wifi_hardware_task(void* pvParameter);
extern uint8_t module_mac_addr[6];

extern QueueHandle_t driver_event_q_hdl;
extern StaticQueue_t driver_event_q;

extern StaticSemaphore_t driver_event_q_sem_buf;
extern SemaphoreHandle_t driver_event_q_sem_hdl;

extern QueueHandle_t mac_event_q_hdl;
extern StaticQueue_t mac_event_q;

extern StaticSemaphore_t mac_event_q_sem_buf;
extern SemaphoreHandle_t mac_event_q_sem_hdl;

#define _MMIO_DWORD(mem_addr) (*(volatile uint32_t *)(mem_addr))
#define _MMIO_ADDR(mem_addr) ((volatile uint32_t*)(mem_addr))