#pragma once
#include "main.h"
#include <stdbool.h>

bool nvm_flash_erase_page(uint32_t *address);
bool nvm_flash_write_half_page(uint32_t *address, uint32_t *data);

#define FLASH_KEY1 0x45670123
#define FLASH_KEY2 0xCDEF89AB