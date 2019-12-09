struct colour {
	unsigned char r, g, b;
};

void signal_bit_0();
void signal_bit_1();
void signal_byte(unsigned char);
void signal_led(struct colour *);
void signal_led_sequence(int, struct colour *, int, int, int);
