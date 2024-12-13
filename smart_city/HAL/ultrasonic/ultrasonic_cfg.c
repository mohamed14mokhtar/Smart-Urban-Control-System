/*
 * ultrasonic_cfg.c
 *
 *  Created on: Sep 12, 2024
 *      Author: mokhtar
 */


#include "ultrasonic_cfg.h"


ultra_pins_t ultra_1 = {
		.trigger_port = GPIOB,
		.trigger_pin = GPIO_PIN_0,
		.cc_select = cc2,
		.htim = &htim1
};
