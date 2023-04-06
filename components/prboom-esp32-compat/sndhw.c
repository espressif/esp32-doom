#include "sndhw.h"

typedef void (*snd_cb_t)(int16_t *buf, int len);

static snd_cb_t callback;

void sndhw_init(int rate, snd_cb_t cb) {
	callback=cb;
}
