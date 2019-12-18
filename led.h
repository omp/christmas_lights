#ifndef LED_H
#define LED_H

struct colour {
	unsigned char r, g, b;
};

void led_signal_sequence(int, struct colour *, int, int, int);

#endif
