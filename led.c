#include "config.h"
#include "led.h"

#include <avr/io.h>

static void signal_bit_0() {
	asm volatile (
		"sbi %0, %1\n\t"
		"cbi %0, %1\n\t"
		"rjmp .+0\n\t"
		"rjmp .+0\n\t"
		"rjmp .+0\n\t"
		:
		: "I" (_SFR_IO_ADDR(PORTB)), "I" (LED_PIN)
	);
}

static void signal_bit_1() {
	asm volatile (
		"sbi %0, %1\n\t"
		"rjmp .+0\n\t"
		"nop\n\t"
		"cbi %0, %1\n\t"
		"rjmp .+0\n\t"
		"nop\n\t"
		:
		: "I" (_SFR_IO_ADDR(PORTB)), "I" (LED_PIN)
	);
}

static void signal_byte(unsigned char byte) {
	for (unsigned char mask = 0x80; mask != 0; mask >>= 1) {
		if (byte & mask)
			signal_bit_1();
		else
			signal_bit_0();
	}
}

static void signal_led(struct colour *c) {
	signal_byte(c->r);
	signal_byte(c->g);
	signal_byte(c->b);
}

void led_signal_sequence(int num_leds, struct colour *seq, int num_seq, int col_width, int seq_start) {
	for(int i = 0; i < num_leds; ++i) {
		int j, p = i - seq_start;

		if (p >= 0)
			j = (p / col_width % num_seq);
		else
			j = (num_seq - ((p + 1) / -col_width % num_seq) - 1);

		signal_led(&seq[j]);
	}
}
