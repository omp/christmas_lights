#include "config.h"
#include "led.h"

#include <avr/io.h>

void signal_bit_0() {
	asm volatile (
		"sbi %0, %1\n\t"
		"cbi %0, %1\n\t"
		"rjmp .+0\n\t"
		"rjmp .+0\n\t"
		"rjmp .+0\n\t"
		:
		: "I" (_SFR_IO_ADDR(PORTB)), "I" (PIN_LED)
	);
}

void signal_bit_1() {
	asm volatile (
		"sbi %0, %1\n\t"
		"rjmp .+0\n\t"
		"nop\n\t"
		"cbi %0, %1\n\t"
		"rjmp .+0\n\t"
		"nop\n\t"
		:
		: "I" (_SFR_IO_ADDR(PORTB)), "I" (PIN_LED)
	);
}

void signal_byte(unsigned char byte) {
	for (unsigned char mask = 0x80; mask != 0; mask >>= 1) {
		if (byte & mask)
			signal_bit_1();
		else
			signal_bit_0();
	}
}

void signal_led(struct colour *c) {
	signal_byte(c->r);
	signal_byte(c->g);
	signal_byte(c->b);
}

void signal_led_sequence(int num_leds, struct colour *seq, int num_seq, int width, int start) {
	for(int i = 0; i < num_leds; ++i) {
		int j, p = i - start;

		if (p >= 0)
			j = (p / width % num_seq);
		else
			j = (num_seq - ((p + 1) / -width % num_seq) - 1);

		signal_led(&seq[j]);
	}
}
