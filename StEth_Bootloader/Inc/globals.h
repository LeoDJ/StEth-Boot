#pragma once
// #include "stm32f1xx_hal.h"
#include "main.h"
#include "util.h"

#define PIN_OE  GPIOB, GPIO_PIN_4
#define PIN_LAT GPIOB, GPIO_PIN_5
#define PIN_LED GPIOC, GPIO_PIN_13

#define _WIZCHIP_           5500
#define ETH_SPI             SPI2
// extern SPI_TypeDef          SPI2
#define SPI2_NSS            GPIOB, GPIO_PIN_12
#define SOCKET_DHCP         0