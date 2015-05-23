/***************************************************************
 *
 * OpenBeacon.org - nRF51 Main Entry
 *
 * Copyright 2013 Milosch Meriac <meriac@openbeacon.de>
 *
 ***************************************************************

 This file is part of the OpenBeacon.org active RFID firmware

 OpenBeacon is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 OpenBeacon is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <openbeacon.h>

#include <timer.h>


void blink(uint8_t times)
{
	while(times--)
	{
		nrf_gpio_pin_clear(CONFIG_LED_PIN);
		timer_wait(MILLISECONDS(10));
		nrf_gpio_pin_set(CONFIG_LED_PIN);
		timer_wait(MILLISECONDS(490));
	}
}

void halt(uint8_t times)
{
	while(TRUE)
	{
		blink(times);
		timer_wait(SECONDS(3));
	}
}

void main_entry(void)
{
	uint8_t blink;

	/* enabled LED output */
	nrf_gpio_cfg_output(CONFIG_LED_PIN);
	nrf_gpio_pin_clear(CONFIG_LED_PIN);

	/* initialize UART */
	uart_init();

	/* start timer */
	timer_init();

	debug_printf("\r\nI'm here\r\n");

	/* enter main loop */
	blink = 0;
	nrf_gpio_pin_set(CONFIG_LED_PIN);
	while(TRUE)
	{
		timer_wait(MILLISECONDS(1000));

		/* blink every 5 seconds */
		if(blink<5)
			blink++;
		else
		{
			blink = 0;
			nrf_gpio_pin_clear(CONFIG_LED_PIN);
			timer_wait(MILLISECONDS(1));
			nrf_gpio_pin_set(CONFIG_LED_PIN);
		}
	}
}
