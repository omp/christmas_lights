#include "config.h"
#include "eq.h"
#include "led.h"

#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>

static struct colour seq[6];
static bool interrupted = false;

void red_green() {
	memset(seq, 0, 2 * sizeof(seq[0]));

	seq[0].r = 255;
	seq[1].g = 255;

	while (1) {
		for (int i = 0; i < LED_COUNT; ++i) {
			if (interrupted == true) {
				interrupted = false;
				return;
			}

			led_signal_sequence(LED_COUNT, seq, 2, 1, i);
			_delay_ms(200);
		}
	}
}

void rainbow() {
	memset(seq, 0, 6 * sizeof(seq[0]));

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
		for (int i = 0; i < LED_COUNT; ++i) {
			if (interrupted == true) {
				interrupted = false;
				return;
			}

			led_signal_sequence(LED_COUNT, seq, 6, 4, i);
			_delay_ms(100);
		}
	}
}

void sound_reactive() {
	memset(seq, 0, 3 * sizeof(seq[0]));

	int offset = 0;

	while (1) {
		if (interrupted == true) {
			interrupted = false;
			return;
		}

		eq_read();

		seq[0].r = (eq_levels[0] > eq_levels[1]) ? eq_levels[0] : eq_levels[1];
		unsigned char t = (eq_levels[2] > eq_levels[3]) ? eq_levels[2] : eq_levels[3];
		t = (t > eq_levels[4]) ? t : eq_levels[4];
		seq[1].r = t;
		seq[1].g = t * 170 / 255;
		seq[1].b = t * 65 / 255;
		seq[2].g = (eq_levels[5] > eq_levels[6]) ? eq_levels[5] : eq_levels[6];

		led_signal_sequence(LED_COUNT, seq, 3, 1, offset/60);
		if (++offset == 180)
			offset = 0;
		eq_decay(10);
		_delay_ms(50);
	}
}

int main() {
	DDRB |= (1 << LED_PIN);
	PORTB &= ~(1 << LED_PIN);

	DDRB |= (1 << EQ_PIN_RESET);
	PORTB &= ~(1 << EQ_PIN_RESET);
	DDRB |= (1 << EQ_PIN_STROBE);
	PORTB |= (1 << EQ_PIN_STROBE);
	DDRB &= ~(1 << EQ_PIN_INPUT);

	DDRB &= ~(1 << ISR_PIN);
	GIMSK |= (1 << PCIE);
	PCMSK |= (1 << PCINT4);
	sei();

	ADMUX |= (1 << MUX0) | (1 << ADLAR);
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);

	while (1) {
		sound_reactive();
		red_green();
		rainbow();
	}

	return 0;
}

ISR(PCINT0_vect) {
	if (!(PINB & (1 << ISR_PIN)))
		interrupted = true;
}
