/***************************************************************
 *
 * OpenBeacon.org - nRF51 board config files
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
#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CONFIG_UART_BAUDRATE UART_BAUDRATE_BAUDRATE_Baud115200
#define CONFIG_UART_TXD_PIN       9
#define CONFIG_UART_RXD_PIN      11
#define CONFIG_UART_CTS_PIN      10
#define CONFIG_UART_RTS_PIN       8

#define CONFIG_LED_RGB_RED_PIN   21
#define CONFIG_LED_RGB_GREEN_PIN 22
#define CONFIG_LED_RGB_BLUE_PIN  23

#define CONFIG_LED_PIN CONFIG_LED_RGB_BLUE_PIN

/* only two priority bits available ! */

#define IRQ_PRIORITY_HIGH        0
#define IRQ_PRIORITY_AES         (IRQ_PRIORITY_HIGH)
#define IRQ_PRIORITY_RNG         (IRQ_PRIORITY_HIGH+1)
#define IRQ_PRIORITY_POWER_CLOCK (IRQ_PRIORITY_HIGH+1)
#define IRQ_PRIORITY_RADIO       (IRQ_PRIORITY_HIGH+2)

#define IRQ_PRIORITY_LOW         (IRQ_PRIORITY_HIGH+3)
#define IRQ_PRIORITY_RTC0        (IRQ_PRIORITY_LOW)
#define IRQ_PRIORITY_RTC1        (IRQ_PRIORITY_LOW)
#define IRQ_PRIORITY_ADC         (IRQ_PRIORITY_LOW)
#define IRQ_PRIORITY_UART0       (IRQ_PRIORITY_LOW)

#endif/*__CONFIG_H__*/
