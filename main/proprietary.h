#pragma once
#include <stdint.h>

// All extern function shown here are symbols in the binary blobs
extern bool pp_post(uint32_t requestnum, uint32_t argument);

// Interrupt-related functions
// extern void xt_unhandled_interrupt(void * arg);
extern uint32_t config_get_wifi_task_core_id();

// extern void wdev_process_panic_watchdog();

// Power calibration of TX
extern void tx_pwctrl_background(int a, int b);

// changing channel
extern void chip_v7_set_chan(uint8_t channel, uint8_t _unknown);
extern void rom_disable_wifi_agc();
extern void rom_enable_wifi_agc();
