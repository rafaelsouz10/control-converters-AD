#include <stdio.h>            
#include "pico/stdlib.h"      
#include "hardware/adc.h"     
#include "hardware/pwm.h"
#include <inc/display_ssd1306.h>   

#define VRX_PIN_RED 26
#define VRX_PIN_BLUE 27
#define LED_PIN_RED 13  
#define LED_PIN_BLUE 12

#define ADC_CENTER 2150

float duty_cycle_red;
float duty_cycle_blue;

uint pwm_init_gpio(uint gpio, uint wrap) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, wrap);
    
    pwm_set_enabled(slice_num, true);  
    return slice_num;  
}

int main() {   
    stdio_init_all();
    adc_init(); 
    displayInit(); //Inicializa as configurações do display

    adc_gpio_init(VRX_PIN_RED);
    adc_gpio_init(VRX_PIN_BLUE);

    uint pwm_wrap = 4095;  
    uint pwm_slice_red = pwm_init_gpio(LED_PIN_RED, pwm_wrap);
    uint pwm_slice_blue = pwm_init_gpio(LED_PIN_BLUE, pwm_wrap);
    
    
    uint32_t last_print_time = 0;

    while (true) {
        adc_select_input(1);  
        uint16_t vrx_value = adc_read();
        adc_select_input(0);  
        uint16_t vry_value = adc_read();

        if(vrx_value <= ADC_CENTER) pwm_set_gpio_level(LED_PIN_RED, 0);
        else pwm_set_gpio_level(LED_PIN_RED, vrx_value);
        
        if (vry_value <= ADC_CENTER) pwm_set_gpio_level(LED_PIN_BLUE, 0);
        else pwm_set_gpio_level(LED_PIN_BLUE, vry_value);

        duty_cycle_red = (vrx_value / 4095.0) * 100;
        duty_cycle_blue = (vry_value / 4095.0) * 100;
          

        uint32_t current_time = to_ms_since_boot(get_absolute_time());  
        if (current_time - last_print_time >= 1000) {  
            printf("VRX: %u, Duty Cycle RED: %.2f%%\n", vrx_value, duty_cycle_red);
            printf("VRY: %u, Duty Cycle BLUE: %.2f%%\n\n", vry_value, duty_cycle_blue);
            last_print_time = current_time;  
        }
        sleep_ms(100);  
    }
    return 0;  
}
