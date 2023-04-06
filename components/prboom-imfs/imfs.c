#include <stddef.h>
#include "imfs.h"

extern const uint8_t d_e1m1_imf_start[] asm("_binary_d_e1m1_imf_start");
extern const uint8_t d_e1m2_imf_start[] asm("_binary_d_e1m2_imf_start");
extern const uint8_t d_e1m3_imf_start[] asm("_binary_d_e1m3_imf_start");
extern const uint8_t d_e1m4_imf_start[] asm("_binary_d_e1m4_imf_start");
extern const uint8_t d_e1m5_imf_start[] asm("_binary_d_e1m5_imf_start");
extern const uint8_t d_e1m6_imf_start[] asm("_binary_d_e1m6_imf_start");
extern const uint8_t d_e1m7_imf_start[] asm("_binary_d_e1m7_imf_start");
extern const uint8_t d_e1m8_imf_start[] asm("_binary_d_e1m8_imf_start");
extern const uint8_t d_e1m9_imf_start[] asm("_binary_d_e1m9_imf_start");
extern const uint8_t d_inter_imf_start[] asm("_binary_d_inter_imf_start");
extern const uint8_t d_introa_imf_start[] asm("_binary_d_introa_imf_start");
extern const uint8_t d_intro_imf_start[] asm("_binary_d_intro_imf_start");
extern const uint8_t d_victor_imf_start[] asm("_binary_d_victor_imf_start");


extern const uint8_t d_e1m1_imf_end[] asm("_binary_d_e1m1_imf_end");
extern const uint8_t d_e1m2_imf_end[] asm("_binary_d_e1m2_imf_end");
extern const uint8_t d_e1m3_imf_end[] asm("_binary_d_e1m3_imf_end");
extern const uint8_t d_e1m4_imf_end[] asm("_binary_d_e1m4_imf_end");
extern const uint8_t d_e1m5_imf_end[] asm("_binary_d_e1m5_imf_end");
extern const uint8_t d_e1m6_imf_end[] asm("_binary_d_e1m6_imf_end");
extern const uint8_t d_e1m7_imf_end[] asm("_binary_d_e1m7_imf_end");
extern const uint8_t d_e1m8_imf_end[] asm("_binary_d_e1m8_imf_end");
extern const uint8_t d_e1m9_imf_end[] asm("_binary_d_e1m9_imf_end");
extern const uint8_t d_inter_imf_end[] asm("_binary_d_inter_imf_end");
extern const uint8_t d_introa_imf_end[] asm("_binary_d_introa_imf_end");
extern const uint8_t d_intro_imf_end[] asm("_binary_d_intro_imf_end");
extern const uint8_t d_victor_imf_end[] asm("_binary_d_victor_imf_end");


const imf_files_t g_imf_files[]={
	{"e1m1", d_e1m1_imf_start, d_e1m1_imf_end},
	{"e1m2", d_e1m2_imf_start, d_e1m2_imf_end},
	{"e1m3", d_e1m3_imf_start, d_e1m3_imf_end},
	{"e1m4", d_e1m4_imf_start, d_e1m4_imf_end},
	{"e1m5", d_e1m5_imf_start, d_e1m5_imf_end},
	{"e1m6", d_e1m6_imf_start, d_e1m6_imf_end},
	{"e1m7", d_e1m7_imf_start, d_e1m7_imf_end},
	{"e1m8", d_e1m8_imf_start, d_e1m8_imf_end},
	{"e1m9", d_e1m9_imf_start, d_e1m9_imf_end},
	{"inter", d_inter_imf_start, d_inter_imf_end},
	{"introa", d_introa_imf_start, d_introa_imf_end},
	{"intro", d_intro_imf_start, d_intro_imf_end},
	{"victor", d_victor_imf_start, d_victor_imf_end},
	{NULL, NULL, NULL}
};
