#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "config.h"
#include "led.h"

static struct colour seq[6];

int main() {
	DDRB |= (1 << PIN_LED);
	PORTB &= ~(1 << PIN_LED);

	seq[0].r = 255;
	seq[1].r = 255;
	seq[1].g = 127;
	seq[2].r = 255;
	seq[2].g = 255;
	seq[3].g = 255;
	seq[4].b = 255;
	seq[5].r = 127;
	seq[5].b = 255;

	while (1) {
		for (int i = 0; i < NUM_LEDS; ++i) {
			signal_led_sequence(NUM_LEDS, seq, 6, 4, i);
			_delay_ms(100);
		}
	}

	return 0;
}
