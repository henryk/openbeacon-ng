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

extern void radio_init(void);
extern int radio_transmit(uint8_t channel, uint8_t *packet);
extern int radio_receive(uint8_t channel, uint8_t *packet);
