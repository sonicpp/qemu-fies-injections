#ifndef HVD_H
#define HVD_H

#define BLOCKS	2
#define ROWS	BLOCKS * 32

struct HVD {
	int data[ROWS];
	unsigned sd[BLOCKS + 1];
	unsigned bd[BLOCKS + 1];
	unsigned h[BLOCKS];
	unsigned v;
};

int HVD_repair_data(struct HVD *hvd_t, struct HVD *hvd_r);
void HVD_calc_parity(struct HVD *hvd);
int HVD_check_parity(struct HVD *hvd_r, struct HVD *hvd_t);

#endif	// HVD_H

