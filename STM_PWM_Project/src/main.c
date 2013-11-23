#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>

#include "led_dim.h"

/**
 * Setup the system clock to 72MHz; for the stm32-discovery its 24MHz
 */
void clock_init(void) {
	//Warning: f_max = 24MHz!!!
//     rcc_clock_setup_in_hse_8mhz_out_72mhz();
	rcc_clock_setup_in_hse_8mhz_out_24mhz();
}

/**
 * Initialize the GPIO port for the LED at pin 12 on port C.
 */
void gpio_init(void) {
	/* enable GPIOC clock */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);

	/*
	 * set GPIO12 at PORTC (led) to 'output alternate function push-pull'.
	 */
	//too fast for stm32-discovery - use only 2MHz here
	//blue led ist not GPIO12 on the stm32-discovery but GPIO8
//     gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);

	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8);

}

/**
 * Delay by executing some "nop"s.
 *
 * @param[in]	d	number of "nop"s to performe.
 */
void delay(int d) {

	int i;

	for (i = 0; i < d; i++) {
		__asm("nop");
	}
}

int main(void) {
	clock_init();
	gpio_init();
	led_dim_init();

	//again GPIO9 is the pin where the LED is connected
//     gpio_set(GPIOC, GPIO12);
	gpio_set(GPIOC, GPIO8);

	delay(800000);

	// let pan-til "look around a little"
	while (1) {

		led_set_dim_value(LED_CH1, LED_ON);
		delay(800000);

		led_set_dim_value(LED_CH1, LED_DIM);
		delay(800000);

		led_set_dim_value(LED_CH1, LED_OFF);
		delay(800000);
		delay(800000);
		delay(800000);

		led_set_dim_value(LED_CH1, LED_DIM);
		delay(800000);
		gpio_toggle(GPIOC, GPIO8);
	}

	return 0;
}
