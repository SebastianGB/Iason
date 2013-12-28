#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "esc_pwm.h"

// +++++++++++++++++++++++ USART ++++++++++++++++++++++++++


void clock_setup(void)
{
//	rcc_clock_setup_in_hse_8mhz_out_24mhz();

	/* Enable GPIOC clock. */
//	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);

	/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_USART1EN);
}

void usart_setup(void)
{
	/* Setup GPIO pin GPIO_USART1_TX/GPIO9 on GPIO port A for transmit. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	/* Setup GPIO pin for transmit*/
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
		      GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

	/* Setup UART parameters. */
	// usart_set_baudrate(USART1, 38400);
	/* TODO usart_set_baudrate() doesn't support 24MHz clock (yet). */
	/* This is the equivalent: */
	USART_BRR(USART1) = (uint16_t)((24000000 << 4) / (38400 * 16));

	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART1);
}

void gpio_setup(void)
{
	/* Set GPIO9 (in GPIO port C) to 'output push-pull'. [LED] */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO9);
}


// +++++++++++++++++++++++++ PWM +++++++++++++++++++++++++++++


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

#define PWM_WIDTH_MULTIPLICATOR	100

int main(void) {

	// ++++++++++++++++++++++++ pwm +++++++++++++++++++++

	clock_init();
	gpio_init();
	esc_init();

	// +++++++++++++++++++++++++ usart +++++++++++++++++

	uint8_t j = 0;

	clock_setup();
	gpio_setup();
	usart_setup();
	uint16_t rec = 0;
	uint16_t old_byte_received = 0;

	// +++++++++++++++++++++++++ usart end ++++++++++++++++


	gpio_set(GPIOC, GPIO8);

	int currentPulseWidthMotor1 = 0;
	int currentPulseWidthMotor2 = 0;


	while (1) {

		//PWM
		esc_set_pulse_width(ESC_CHANNEL1, currentPulseWidthMotor1);
		esc_set_pulse_width(ESC_CHANNEL2, currentPulseWidthMotor2);

		gpio_toggle(GPIOC, GPIO8);

		//USART
		//can only read 1 byte at the same time!!!
		rec = usart_recv(USART1);
		if(rec != old_byte_received) {
			usart_send_blocking(USART1, rec); /* USART1: Send byte. */
		}
		if(rec == 200){
			gpio_toggle(GPIOC, GPIO9);	/* LED on/off */
		}

		//filter out only the 8 lowest bits
		uint16_t tmp = rec & 0x00FF;
		if((tmp >> 7) == 0){
			//filter out first bit
			currentPulseWidthMotor1 = (rec&0x7F) * PWM_WIDTH_MULTIPLICATOR;
		} else {
			currentPulseWidthMotor2 = (rec&0x7F) * PWM_WIDTH_MULTIPLICATOR;
		}
//		currentPulseWidthMotor1 = rec * 10;
		//can only send 1 byte at the same time!!!
		if ((j++ % 80) == 0) {		/* Newline after line full. */
			usart_send_blocking(USART1, '\r');
			usart_send_blocking(USART1, '\n');
		}

		delay(100000);

		old_byte_received = rec;
	}

	return 0;
}
