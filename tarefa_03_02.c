#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "matriz_leds.h"

// Definição dos pinos
#define led_green_pin 11
#define led_blue_pin 12
#define btnA_pin 5
#define btnB_pin 6

// Variáveis para debounce
static volatile uint32_t last_time_a = 0;
static volatile uint32_t last_time_b = 0;

volatile bool led_green_state = false;
volatile bool led_blue_state = false;

// Protótipos das funções
void gpio_callback_btn(uint gpio, uint32_t events);

int main() {
    stdio_init_all();

    // Inicializações
    gpio_init(led_green_pin);
    gpio_init(led_blue_pin);
    gpio_init(btnA_pin);
    gpio_init(btnB_pin);

    gpio_set_dir(led_green_pin, GPIO_OUT);
    gpio_set_dir(led_blue_pin, GPIO_OUT);
    gpio_set_dir(btnA_pin, GPIO_IN);
    gpio_set_dir(btnB_pin, GPIO_IN);

    gpio_pull_up(btnA_pin);  // Botão em pull-up
    gpio_pull_up(btnB_pin);

    // Configuração de interrupções
    gpio_set_irq_enabled_with_callback(btnA_pin, GPIO_IRQ_EDGE_FALL, true, &gpio_callback_btn);
    gpio_set_irq_enabled_with_callback(btnB_pin, GPIO_IRQ_EDGE_FALL, true, &gpio_callback_btn);

    while (true) {
        sleep_ms(1000);
    }
}

// Callback para o botão A
void gpio_callback_btn(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (gpio == btnA_pin && (current_time - last_time_a) > 200) {  // Debounce 200 ms
        last_time_a = current_time;
        led_green_state = !led_green_state;

        // Atualiza o LED Verde
        gpio_put(led_green_pin, led_green_state);

        // Mensagem no Serial Monitor
        printf("LED VERDE: %s\n", led_green_state ? "Ligado" : "Desligado");
    }
    else if (gpio == btnB_pin && (current_time - last_time_b) > 200) {  // Debounce 200 ms
        last_time_b = current_time;
        led_blue_state = !led_blue_state;

        // Atualiza o LED Azul
        gpio_put(led_blue_pin, led_blue_state);

        // Mensagem no Serial Monitor
        printf("LED AZUL: %s\n", led_blue_state ? "Ligado" : "Desligado");
    }
}