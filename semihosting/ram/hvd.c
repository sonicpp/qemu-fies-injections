/*
 * hvd.c - Horizontal-Vertical-Diagonal Parity.
 *
 * Copyright (C) 2016 Jan Havran
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <angelio.h>
#include "hvd.h"

/** 
 * HVD_calc_h_parity - Calc Horizontal Parity
 *
 * Each parity bit is result of XOR of each bit in horizontal line.
 * For example, parity "p03" is XOR of all data bits marked as "03".
 * Example of 8*8 data bits is shown in the following table:
 *
 * | 01| 01| 01| 01| 01| 01| 01| 01||p01
 * | 02| 02| 02| 02| 02| 02| 02| 02||p02
 * | 03| 03| 03| 03| 03| 03| 03| 03||p03
 * | 04| 04| 04| 04| 04| 04| 04| 04||p04
 * | 05| 05| 05| 05| 05| 05| 05| 05||p05
 * | 06| 06| 06| 06| 06| 06| 06| 06||p06
 * | 07| 07| 07| 07| 07| 07| 07| 07||p07
 * | 08| 08| 08| 08| 08| 08| 08| 08||p08
 *
 * Parity stored in memory:
 *          MSB                         LSB
 * word 1: |p08|p07|p06|p05|p04|p03|p02|p01|
 */
static void HVD_calc_h_parity(struct HVD *hvd)
{
	/* parity mask for nibble:
		0 1 1 0 1 0 0 1
		1 0 0 1 0 1 1 0
	*/

	unsigned parity = 0x6996;

	for (int i = 0; i < BLOCKS; i++) {
		unsigned a = 0;
		unsigned b = 0;
		unsigned c = 0;
		unsigned d = 0;
		unsigned e = 0;
		unsigned f = 0;
		unsigned g = 0;
		unsigned h = 0;
		for (int j = 0; j < 32; j++) {
			unsigned data = hvd->data[j + 32 * i];
			a |= ((parity >> (data & 0xf)) & 0x1) << j;
			b |= ((parity >> ((data >> 4) & 0xf)) & 0x1) << j;
			c |= ((parity >> ((data >> 8) & 0xf)) & 0x1) << j;
			d |= ((parity >> ((data >> 12) & 0xf)) & 0x1) << j;
			e |= ((parity >> ((data >> 16) & 0xf)) & 0x1) << j;
			f |= ((parity >> ((data >> 20) & 0xf)) & 0x1) << j;
			g |= ((parity >> ((data >> 24) & 0xf)) & 0x1) << j;
			h |= ((parity >> ((data >> 28) & 0xf)) & 0x1) << j;
		}
		hvd->h[i] = a ^ b ^ c ^ d ^ e ^ f ^ g ^ h;
	}
}

/** 
 * HVD_calc_v_parity - Calc Vertical Parity
 *
 * Each parity bit is result of XOR of each bit in vertical line.
 * For example, parity "p03" is XOR of all data bits marked as "03".
 * Example of 8*8 data bits is shown in the following table:
 *
 * |p01|p02|p03|p04|p05|p06|p07|p08
 *  -------------------------------
 * | 01| 02| 03| 04| 05| 06| 07| 08
 * | 01| 02| 03| 04| 05| 06| 07| 08
 * | 01| 02| 03| 04| 05| 06| 07| 08
 * | 01| 02| 03| 04| 05| 06| 07| 08
 * | 01| 02| 03| 04| 05| 06| 07| 08
 * | 01| 02| 03| 04| 05| 06| 07| 08
 * | 01| 02| 03| 04| 05| 06| 07| 08
 * | 01| 02| 03| 04| 05| 06| 07| 08
 *
 * Parity stored in memory:
 *          MSB                         LSB
 * word 1: |p01|p02|p03|p04|p05|p06|p07|p08|
 */
static void HVD_calc_v_parity(struct HVD *hvd)
{
	hvd->v = 0;
	for (int i = 0; i < ROWS; i++)
		hvd->v ^= hvd->data[i];
}

/** 
 * HVD_calc_sd_parity - Calc Slash Diagonal Parity
 *
 * Each parity bit is result of XOR of each bit in slash diagonal
 * line. For example, parity "p03" is XOR of all data bits marked as "03".
 * Example of 8*8 data bits is shown in the following table:
 *
 *     |p01|p02|p03|p04|p05|p06|p07||p08
 *  -------------------------------\|
 * | 01| 02| 03| 04| 05| 06| 07| 08||p09
 * | 02| 03| 04| 05| 06| 07| 08| 09||p10
 * | 03| 04| 05| 06| 07| 08| 09| 10||p11
 * | 04| 05| 06| 07| 08| 09| 10| 11||p12
 * | 05| 06| 07| 08| 09| 10| 11| 12||p13
 * | 06| 07| 08| 09| 10| 11| 12| 13||p14
 * | 07| 08| 09| 10| 11| 12| 13| 14||p15
 * | 08| 09| 10| 11| 12| 13| 14| 15
 *
 * Parity stored in memory:
 *          MSB                         LSB
 * word 1: |p01|p02|p03|p04|p05|p06|p07|p08|
 * word 2: |p09|p10|p11|p12|p13|p14|p15|---|
 */
