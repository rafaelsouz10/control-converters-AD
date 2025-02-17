#ifndef CONFIG_BTN_H
#define CONFIG_BTN_H

#include "hardware/timer.h"

//definição de GPIO botões
#define BTN_A 5

// Variáveis globais 
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
bool estado_pwm_leds = true;

// Configuração inicial ds botões
void setup_gpio_BTN_A() {
  //Botão A
  gpio_init(BTN_A);
  gpio_set_dir(BTN_A, GPIO_IN);
  gpio_pull_up(BTN_A);
}

// Função de interrupção com debouncing do botão
void gpio_irq_handler(uint gpio, uint32_t events){

  // Obtém o tempo atual em microssegundos
  uint32_t current_time = to_us_since_boot(get_absolute_time());
  
  // Verifica se passou tempo suficiente desde o último evento
  if (current_time - last_time > 200000) { // 200 ms de debouncing
    last_time = current_time; // Atualiza o tempo do último evento
      estado_pwm_leds = !estado_pwm_leds; //Alterna o estado dos leds pwm quando o botão é clicado
  }
}

#endif