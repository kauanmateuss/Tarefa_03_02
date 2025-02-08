#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "matriz_leds.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "string.h"

// Definição dos pinos
#define led_green_pin 11
#define led_blue_pin 12
#define btnA_pin 5
#define btnB_pin 6

PIO pio;
uint sm;

// Pinos do display
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// Variáveis para debounce
static volatile uint32_t last_time_a = 0;
static volatile uint32_t last_time_b = 0;

volatile bool led_green_state = false;
volatile bool led_blue_state = false;

// Função para inicializar o display Oled
struct render_area start(){
    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    return frame_area;
}

struct render_area *frame_area_pont;

Matriz_leds_config zero = {
    //   Coluna 0         Coluna 1         Coluna 2         Coluna 3         Coluna 4
    // R    G    B      R    G    B      R    G    B      R    G    B      R    G    B
    {{0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 4
};

Matriz_leds_config um = {
    //   Coluna 0         Coluna 1         Coluna 2         Coluna 3         Coluna 4
    // R    G    B      R    G    B      R    G    B      R    G    B      R    G    B
    {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 4
};

Matriz_leds_config dois = {
    //   Coluna 0         Coluna 1         Coluna 2         Coluna 3         Coluna 4
    // R    G    B      R    G    B      R    G    B      R    G    B      R    G    B
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 4
};
Matriz_leds_config tres = {
    //   Coluna 0         Coluna 1         Coluna 2         Coluna 3         Coluna 4
    // R    G    B      R    G    B      R    G    B      R    G    B      R    G    B
    {{0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 4
};

Matriz_leds_config quatro = {
    //   Coluna 0         Coluna 1         Coluna 2         Coluna 3         Coluna 4
    // R    G    B      R    G    B      R    G    B      R    G    B      R    G    B
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 4
};

Matriz_leds_config cinco = {
    //   Coluna 0         Coluna 1         Coluna 2         Coluna 3         Coluna 4
    // R    G    B      R    G    B      R    G    B      R    G    B      R    G    B
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 4
};

Matriz_leds_config seis = {
    //   Coluna 0         Coluna 1         Coluna 2         Coluna 3         Coluna 4
    // R    G    B      R    G    B      R    G    B      R    G    B      R    G    B
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 4
};

Matriz_leds_config sete = {
    //   Coluna 0         Coluna 1         Coluna 2         Coluna 3         Coluna 4
    // R    G    B      R    G    B      R    G    B      R    G    B      R    G    B
    {{0.0, 0.0, 0.0}, {0.2, 0.05, 0.2}, {0.2, 0.2, 0.2}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.2, 0.2, 0.2}, {0.0, 0.0, 0.0}}, // Linha 4
};

Matriz_leds_config oito = {
    //   Coluna 0         Coluna 1         Coluna 2         Coluna 3         Coluna 4
    // R    G    B      R    G    B      R    G    B      R    G    B      R    G    B
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 4
};

Matriz_leds_config nove = {
    //   Coluna 0         Coluna 1         Coluna 2         Coluna 3         Coluna 4
    // R    G    B      R    G    B      R    G    B      R    G    B      R    G    B
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.05, 0.0}, {0.0, 0.0, 0.0}}, // Linha 4
};

Matriz_leds_config clear = {
    //   Coluna 0         Coluna 1         Coluna 2         Coluna 3         Coluna 4
    // R    G    B      R    G    B      R    G    B      R    G    B      R    G    B
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 0
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 1
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 2
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 3
    {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}, // Linha 4
};

Matriz_leds_config *numeros[] = {&zero, &um, &dois, &tres, &quatro, &cinco, &seis, &sete, &oito, &nove};

// Definindo os texto para os leds acesos e apagados:
char *led_azul_on[8] = {
        //"             "
        "               ",
        "               ",
        "               ",
        "  LED Azul ON  ",
        "               ",
        "               ",
        "               ",
        "               "
        };

char *led_azul_off[8] = {
        //"             "
        "               ",
        "               ",
        "               ",
        " LED Azul OFF  ",
        "               ",
        "               ",
        "               ",
        "               "
        };


char *led_verde_on[8] = {
        //"             "
        "               ",
        "               ",
        "               ",
        "  LED Verde ON ",
        "               ",
        "               ",
        "               ",
        "               "
        };

char *led_verde_off[8] = {
        //"             "
        "               ",
        "               ",
        "               ",
        " LED Verde OFF ",
        "               ",
        "               ",
        "               ",
        "               "
        };

// Protótipos das funções
void gpio_callback_btn(uint gpio, uint32_t events);

void print_display(char *text[8], size_t count, struct render_area *frame_area);

int main() {
    stdio_init_all();

    struct render_area frame_area = start();
    frame_area_pont = &frame_area;

    // Inicializações dos pinos
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

    // para trabalhar com a matriz de leds
    pio = pio0;
    sm = configurar_matriz(pio);

    // Configuração de interrupções
    gpio_set_irq_enabled_with_callback(btnA_pin, GPIO_IRQ_EDGE_FALL, true, &gpio_callback_btn);
    gpio_set_irq_enabled_with_callback(btnB_pin, GPIO_IRQ_EDGE_FALL, true, &gpio_callback_btn);

    char caractere;

    while (true) {
        // lendo o caractere pelo monitor serial:
        scanf(" %c", &caractere);

        // vai verificar se é um caractere maiúsculo ou minúsculo
        if ((caractere >= 65 && caractere <= 90) || (caractere >= 97 && caractere <= 122)){

            // Apaga a matriz de led se for letras
            imprimir_desenho(clear, pio, sm);

            char *text[8];
            for (int i = 0; i < 8; i++) {
                text[i] = malloc(32 * sizeof(char)); // Aloca espaço suficiente para cada string
                if (text[i] == NULL) {
                    printf("Erro ao alocar memória\n");
                }
            }

            // Preenchendo o array de strings com formato
            snprintf(text[0], 32, "               ");
            snprintf(text[1], 32, "               ");
            snprintf(text[2], 32, "   Caractere   ");
            snprintf(text[3], 32, "  digitado: %c ", caractere);
            snprintf(text[4], 32, "               ");
            snprintf(text[5], 32, "               ");
            snprintf(text[6], 32, "               ");
            snprintf(text[7], 32, "               ");

            // Mostra no display o caractere que foi digitado
            print_display(text,count_of(text), frame_area_pont);
        }
        // verifica se o caractere se é um numero entre 0 e 9:
        else if(caractere >= 48 && caractere <= 57){
            int i = caractere - 48;

            // Imprimindo o numero na matriz de leds
            imprimir_desenho(*numeros[i], pio, sm);
        }

        sleep_ms(1000);
    }

}   // FIM MAIN


// -------------------------------- FUNÇÔES --------------------------------

// Callback para o botão A
void gpio_callback_btn(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (gpio == btnA_pin && (current_time - last_time_a) > 200) {  // Debounce 200 ms
        last_time_a = current_time;
        led_green_state = !led_green_state;

        // Atualiza o LED Verde
        gpio_put(led_green_pin, led_green_state);

        // Escreve no display:
        led_green_state ? print_display(led_verde_on, count_of(led_verde_on), frame_area_pont) : print_display(led_verde_off, count_of(led_verde_off), frame_area_pont);

        // Mensagem no Serial Monitor
        printf("LED VERDE: %s\n", led_green_state ? "Ligado" : "Desligado");
    }
    else if (gpio == btnB_pin && (current_time - last_time_b) > 200) {  // Debounce 200 ms
        last_time_b = current_time;
        led_blue_state = !led_blue_state;

        // Atualiza o LED Azul
        gpio_put(led_blue_pin, led_blue_state);

        led_blue_state ? print_display(led_azul_on, count_of(led_azul_on), frame_area_pont) : print_display(led_azul_off, count_of(led_azul_off), frame_area_pont);

        // Mensagem no Serial Monitor
        printf("LED AZUL: %s\n", led_blue_state ? "Ligado" : "Desligado");
    }
}

void print_display(char *text[8], size_t count, struct render_area *frame_area){
    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, frame_area);

restart:

    int y = 0;
    for (uint i = 0; i < count; i++)
    {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, frame_area);
}