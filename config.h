#ifndef CONFIG_H
#define CONFIG_H

#define F_CPU 8000000UL

#define LED_COUNT 100
#define LED_PIN PB3

#define EQ_PIN_INPUT PB2
#define EQ_PIN_RESET PB0
#define EQ_PIN_STROBE PB1
#define EQ_THRESHOLDS \
	{0x76, 0xFF}, \
	{0x82, 0xFF}, \
	{0x58, 0xFF}, \
	{0x5F, 0xFF}, \
	{0x40, 0xAA}, \
	{0x3D, 0x88}, \
	{0x1F, 0x66}

#define ISR_PIN PB4

#endif
