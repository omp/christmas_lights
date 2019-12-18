#include "config.h"
#include "eq.h"

#include <avr/io.h>
#include <util/delay.h>

unsigned char eq_levels[7];

static const unsigned char thresholds[][2] = { EQ_THRESHOLDS };

static void reset() {
	PORTB |= (1 << EQ_PIN_RESET);
	PORTB &= ~(1 << EQ_PIN_RESET);
	_delay_us(36);
}

static void strobe() {
	PORTB |= (1 << EQ_PIN_STROBE);
	_delay_us(36);
	PORTB &= ~(1 << EQ_PIN_STROBE);
	_delay_us(36);
}

static unsigned char map_level(unsigned char val, unsigned char min, unsigned char max) {
	if (val <= min)
		return 0;
	else if (val >= max)
		return 255;
	else
		return val * 255 / (max - min);
}

void eq_read() {
	reset();

	for (int i = 0; i < 7; ++i) {
		strobe();

		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC));
		eq_levels[i] = map_level(ADCH, thresholds[i][0], thresholds[i][1]);
	}
}

void eq_decay() {
	reset();

	for (int i = 0; i < 7; ++i) {
		strobe();
	}
}
