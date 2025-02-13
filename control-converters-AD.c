#include <stdio.h>            
#include "pico/stdlib.h"      
#include "hardware/adc.h"     
#include "hardware/pwm.h"
#include <inc/display_ssd1306.h>   

// Definição dos pinos dos potenciômetros do joystick
#define VRX_PIN_RED 26  // Pino de entrada analógica para eixo X do joystick
#define VRX_PIN_BLUE 27 // Pino de entrada analógica para eixo Y do joystick

// Definição dos pinos dos LEDs PWM
#define LED_PIN_RED 13  
#define LED_PIN_BLUE 12

#define ADC_CENTER 2150 //os valores da posição central ficou variando entre 2048 e 20140 em x

// Variáveis para armazenar os valores do ciclo de trabalho dos LEDs
float duty_cycle_red;
float duty_cycle_blue;

// Função para inicializar um pino GPIO para saída PWM
uint pwm_init_gpio(uint gpio, uint wrap) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);  // Configura o pino como saída PWM

    uint slice_num = pwm_gpio_to_slice_num(gpio); // Obtém o slice PWM correspondente ao pino
    pwm_set_wrap(slice_num, wrap);  // Define o valor máximo do contador PWM
    
    pwm_set_enabled(slice_num, true); // Habilita o PWM no slice
    return slice_num;  
}

int main() {   
    stdio_init_all(); // Inicializa entrada e saída padrão
    adc_init();  // Inicializa o ADC (conversor analógico-digital)
    displayInit(); // Inicializa as configurações do display OLED SSD1306

    // Configura os pinos do joystick como entradas analógicas
    adc_gpio_init(VRX_PIN_RED);
    adc_gpio_init(VRX_PIN_BLUE);

    uint pwm_wrap = 4095;  // Define o valor máximo do PWM (12 bits - 0 a 4095)
    uint pwm_slice_red = pwm_init_gpio(LED_PIN_RED, pwm_wrap);   // Inicializa o PWM para o LED vermelho
    uint pwm_slice_blue = pwm_init_gpio(LED_PIN_BLUE, pwm_wrap); // Inicializa o PWM para o LED azul
    
    uint32_t last_print_time = 0; // Variável para controle de tempo do printf

    bool cor = true; // Define a cor do display (preto/branco)

    while (true) {
        // Lê o valor do eixo X do joystick (pino ADC 1)
        adc_select_input(1);  
        uint16_t vrx_value = adc_read();
        // Lê o valor do eixo Y do joystick (pino ADC 0)
        adc_select_input(0);  
        uint16_t vry_value = adc_read();

        // Controle do brilho dos LEDs baseado nos valores do joystick
        if(vrx_value <= ADC_CENTER) pwm_set_gpio_level(LED_PIN_RED, 0);  // Desliga o LED vermelho se abaixo do centro
        else pwm_set_gpio_level(LED_PIN_RED, vrx_value);                // Ajusta PWM do LED vermelho
        
        if (vry_value <= ADC_CENTER) pwm_set_gpio_level(LED_PIN_BLUE, 0);   // Desliga o LED azul se abaixo do centro
        else pwm_set_gpio_level(LED_PIN_BLUE, vry_value);                  // Ajusta PWM do LED azul

        // Calcula os ciclos de trabalho (duty cycle) dos LEDs em percentual
        duty_cycle_red = (vrx_value / 4095.0) * 100;
        duty_cycle_blue = (vry_value / 4095.0) * 100;

        // Mapeia os valores do joystick para as coordenadas do display (128x64 pixels)
        int x_pos = (vrx_value * (128 - 8)) / 4095;              // Ajusta X para a largura do display (0 a 120)
        int y_pos = (64 - 8) - (vry_value * (64 - 8)) / 4095;   // Ajusta Y para a altura do display (0 a 56), invertendo para corresponder ao display

        ssd1306_fill(&ssd, !cor); // Limpa o display
        ssd1306_rect(&ssd, y_pos,  x_pos, 8, 8, cor, cor); // Desenha o quadrado no display na posição calculada
        // ssd1306_draw_string(&ssd, "EMBARCATECH", 20, 10); // Desenha uma string
        ssd1306_send_data(&ssd); // Atualiza o display
        
        // Exibe valores no console a cada 1s
        uint32_t current_time = to_ms_since_boot(get_absolute_time());  
        if (current_time - last_print_time >= 1000) {  
            printf("VRX: %u, DC RED: %.2f%%, x_pos: %d\n", vrx_value, duty_cycle_red, x_pos);
            printf("VRY: %u, DC BLUE: %.2f%%, y_pos: %d\n\n", vry_value, duty_cycle_blue, y_pos);
            last_print_time = current_time;  
        }
        sleep_ms(100);  
    }
    return 0;  
}
