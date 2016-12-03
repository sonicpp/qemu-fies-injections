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
static void HVD_calc_h_parity(struct HVD *hvd, unsigned *h)
{
	/* parity mask for nibble:
		0 1 1 0 1 0 0 1
		1 0 0 1 0 1 1 0
	*/

	unsigned parity = 0x6996;

	for (int i = 0; i < BLOCKS; i++) {
		unsigned n1, n2, n3, n4, n5, n6, n7, n8;
		n1 = n2 = n3 = n4 = n5 = n6 = n7 = n8 = 0;
		for (int j = 0; j < 32; j++) {
			unsigned data = hvd->data[j + 32 * i];
			n1 |= ((parity >> (data & 0xf)) & 0x1) << j;
			n2 |= ((parity >> ((data >> 4) & 0xf)) & 0x1) << j;
			n3 |= ((parity >> ((data >> 8) & 0xf)) & 0x1) << j;
			n4 |= ((parity >> ((data >> 12) & 0xf)) & 0x1) << j;
			n5 |= ((parity >> ((data >> 16) & 0xf)) & 0x1) << j;
			n6 |= ((parity >> ((data >> 20) & 0xf)) & 0x1) << j;
			n7 |= ((parity >> ((data >> 24) & 0xf)) & 0x1) << j;
			n8 |= ((parity >> ((data >> 28) & 0xf)) & 0x1) << j;
		}
		h[i] = n1 ^ n2 ^ n3 ^ n4 ^ n5 ^ n6 ^ n7 ^ n8;
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
static void HVD_calc_v_parity(struct HVD *hvd, unsigned *v)
{
	*v = 0;
	for (int i = 0; i < ROWS; i++)
		*v ^= hvd->data[i];
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
static void HVD_calc_sd_parity(struct HVD *hvd, unsigned *sd)
{
	int parity = 0;

	for (int i = 0; i <  BLOCKS; i++) {
		sd[i] = 0;
		for (int j = 0; j < 32; j++) {
			parity ^= hvd->data[j + 32 * i];
			sd[i] |= (parity >> 31) & 0x1;
			sd[i] <<= (j == 31) ? 0 : 1;
			parity <<= 1;
		}
	}
	sd[BLOCKS] = parity;
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
static void HVD_calc_bd_parity(struct HVD *hvd, unsigned *bd)
{
	unsigned int parity = 0;

	for (int i = 0; i <  BLOCKS; i++) {
		bd[i] = 0;
		for (int j = 0; j < 32; j++) {
			parity ^= hvd->data[j + 32 * i];
			bd[i] |= parity & 0x1;
			bd[i] <<= (j == 31) ? 0 : 1;
			parity >>= 1;
		}
	}

	bd[BLOCKS] = 0;
	/* Revert last parity word */
	for (int i = 0; i < 30; i++) {
		bd[BLOCKS] |= parity & 0x1;
		parity >>= 1;
		bd[BLOCKS] <<= 1;
	}
	bd[BLOCKS] <<= 1;
}

void HVD_calc_parity(struct HVD *hvd)
{
	HVD_calc_h_parity(hvd, hvd->h);
	HVD_calc_v_parity(hvd, &hvd->v);
	HVD_calc_sd_parity(hvd, hvd->sd);
	HVD_calc_bd_parity(hvd, hvd->bd);
}

int HVD_check_integrity(struct HVD *hvd)
{
	unsigned sd[BLOCKS + 1];
	unsigned bd[BLOCKS + 1];
	unsigned h[BLOCKS];
	unsigned v;

	HVD_calc_h_parity(hvd, h);
	HVD_calc_v_parity(hvd, &v);
	HVD_calc_sd_parity(hvd, sd);
	HVD_calc_bd_parity(hvd, bd);

	if (hvd->v != v) return 0;
	for (int i = 0; i < BLOCKS; i++) {
		if (hvd->h[i] != h[i])
			return 0;
		if (hvd->sd[i] != sd[i])
			return 0;
		if (hvd->bd[i] != bd[i])
			return 0;
	}
	if (hvd->sd[BLOCKS] != sd[BLOCKS])
		return 0;
	if (hvd->bd[BLOCKS] != bd[BLOCKS])
		return 0;

	return 1;
}

int HVD_repair_data(struct HVD *hvd)
{
	int flips = 0;
	unsigned sd[BLOCKS + 1];
	unsigned bd[BLOCKS + 1];
	unsigned h[BLOCKS];
	unsigned v;

	HVD_calc_h_parity(hvd, h);
	HVD_calc_v_parity(hvd, &v);
	HVD_calc_sd_parity(hvd, sd);
	HVD_calc_bd_parity(hvd, bd);

	/* For every erroneous row... */
	for (int i = 0; i < ROWS; i++) {
	if (
		(hvd->h[i / 32] >> i % 32 & 0x1) !=
		(h[i / 32] >> i % 32 & 0x1)
	) {
		/* ...every erroneous column... */
		for (int j = 0; j < 32; j++) {
		if (
			((hvd->v >> (31 - j)) & 0x1) !=
			((v >> (31 - j)) & 0x1)
		) {
			int sd_b = (i + j) / 32;
			int sd_r = (i + j) % 32;
			int bd_b = (i + 31 - j) / 32;
			int bd_r = (i + 31 - j) % 32;
			/* ...check slash and backslash diagonals, if they
			 * are erroneous too */
			if (
				(
				((hvd->sd[sd_b] >> (31 - sd_r)) & 0x1) !=
				((sd[sd_b] >> (31 - sd_r)) & 0x1)
				) &
				(
				((hvd->bd[bd_b] >> (31 - bd_r)) & 0x1) !=
				((bd[bd_b] >> (31 - bd_r) & 0x1))
				)
			) {
				/* flip erroneous bit to correct */
				aprintf("col %d row %d mismatch\n", j, i);
				hvd->data[i] ^= 1 << (31 - j);
				flips++;
			}
		}
		}
	}
	}

	return flips;
}
