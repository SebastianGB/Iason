#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "pwm.h"
#include "esc_pwm.h"

void esc_init(void)
{
     /* init timer2 with a period of 20ms */
//     pwm_init_timer(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN, LED_TIM, PWM_PRESCALE, PWM_PERIOD);
	pwm_init_timer(&RCC_APB1ENR, RCC_APB1ENR_TIM4EN, ESC_TIMER, PWM_PRESCALE, ESC_PWM_PERIOD);

     /* init output of channel2 of timer2 */
//     pwm_init_output_channel(LED_TIM, LED_CH1, &RCC_APB2ENR, RCC_APB2ENR_IOPAEN, GPIOA, GPIO_TIM2_CH2);
    pwm_init_output_channel(ESC_TIMER, ESC_CHANNEL1, &RCC_APB2ENR, RCC_APB2ENR_IOPBEN, GPIOB, GPIO_TIM4_CH2);


     pwm_set_pulse_width(ESC_TIMER, ESC_CHANNEL1, ESC_LOW);

     /* start timer1 */
//     pwm_start_timer(TIM2);
     pwm_start_timer(ESC_TIMER);

}

void esc_set_pulse_width(enum tim_oc_id ch, uint32_t pos_us)
{
//     pwm_set_pulse_width(TIM2, ch, pos_us);
	pwm_set_pulse_width(ESC_TIMER, ch, pos_us);
}

