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

#include <radio.h>
#include <timer.h>

#define CRC_POLY 0x8408

#define PACKET_LENGTH (1+2+7+2)       /* 1 Byte S0, 1.5 Bytes S1, 7 Bytes data, 2 Bytes CRC */
static uint8_t packet[PACKET_LENGTH]; /* Packet to receive. */


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

uint16_t calc_crc(uint8_t *data, size_t data_length) {
	uint16_t state = 0;
	for (size_t i = 0; i < data_length; i++) {
		uint8_t byte = data[i];
		for (int j = 0; j < 8; j++) {
			if ((byte ^ state) & 0x01) {
				state = (state >> 1) ^ CRC_POLY;
			} else {
				state = state >> 1;
			}
			byte = byte >> 1;
		}
	}
	return state;
}


static void demangle_packet(uint8_t *in, uint8_t *out) {
	int i = 0;
	memset(out, 0, 10);

	in++; // packet[0] is 0x25 (part of original sync)
	while (i < 11) {
		if (i > 0)
			out[i - 1] |= ((in[0]) & 0xF) << 4;
		i++;
		if (i < 11)
			out[i - 1] |= (in[0] >> 4) & 0xF;
		in++;
	}
}

static void mangle_packet(uint8_t *out, uint8_t *in) {
	int i = 0;
	memset(out, 0, 12);

	*out++ = 0x25; // part of original sync
	*out = 0x05; // original trailer

	while (i < 10) {
		*out |= (in[i] & 0xf) << 4;
		out++;
		*out |= (in[i] >> 4) & 0xf;
		i++;
	}

	while (i < 11) {
		if (i < 11)
			*out |= (in[i - 1] >> 4) & 0xf;
		out++;
		if (i > 0)
			*out |= ((in[i - 1] & 0xf) << 4);
		i++;
	}
}

static void __attribute__((unused)) dump_milight_packet(uint8_t *packet) {
	static uint8_t prev_packet[10];
	uint8_t tmp[10] = { 0 };
	demangle_packet(packet, tmp);

	uint16_t crc = calc_crc(tmp, 8);
	if ((tmp[8] != (crc & 0xff)) || (tmp[9] != ((crc >> 8) & 0xff))) {
		return;
	}

	if (memcmp(tmp, prev_packet, 10) != 0) {
		debug_printf("\r\n");
		for (int i = 0; i < 10; i++) {
			debug_printf("%02X ", tmp[i]);
		}
		memcpy(prev_packet, tmp, 10);
	} else {
		debug_printf(".");
	}
}

static void __attribute__((unused)) send_milight(uint32_t sender_id, uint8_t val_1, uint8_t val_2,
		uint8_t cmd, uint8_t counter, uint8_t repetitions) {
	uint8_t tmp[10] = { 0 };
	tmp[0] = 0x07;
	tmp[1] = sender_id >> 16;
	tmp[2] = sender_id >> 8;
	tmp[3] = sender_id >> 0;
	tmp[4] = val_1;
	tmp[5] = val_2;
	tmp[6] = cmd;
	tmp[7] = counter;
	uint16_t crc = calc_crc(tmp, 8);
	tmp[8] = crc;
	tmp[9] = crc >> 8;

	mangle_packet(packet, tmp);

	int freqs[] = { 11, 42, 73 };

	for (int i = 0; i < repetitions; i++) {
		for (size_t f = 0; f < sizeof(freqs) / sizeof(freqs[0]); f++) {
			radio_transmit(freqs[f], packet);
		}
		timer_wait(MILLISECONDS(0.8));
	}
}


void clock_init(void)
{
	/* Get HFCLOCK from the 16 MHz crystal */
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 1;
	while(!NRF_CLOCK->EVENTS_HFCLKSTARTED) ;
}

void main_entry(void)
{
	/* enabled LED output */
	nrf_gpio_cfg_output(CONFIG_LED_PIN);
	nrf_gpio_pin_clear(CONFIG_LED_PIN);

	/* initialize clock */
	clock_init();

	/* initialize UART */
	uart_init();

	/* start timer */
	timer_init();

	/* set up radio */
	radio_init();

	debug_printf("\r\nI'm here\r\n");

	/* enter main loop */
	nrf_gpio_pin_set(CONFIG_LED_PIN);

	uint32_t remote_id = 0xB0F2EA;
	int counter = 0;

	if(0) while (TRUE) {
		send_milight(remote_id, 0, 0, 0x11, counter, 40);
		counter++;

		blink(1);
		timer_wait(MILLISECONDS(500));

		send_milight(remote_id, 0, 0, 0x02, counter, 40);
		counter++;

		blink(1);
		timer_wait(MILLISECONDS(500));
	}

	while(TRUE)
	{
		radio_receive(11, packet);
		dump_milight_packet(packet);

	}

}
