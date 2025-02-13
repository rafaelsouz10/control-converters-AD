#include <stdio.h>            
#include "pico/stdlib.h"
#include <inc/display_ssd1306.h>
#include <inc/joystick.h>

uint32_t last_print_time = 0; // Variável para controle de tempo do printf
bool led_state = false;

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
        uint16_t vrx_value = adc_read();
        // Lê o valor do eixo Y do joystick (pino ADC 0)
        adc_select_input(0);  
        uint16_t vry_value = adc_read();
        bool sw_value = gpio_get(SW_PIN) == 0; 

        // Controle do brilho dos LEDs baseado nos valores do joystick
        if(vrx_value <= ADC_CENTER) pwm_set_gpio_level(LED_PIN_RED, 0);  // Desliga o LED vermelho se abaixo do centro
        else pwm_set_gpio_level(LED_PIN_RED, vrx_value);                // Ajusta PWM do LED vermelho
        
        if (vry_value <= ADC_CENTER) pwm_set_gpio_level(LED_PIN_BLUE, 0);   // Desliga o LED azul se abaixo do centro
        else pwm_set_gpio_level(LED_PIN_BLUE, vry_value);                  // Ajusta PWM do LED azul

        if (sw_value) {
            led_state = !led_state; // Alterna o estado do LED verde
            gpio_put(LED_PIN_GREEN, led_state);

            // Altera o estágio do retângulo
            stage_retangulo = (stage_retangulo + 1) % 3;  // Cicla entre os três estágios (0, 1, 2)
            sleep_ms(200);  // Debounce do botão para evitar múltiplos cliques
        }

        // Mapeia os valores do joystick para as coordenadas do display (128x64 pixels)
        x_pos = (vrx_value * (128 - 8)) / 4095;              // Ajusta X para a largura do display (0 a 120)
        y_pos = (64 - 8) - (vry_value * (64 - 8)) / 4095;   // Ajusta Y para a altura do display (0 a 56), invertendo para corresponder ao display

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
