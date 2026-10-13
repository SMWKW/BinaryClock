#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "GPIO/config_GPIO.h"
#include "GPIO/GPIO_init.h"

void GPIO_init(void)
{
    // Route both pins to PWM; GP2 and GP3 use separate channels of the same slice.
    gpio_set_function(Sec_Bit0, GPIO_FUNC_PWM);
    gpio_set_function(Sec_Bit1, GPIO_FUNC_PWM);

    // Start from the SDK defaults, then configure one shared PWM period and clock.
    pwm_config config = pwm_get_default_config();
    
    // PWM frequency = f_sys / (clkdiv * (wrap + 1)).
    // With f_sys = 150 MHz, clkdiv = 40, and wrap = 99, this gives 37.5 kHz.

    pwm_config_set_wrap(&config, LED_PWM_WRAP); // Count from 0 through 99, giving 100 PWM steps per period.
    
    pwm_config_set_clkdiv(&config, 40.0f);

    // Both pins share this slice; initialize it once and enable it immediately.
    pwm_init(pwm_gpio_to_slice_num(Sec_Bit0), &config, true);
}