static void HVD_calc_sd_parity(struct HVD *hvd)
{
	int parity = 0;

	for (int i = 0; i <  BLOCKS; i++) {
		hvd->sd[i] = 0;
		for (int j = 0; j < 32; j++) {
			parity ^= hvd->data[j + 32 * i];
			hvd->sd[i] |= (parity >> 31) & 0x1;
			hvd->sd[i] <<= (j == 31) ? 0 : 1;
			parity <<= 1;
		}
	}
	hvd->sd[BLOCKS] = parity;
}

/** 
 * HVD_calc_bd_parity - Calc BackSlash Diagonal Parity
 *
 * Each parity bit is result of XOR of each bit in back slash diagonal
 * line. For example, parity "p03" is XOR of all data bits marked as "03".
 * Example of 8*8 data bits is shown in the following table:
 *
 * |p08||p07|p06|p05|p04|p03|p02|p01
 *     |/---------------------------
 * |p09|| 08| 07| 06| 05| 04| 03| 02| 01
 * |p10|| 09| 08| 07| 06| 05| 04| 03| 02
 * |p11|| 10| 09| 08| 07| 06| 05| 04| 03
 * |p12|| 11| 10| 09| 08| 07| 06| 05| 04
 * |p13|| 12| 11| 10| 09| 08| 07| 06| 05
 * |p14|| 13| 12| 11| 10| 09| 08| 07| 06
 * |p15|| 14| 13| 12| 11| 10| 09| 08| 07
 *        15| 14| 13| 12| 11| 10| 09| 08
 *
 * Parity stored in memory:
 *          MSB                         LSB
 * word 1: |p01|p02|p03|p04|p05|p06|p07|p08|
 * word 2: |p09|p10|p11|p12|p13|p14|p15|---|
 */
static void HVD_calc_bd_parity(struct HVD *hvd)
{
	unsigned int parity = 0;

	for (int i = 0; i <  BLOCKS; i++) {
		hvd->bd[i] = 0;
		for (int j = 0; j < 32; j++) {
			parity ^= hvd->data[j + 32 * i];
			hvd->bd[i] |= parity & 0x1;
			hvd->bd[i] <<= (j == 31) ? 0 : 1;
			parity >>= 1;
		}
	}

	hvd->bd[BLOCKS] = 0;
	/* Revert last parity word */
	for (int i = 0; i < 30; i++) {
		hvd->bd[BLOCKS] |= parity & 0x1;
		parity >>= 1;
		hvd->bd[BLOCKS] <<= 1;
	}
	hvd->bd[BLOCKS] <<= 1;
}

void HVD_calc_parity(struct HVD *hvd)
{
	HVD_calc_h_parity(hvd);
	HVD_calc_v_parity(hvd);
	HVD_calc_sd_parity(hvd);
	HVD_calc_bd_parity(hvd);
}
	
int HVD_repair_data(struct HVD *hvd_t, struct HVD *hvd_r)
{
	int flips = 0;

	/* For every erroneous row... */
	for (int i = 0; i < ROWS; i++) {
	if (
		(hvd_t->h[i / 32] >> i % 32 & 0x1) !=
		(hvd_r->h[i / 32] >> i % 32 & 0x1)
	) {
		/* ...every erroneous column... */
		for (int j = 0; j < 32; j++) {
		if (
			((hvd_r->v >> (31 - j)) & 0x1) !=
			((hvd_t->v >> (31 - j)) & 0x1)
		) {
			int sd_b = (i + j) / 32;
			int sd_r = (i + j) % 32;
			int bd_b = (i + 31 - j) / 32;
			int bd_r = (i + 31 - j) % 32;
			/* ...check slash and backslash diagonals, if they
			 * are erroneous too */
			if (
				(
				((hvd_r->sd[sd_b] >> (31 - sd_r)) & 0x1) !=
				((hvd_t->sd[sd_b] >> (31 - sd_r)) & 0x1)
				) &
				(
				((hvd_r->bd[bd_b] >> (31 - bd_r)) & 0x1) !=
				((hvd_t->bd[bd_b] >> (31 - bd_r) & 0x1))
				)
			) {
				/* flip erroneous bit to correct */
				aprintf("col %d row %d mismatch\n", j, i);
				hvd_r->data[i] ^= 1 << (31 - j);
				flips++;
			}
		}
		}
	}
	}

	return flips;
}

int HVD_check_parity(struct HVD *hvd_r, struct HVD *hvd_t)
{
	if (hvd_r->v != hvd_t->v) return 0;
	for (int i = 0; i < BLOCKS; i++) {
		if (hvd_r->h[i] != hvd_t->h[i])
			return 0;
		if (hvd_r->sd[i] != hvd_t->sd[i])
			return 0;
		if (hvd_r->bd[i] != hvd_t->bd[i])
			return 0;
	}
	if (hvd_r->sd[BLOCKS] != hvd_t->sd[BLOCKS])
		return 0;
	if (hvd_t->bd[BLOCKS] != hvd_t->bd[BLOCKS])
		return 0;

	return 1;
}

