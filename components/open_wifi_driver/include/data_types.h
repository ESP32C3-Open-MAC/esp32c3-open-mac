#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/** @brief Received packet radio metadata header, this is the common header at the beginning of all promiscuous mode RX callback buffers */
typedef struct {
    signed rssi:8;                /**< Received Signal Strength Indicator(RSSI) of packet. unit: dBm */
    unsigned rate:5;              /**< PHY rate encoding of the packet. Only valid for non HT(11bg) packet */
    unsigned :1;                  /**< reserved */
    unsigned sig_mode:2;          /**< 0: non HT(11bg) packet; 1: HT(11n) packet; 3: VHT(11ac) packet */
    unsigned :16;                 /**< reserved */
    unsigned mcs:7;               /**< Modulation Coding Scheme. If is HT(11n) packet, shows the modulation, range from 0 to 76(MSC0 ~ MCS76) */
    unsigned cwb:1;               /**< Channel Bandwidth of the packet. 0: 20MHz; 1: 40MHz */
    unsigned :16;                 /**< reserved */
    unsigned smoothing:1;         /**< reserved */
    unsigned not_sounding:1;      /**< reserved */
    unsigned :1;                  /**< reserved */
    unsigned aggregation:1;       /**< Aggregation. 0: MPDU packet; 1: AMPDU packet */
    unsigned stbc:2;              /**< Space Time Block Code(STBC). 0: non STBC packet; 1: STBC packet */
    unsigned fec_coding:1;        /**< Flag is set for 11n packets which are LDPC */
    unsigned sgi:1;               /**< Short Guide Interval(SGI). 0: Long GI; 1: Short GI */
#if CONFIG_IDF_TARGET_ESP32
    signed noise_floor:8;         /**< noise floor of Radio Frequency Module(RF). unit: dBm*/
#elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C2
    unsigned :8;                  /**< reserved */
#endif
    unsigned ampdu_cnt:8;         /**< ampdu cnt */
    unsigned channel:4;           /**< primary channel on which this packet is received */
    unsigned secondary_channel:4; /**< secondary channel on which this packet is received. 0: none; 1: above; 2: below */
    unsigned :8;                  /**< reserved */
    unsigned timestamp:32;        /**< timestamp. The local time when this packet is received. It is precise only if modem sleep or light sleep is not enabled. unit: microsecond */
    unsigned :32;                 /**< reserved */
#if CONFIG_IDF_TARGET_ESP32S2
    unsigned :32;                 /**< reserved */
#elif CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C2
    signed noise_floor:8;         /**< noise floor of Radio Frequency Module(RF). unit: dBm*/
    unsigned :24;                 /**< reserved */
    unsigned :32;                 /**< reserved */
#endif
    unsigned :31;                 /**< reserved */
    unsigned ant:1;               /**< antenna number from which this packet is received. 0: WiFi antenna 0; 1: WiFi antenna 1 */
#if CONFIG_IDF_TARGET_ESP32S2
    signed noise_floor:8;         /**< noise floor of Radio Frequency Module(RF). unit: dBm*/
    unsigned :24;                 /**< reserved */
#elif CONFIG_IDF_TARGET_ESP32S3 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C2
    unsigned :32;                 /**< reserved */
    unsigned :32;                 /**< reserved */
    unsigned :32;                 /**< reserved */
#endif
    unsigned sig_len:12;          /**< length of packet including Frame Check Sequence(FCS) */
    unsigned :12;                 /**< reserved */
    unsigned rx_state:8;          /**< state of the packet. 0: no error; others: error numbers which are not public */
} wifi_pkt_rx_ctrl_openmac_t;

/** @brief Payload passed to 'buf' parameter of promiscuous mode RX callback.
 */
typedef struct {
    wifi_pkt_rx_ctrl_openmac_t rx_ctrl; /**< metadata header */
    uint8_t payload[0];       /**< Data or management payload. Length of payload is described by rx_ctrl.sig_len. Type of content determined by packet type argument of callback. */
} wifi_promiscuous_pkt_openmac_t;


typedef enum {
    EVENT_TYPE_MAC_TX_DATA_FRAME,
    EVENT_TYPE_MAC_FREE_RX_DATA,
    EVENT_TYPE_PHY_RX_DATA,
} mac_event_type_t;


typedef struct __attribute__((packed)) dma_list_item { // TODO replace the fields with the names from the dma struct in the ESP IDF
	uint16_t size : 12;
	uint16_t length : 12;
	uint8_t _unknown : 6;
	uint8_t has_data : 1;
	uint8_t owner : 1;
	wifi_promiscuous_pkt_openmac_t* packet;
	struct dma_list_item* next;
} dma_list_item;


typedef struct {
	uint8_t* buffer;
	bool in_use;
	size_t buffersize;
} mac_rx_frame_t;


typedef struct {
    uint8_t* payload;
    size_t payload_length; // modifiable, contains the amount of valid data in payload
    size_t payload_size; // not modifiable, contains at all times the actual length of payload buffer
    uint32_t rate;
} smart_frame_t; // has a frame, and the metadata (rate, length, ...)

typedef struct {
	smart_frame_t* frame;
	bool in_use;
} smart_frame_tracker_t;

typedef struct {
	void* ptr;
	mac_event_type_t event_type;
	size_t len;
} mac_event_queue_item_t;


typedef enum {
    MAC_INACTIVE, // MAC state is inactive
    MAC_SCAN, // MAC actively scans for APs i.e. receives beacons
    MAC_AUTHENTICATE, // MAC sends auth frame and awaits auth response
    MAC_ASSOCIATE, // MAC sends assoc frame and awaits response
    MAC_CONNECTED // MAC is connected to AP with known SSID
} mac_state_t;


