#include <stdio.h>            
#include "pico/stdlib.h"
#include <inc/display_ssd1306.h>
#include <inc/joystick.h>

uint32_t last_print_time = 0; // Variável para controle de tempo do printf

int main() {
    stdio_init_all();       // Inicializa entrada e saída padrão
    adc_init();            // Inicializa o ADC (conversor analógico-digital)
    displayInit();        // Inicializa as configurações do display OLED SSD1306
    set_init_joystick(); //Inicializa os pinos do joystick

    //Configura o led verde como saída
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);

    pwm_gpio_init(); //configuração inicial dos pin pwm

    while (true) {
        // Lê o valor do eixo X do joystick (pino ADC 1)
        adc_select_input(1);  
        vrx_value = adc_read();
        // Lê o valor do eixo Y do joystick (pino ADC 0)
        adc_select_input(0);  
        vry_value = adc_read();
        // Lê o estado do botão do joystick (true ou false)
        sw_value = gpio_get(SW_PIN) == 0; 

        //Função responsável por configuração do joystick controlar os leds
        control_joystick_leds();

        //Função responsável pelas informações do display
        print_display();
        
        // Exibe valores no console a cada 1s
        uint32_t current_time = to_ms_since_boot(get_absolute_time());  
        if (current_time - last_print_time >= 1000) {
            printf("VRX: %u, x_pos: %d\n", vrx_value, x_pos);
            printf("VRY: %u, y_pos: %d\n", vry_value, y_pos);
            printf("SW: %d, GREEN: %s\n\n", sw_value, led_state ? "ON" : "OFF");
            last_print_time = current_time;
        }
        sleep_ms(100);  
    }
    return 0;
}
