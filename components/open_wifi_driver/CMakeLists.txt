idf_build_get_property(idf_target IDF_TARGET)

set(srcs 
    "src/hardware.c" 
    "src/hwinit.c"
    "src/mac.c"
    "src/netif.c")

idf_component_register(
    SRCS "${srcs}"
    INCLUDE_DIRS "include"
    REQUIRES esp_common freertos esp_event esp_timer esp_wifi esp_phy)