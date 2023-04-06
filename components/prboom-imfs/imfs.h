#include <stdint.h>

typedef struct {
	const char *name;
	const uint8_t *data;
	const uint8_t *end;
} imf_files_t;

extern const imf_files_t g_imf_files[];
