/***************************************************************
 *
 * OpenBeacon.org - Mi-Light radio routines
 *
 * Copyright 2015 Henryk Plötz <henryk@ploetzli.ch>
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

int radio_receive(uint8_t channel, uint8_t *packet)
{
	NRF_RADIO->PACKETPTR = (uint32_t)packet;
	NRF_RADIO->FREQUENCY = channel;

	NRF_RADIO->EVENTS_READY = 0U;
	// Enable radio and wait for ready
	NRF_RADIO->TASKS_RXEN = 1U;

	while (NRF_RADIO->EVENTS_READY == 0U) {
		// wait
	}
	NRF_RADIO->EVENTS_END = 0U;
	// Start listening and wait for address received event
	NRF_RADIO->TASKS_START = 1U;

	// Wait for end of packet
	while (NRF_RADIO->EVENTS_END == 0U) {
		// wait
	}

	NRF_RADIO->EVENTS_DISABLED = 0U;
	// Disable radio
	NRF_RADIO->TASKS_DISABLE = 1U;

	while (NRF_RADIO->EVENTS_DISABLED == 0U) {
		// wait
	}
	return 0;
}

int radio_transmit(uint8_t channel, uint8_t *packet)
{
	NRF_RADIO->PACKETPTR = (uint32_t)packet;
	NRF_RADIO->FREQUENCY = channel;

	// send the packet:
	NRF_RADIO->EVENTS_READY = 0U;
	NRF_RADIO->TASKS_TXEN = 1;

	while (NRF_RADIO->EVENTS_READY == 0U) {
		// wait
	}
	NRF_RADIO->EVENTS_END = 0U;
	NRF_RADIO->TASKS_START = 1U;

	while (NRF_RADIO->EVENTS_END == 0U) {
		// wait
	}

	NRF_RADIO->EVENTS_DISABLED = 0U;
	// Disable radio
	NRF_RADIO->TASKS_DISABLE = 1U;

	while (NRF_RADIO->EVENTS_DISABLED == 0U) {
		// wait
	}

	return 0;
}



void radio_init(void)
{
	/* Radio config */
	NRF_RADIO->TXPOWER = (RADIO_TXPOWER_TXPOWER_Pos4dBm
			<< RADIO_TXPOWER_TXPOWER_Pos);
	NRF_RADIO->FREQUENCY = 11UL;  // Frequency bin 11, 2411MHz
	NRF_RADIO->MODE = (RADIO_MODE_MODE_Nrf_1Mbit << RADIO_MODE_MODE_Pos);

	/* Radio address config for Mi-Light packets*/
	NRF_RADIO->PREFIX0 = 0x8b;

	NRF_RADIO->BASE0 = 0x147aaaaa;

	NRF_RADIO->TXADDRESS = 0x00UL;
	NRF_RADIO->RXADDRESSES = 0x01UL;

	/* Packet configuration: no enhanced features */
	NRF_RADIO->PCNF0 = (0 << RADIO_PCNF0_S1LEN_Pos)
			| (0 << RADIO_PCNF0_S0LEN_Pos)
			| (0 << RADIO_PCNF0_LFLEN_Pos); //lint !e845 "The right argument to operator '|' is certain to be 0"

	/* Packet configuration: no whitening, little endian, 4 bytes base address, 12 byte packet length */
	NRF_RADIO->PCNF1 = (RADIO_PCNF1_WHITEEN_Disabled << RADIO_PCNF1_WHITEEN_Pos)
			| (RADIO_PCNF1_ENDIAN_Little << RADIO_PCNF1_ENDIAN_Pos)
			| (4 << RADIO_PCNF1_BALEN_Pos)
			| (12 << RADIO_PCNF1_STATLEN_Pos)
			| (12 << RADIO_PCNF1_MAXLEN_Pos); //lint !e845 "The right argument to operator '|' is certain to be 0"

	/* CRC Config: No CRC */
	NRF_RADIO->CRCCNF = (RADIO_CRCCNF_LEN_Disabled << RADIO_CRCCNF_LEN_Pos);

}
