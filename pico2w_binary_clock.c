#include <stdio.h>
#include <stdint.h>
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "GPIO/config_GPIO.h"
#include "GPIO/GPIO_init.h"
#include "WiFi/WLAN_init.h"


int main()
{
    // Initialize all standard I/O
    stdio_init_all();

    // Initialize GPIO
    GPIO_init();

    // Initialize Wi-Fi
    // if (WLAN_init() != 0) {
    //     return -1;
    // }

    // Initial state of the GPIO pin
    bool state = false;  
    // With LED_PWM_WRAP 99, pwm_level 10 corresponds to about 10% duty cycle.
     uint16_t pwm_level = 10u;

    printf("Main loop started.\n");
    // Main loop
    while (true) {
        printf("Main loop running ...\n");
        state = !state;
        pwm_level = pwm_level;
        pwm_set_gpio_level(Sec_Bit0, state ? pwm_level : 0u);
        pwm_set_gpio_level(Sec_Bit1, state ? 0u : pwm_level);
        sleep_ms(1000);
    }
}
