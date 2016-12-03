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

void HVD_calc_parity(struct HVD *hvd);
int HVD_check_integrity(struct HVD *hvd);
int HVD_repair_data(struct HVD *hvd);

#endif	// HVD_H

