#ifndef __SERVO_H_
#define __LED_DIM_H_

#include <libopencm3/stm32/timer.h>

/**
 * Prescale 24000000 Hz system clock by 24 = 1000000 Hz.
 */
#define PWM_PRESCALE	(24) //as in "Discovering STM32...", values 0..23 are 24 values

/**
 * We need a 50 Hz period (1000 / 20ms = 50), thus devide 100000 by 50 = 20000 (us).
 */
#define ESC_PWM_PERIOD		(10000)

#define ESC_MAX		(19999)
#define ESC_MID		(10000)
#define ESC_LOW		(1000)

#define ESC_TIMER		TIM4


/**
 * Timer channel for ESC.
 *
 * Changing this also requires to change settings in!
 */
#define ESC_CHANNEL1		TIM_OC1//TIM_OC1//TIM_OC2
#define ESC_CHANNEL2		TIM_OC2//TIM_OC1//TIM_OC2

/**
 * Initialize and start the PWM used for the ESCs
 */
void esc_init(void);

/**
 * Drive the ESC connected to the given channel to the given position in us.
 *
 * @param[in]	ch		The channel of the servo. E.g. SERVO_CH1, SERVO_CH2.
 * @param[in]	pos_us	The position in us to which to drive the servo to.
 */
void esc_set_pulse_width(enum tim_oc_id ch, uint32_t pos_us);

#endif
