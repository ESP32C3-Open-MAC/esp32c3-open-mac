#pragma once
#include "esp_wifi.h"

typedef void rx_callback(wifi_promiscuous_pkt_t* packet);

typedef bool tx_func(uint8_t* packet, uint32_t len);

typedef void tx_func_callback(tx_func* t);

typedef struct hardware_mac_args {
	rx_callback* _rx_callback;
    tx_func_callback* _tx_func_callback;
} hardware_mac_args;

void wifi_hardware_task(void* pvParameter);
extern uint8_t module_mac_addr[6];

#define _MMIO_DWORD(mem_addr) (*(volatile uint32_t *)(mem_addr))
#define _MMIO_ADDR(mem_addr) ((volatile uint32_t*)(mem_addr))

// there are 5 TX slots
// format: _BASE addresses are the base addresses
//         _OS amounts is the amount of 4-byte words in the offset between slots
// So for example, if the MAC_TX_PLCP0 for slot 0 is at 0x3ff73d20
// then the MAC_TX_PLCP0 for slot 1 will be at 0x3ff73d20 - 2 * 4 = 0x3ff73d18

#define MAC_TX_PLCP0_BASE _MMIO_ADDR(0x60033d08)
#define MAC_TX_PLCP0_OS (-2)

#define WIFI_TX_CONFIG_BASE _MMIO_ADDR(0x60033d04)
#define WIFI_TX_CONFIG_OS (-2)


#define MAC_TX_PLCP1_BASE _MMIO_ADDR(0x600342f8)
#define MAC_TX_PLCP1_OS (-0xf)

#define MAC_TX_PLCP2_BASE _MMIO_ADDR(0x600342fc)
#define MAC_TX_PLCP2_OS (-0xf)

#define MAC_TX_DURATION_BASE _MMIO_ADDR(0x60034318)
#define MAC_TX_DURATION_OS (-0xf)

#define MAC_TX_HT_SIG_BASE _MMIO_ADDR(0x60034310)
#define MAC_TX_HT_SIG_OS (-0xf)

#define MAC_TX_HT_UNKNOWN_BASE _MMIO_ADDR(0x60034314)
#define MAC_TX_HT_UNKNOWN_OS (-0xf)

#define WIFI_DMA_INT_STATUS _MMIO_DWORD(0x60033c3c)
#define WIFI_DMA_INT_CLR _MMIO_DWORD(0x60033c40)

#define WIFI_MAC_BITMASK_084 _MMIO_DWORD(0x60033084)
#define WIFI_NEXT_RX_DSCR _MMIO_DWORD(0x6003308c)
#define WIFI_LAST_RX_DSCR _MMIO_DWORD(0x60033090)
#define WIFI_BASE_RX_DSCR _MMIO_DWORD(0x60033088)

#define WIFI_TXQ_GET_STATE_COMPLETE _MMIO_DWORD(0x60033cb0)
#define WIFI_TXQ_CLR_STATE_COMPLETE _MMIO_DWORD(0x60033cac)

// Collision or timeout
#define WIFI_TXQ_GET_STATE_ERROR _MMIO_DWORD(0x60033ca8)
#define WIFI_TXQ_CLR_STATE_ERROR _MMIO_DWORD(0x60033ca4)


#define WIFI_MAC_ADDR_SLOT_0 0x60033040
#define WIFI_MAC_ADDR_ACK_ENABLE_SLOT_0 0x60033064

#define WIFI_BSSID_FILTER_ADDR_SLOT_0 _MMIO_ADDR(0x60033000)


#define MAC_CTRL_REG _MMIO_DWORD(0x60033ca0)

// Intererupt registers
#define INTR_SRC_MAC 0x600c2000
#define INTR_SRC_PWR 0x600c2008
#define INTR_ENABLE_REG 0x600c2104 // Writing a 1 to corresponding bit enables and writing 0 disables
#define INTR_STATUS_REG 0x600c00F8

#define WIFI_INTR_NUMBER 1
#define SYSTICK_INTR_NUMBER 7 // Tick 
#define TIMER_ALARM_NUMBER 3
#define TASK_WDT_NUMBER 9
