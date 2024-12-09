#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "data_types.h"

void mac_layer_init(void);
void mac_layer_deinit(void);
void mac_task(void);