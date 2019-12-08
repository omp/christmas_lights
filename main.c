#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#define PIN_LED PB0
#define NUM_LEDS 50

struct led {
	unsigned char r, g, b;
};

void signal_high() {
	asm volatile (
		"sbi %0, %1\n\t"
		"rjmp .+0\n\t"
		"nop\n\t"
		"cbi %0, %1\n\t"
		"rjmp .+0\n\t"
		"rjmp .+0\n\t"
		:
		: "I" (_SFR_IO_ADDR(PORTB)), "I" (PIN_LED)
	);
}

void signal_low() {
	asm volatile (
		"sbi %0, %1\n\t"
		"cbi %0, %1\n\t"
		"rjmp .+0\n\t"
		"rjmp .+0\n\t"
		"rjmp .+0\n\t"
		"nop\n\t"
		:
		: "I" (_SFR_IO_ADDR(PORTB)), "I" (PIN_LED)
	);
}

void signal_byte(unsigned char byte) {
	for (unsigned char mask = 0x80; mask != 0; mask >>= 1) {
		if (byte & mask)
			signal_high();
		else
			signal_low();
	}
}

void signal_led(struct led *led) {
	signal_byte(led->r);
	signal_byte(led->g);
	signal_byte(led->b);
}

void signal_led_sequence(int num_leds, struct led **seq, int num_seq) {
	for(int i = 0; i < num_leds; ++i) {
		int j = i % num_seq;
		signal_led(seq[j]);
	}
}

int main() {
	DDRB |= (1 << PIN_LED);
	PORTB &= ~(1 << PIN_LED);

	struct led **seq = calloc(3, sizeof(*seq)); 

	seq[0] = calloc(1, sizeof(struct led));
	seq[1] = calloc(1, sizeof(struct led));
	seq[2] = calloc(1, sizeof(struct led));

	while (1) {
		seq[0]->g = 0;
		seq[1]->b = 0;
		seq[2]->r = 0;
		seq[0]->r = 255;
		seq[1]->g = 255;
		seq[2]->b = 255;

		signal_led_sequence(NUM_LEDS, seq, 3);
		_delay_ms(250);

		seq[0]->r = 0;
		seq[1]->g = 0;
		seq[2]->b = 0;
		seq[0]->b = 255;
		seq[1]->r = 255;
		seq[2]->g = 255;

		signal_led_sequence(NUM_LEDS, seq, 3);
		_delay_ms(250);

		seq[0]->b = 0;
		seq[1]->r = 0;
		seq[2]->g = 0;
		seq[0]->g = 255;
		seq[1]->b = 255;
		seq[2]->r = 255;

		signal_led_sequence(NUM_LEDS, seq, 3);
		_delay_ms(250);
	}

	return 0;
}
