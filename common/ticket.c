/*
 * Copyright 2008, 2009 by Brian Dominy <brian@oddchange.com>
 *
 * This file is part of FreeWPC.
 *
 * FreeWPC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * FreeWPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FreeWPC; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * \file
 * \brief Manage an optional ticket dispenser
 *
 */

/* TODO - I have no idea how this ticket I/O really works */

#include <freewpc.h>

U8 ticket_out_count;

U8 ticket_switches;

U8 ticket_pulses;

__fastram__ U8 ticket_connected;

__local__ U8 tickets_awarded;


/**
 * Award 1 ticket to the player.
 */
void ticket_award (void)
{
	if (tickets_awarded >= system_config.max_tickets_per_player)
		return;

	tickets_awarded++;
	audit_increment (&system_audits.tickets_awarded);

	if (ticket_connected)
		ticket_out_count++;
}


CALLSET_ENTRY (ticket, idle_every_100ms)
{
	if (ticket_connected)
	{
		ticket_switches = pinio_read_ticket ();

		if (ticket_out_count > 0)
		{
		}

		pinio_write_ticket (ticket_pulses);
	}
}


CALLSET_ENTRY (ticket, start_player)
{
	tickets_awarded = 0;
}


CALLSET_ENTRY (ticket, init)
{
	ticket_connected = 0;
	ticket_out_count = 0;
	ticket_pulses = 0;

	if (system_config.ticket_board == NO)
		return;

	ticket_switches = pinio_read_ticket ();
	if (ticket_switches != 0xFF)
	{
		ticket_connected++;
	}
}

