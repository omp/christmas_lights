#ifndef CONFIG_H
#define CONFIG_H

#define F_CPU 8000000UL

#define LED_COUNT 50
#define LED_PIN PB3

#define EQ_PIN_INPUT PB2
#define EQ_PIN_RESET PB0
#define EQ_PIN_STROBE PB1
#define EQ_THRESHOLDS \
	{16, 82}, \
	{24, 130}, \
	{24, 99}, \
	{24, 79}, \
	{24, 77}, \
	{24, 87}, \
	{26, 37}

#define ISR_PIN PB4

#endif
