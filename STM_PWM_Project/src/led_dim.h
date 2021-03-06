#ifndef __SERVO_H_
#define __LED_DIM_H_

#include <libopencm3/stm32/timer.h>

/**
 * Prescale 24000000 Hz system clock by 24 = 1000000 Hz.
 */
#define PWM_PRESCALE	(23) //as in "Discovering STM32...", values 0..23 are 24 values

/**
 * We need a 50 Hz period (1000 / 20ms = 50), thus devide 100000 by 50 = 20000 (us).
 */
#define PWM_PERIOD		(20000)

#define LED_ON		(19999)

#define LED_DIM		(10000)

#define LED_OFF		(1000)

/**
 * Timer to use for the servos.
 */
#define LED_TIM		TIM4

/**
 * TIM2 channel for servo 1.
 *
 * Changing this also requires to change settings in {@link servo_init}!
 */
#define LED_CH1		TIM_OC2

/**
 * Initialize and start the PWM used for the servos, drive servos to middle position.
 */
void led_dim_init(void);

/**
 * Drive the servo connected to the given channel to the given position in us.
 *
 * @param[in]	ch		The channel of the servo. E.g. SERVO_CH1, SERVO_CH2.
 * @param[in]	pos_us	The position in us to which to drive the servo to.
 */
void led_set_dim_value(enum tim_oc_id ch, uint32_t pos_us);

#endif
