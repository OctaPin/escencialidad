#include <stdio.h>
#include "pico/stdlib.h"


const uint PIN_PWM1 = 8;
const uint PIN_PWM2 = 6;
const uint PIN_OUT1 = 4;
const uint PIN_OUT2 = 2;
const uint PIN_BUTTON1 = 5;
const uint PIN_BUTTON2 = 11;


int main() {
    stdio_init_all();

   
    gpio_init(PIN_PWM1);
    gpio_init(PIN_PWM2);
    gpio_init(PIN_OUT1);
    gpio_init(PIN_OUT2);

    
    gpio_init(PIN_BUTTON1);
    gpio_init(PIN_BUTTON2);
    gpio_set_dir(PIN_BUTTON1, GPIO_IN);
    gpio_set_dir(PIN_BUTTON2, GPIO_IN);

    
    uint sm = pio_claim_unused_sm();
    uint offset = pio_add_program(pio0, &pwm_program);

   
    pwm_config config = pwm_get_default_config();
    config_set_wrap(config, 65535);
    pwm_init(pio0, sm, offset, PIN_PWM1, config, 1000);

    
    bool state = true;
    uint16_t count = 1000; 

    while (true) {
     
        bool button1_pressed = gpio_get(PIN_BUTTON1);
        bool button2_pressed = gpio_get(PIN_BUTTON2);

      
        if (button1_pressed) {
            state = !state;
            gpio_put(PIN_OUT1, state);
            gpio_put(PIN_OUT2, !state);
            pwm_set_enabled(pio0, sm, state ? PIN_PWM1 : PIN_PWM2, false);
            pwm_set_enabled(pio0, sm, state ? PIN_PWM2 : PIN_PWM1, true);
        }

       
        if (button2_pressed) {
            count += 100; 
            if (count > 65535) count = 65535;
            pwm_set_wrap(pio0, sm, offset, count);
        }

       
    }
}