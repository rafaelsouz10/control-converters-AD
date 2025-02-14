#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "hardware/adc.h" 
#include "hardware/pwm.h"

// Definição dos pinos dos potenciômetros do joystick
#define VRX_PIN_RED 26   // Pino de entrada analógica para eixo X do joystick
#define VRX_PIN_BLUE 27 // Pino de entrada analógica para eixo Y do joystick
#define SW_PIN 22      // Pino para o botão do joystick

// Definição dos pinos dos LEDs PWM
#define LED_PIN_RED 13  
#define LED_PIN_BLUE 12
#define LED_PIN_GREEN 11

#define ADC_CENTER 2150 //os valores da posição central ficou variando entre 2048 e 20140 em x

bool led_state = false;
bool sw_value;

// Função para inicializar um pino GPIO para saída PWM
uint pwm_init_gpio(uint gpio, uint wrap) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);  // Configura o pino como saída PWM

    uint slice_num = pwm_gpio_to_slice_num(gpio); // Obtém o slice PWM correspondente ao pino
    pwm_set_wrap(slice_num, wrap);  // Define o valor máximo do contador PWM
    
    pwm_set_enabled(slice_num, true); // Habilita o PWM no slice
    return slice_num;  
}

void set_init_joystick(){
    // Configura os pinos do joystick como entradas analógicas
    adc_gpio_init(VRX_PIN_RED);
    adc_gpio_init(VRX_PIN_BLUE);
    //Configura o botão do joystick como entrada
    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);
}

void pwm_gpio_init(){
    uint pwm_wrap = 4095;  // Define o valor máximo do PWM (12 bits - 0 a 4095)
    uint pwm_slice_red = pwm_init_gpio(LED_PIN_RED, pwm_wrap);   // Inicializa o PWM para o LED vermelho
    uint pwm_slice_blue = pwm_init_gpio(LED_PIN_BLUE, pwm_wrap); // Inicializa o PWM para o LED azul
}

void control_joystick_leds(){

    //botão A ativa ou desativa os leds pwm
    if (estado_pwm_leds) {
        // Controle do brilho dos LEDs baseado nos valores do joystick
        if(vrx_value <= ADC_CENTER) pwm_set_gpio_level(LED_PIN_RED, 0);  // Desliga o LED vermelho se abaixo do centro
        else pwm_set_gpio_level(LED_PIN_RED, vrx_value);                // Ajusta PWM do LED vermelho
        
        if (vry_value <= ADC_CENTER) pwm_set_gpio_level(LED_PIN_BLUE, 0);   // Desliga o LED azul se abaixo do centro
        else pwm_set_gpio_level(LED_PIN_BLUE, vry_value);                  // Ajusta PWM do LED azul
    } else {
        pwm_set_gpio_level(LED_PIN_RED, 0);  // Desliga o LED vermelho se abaixo do centro
        pwm_set_gpio_level(LED_PIN_BLUE, 0);  // Desliga o LED vermelho se abaixo do centro
    }
    

    if (sw_value) {
        led_state = !led_state; // Alterna o estado do LED verde com o botão do joystick
        gpio_put(LED_PIN_GREEN, led_state);

        // Altera o estágio do retângulo
        stage_retangulo = (stage_retangulo + 1) % 4;  // Cicla entre os estágios (0, 1, 2, 3)
        sleep_ms(200);  // Debounce do botão para evitar múltiplos cliques
    }
}

#endif