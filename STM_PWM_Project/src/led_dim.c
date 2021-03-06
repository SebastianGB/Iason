#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "pwm.h"
#include "led_dim.h"

void led_dim_init(void)
{
     /* init timer2 with a period of 20ms */
//     pwm_init_timer(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN, LED_TIM, PWM_PRESCALE, PWM_PERIOD);
	pwm_init_timer(&RCC_APB1ENR, RCC_APB1ENR_TIM4EN, LED_TIM, PWM_PRESCALE, PWM_PERIOD);

     /* init output of channel2 of timer2 */
//     pwm_init_output_channel(LED_TIM, LED_CH1, &RCC_APB2ENR, RCC_APB2ENR_IOPAEN, GPIOA, GPIO_TIM2_CH2);
    pwm_init_output_channel(LED_TIM, LED_CH1, &RCC_APB2ENR, RCC_APB2ENR_IOPBEN, GPIOB, GPIO_TIM4_CH2);


     pwm_set_pulse_width(LED_TIM, LED_CH1, LED_OFF);

     /* start timer1 */
//     pwm_start_timer(TIM2);
     pwm_start_timer(LED_TIM);

}

void led_set_dim_value(enum tim_oc_id ch, uint32_t pos_us)
{
//     pwm_set_pulse_width(TIM2, ch, pos_us);
	pwm_set_pulse_width(LED_TIM, ch, pos_us);
}

