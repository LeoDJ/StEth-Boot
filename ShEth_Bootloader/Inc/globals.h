#pragma once
#include "stm32f1xx_hal.h"

#define PIN_OE  GPIOB, GPIO_PIN_4
#define PIN_LAT GPIOB, GPIO_PIN_5
#define PIN_LED GPIOC, GPIO_PIN_13

#define _WIZCHIP_           5500
#define ETH_SPI             hspi2
extern SPI_HandleTypeDef    hspi2;
#define SPI2_NSS            GPIOB, GPIO_PIN_12
#define SOCKET_DHCP         0