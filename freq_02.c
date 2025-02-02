#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

// Definição dos pinos dos LEDs
#define LED_PIN_RED 13
#define LED_PIN_YELLOW 11
#define LED_PIN_GREEN 12

// Estado inicial do semáforo
int estado = 0;

int main() {
    stdio_init_all(); // Inicializa a comunicação serial

    // Inicializar os pinos GPIO dos LEDs
    gpio_init(LED_PIN_RED);
    gpio_init(LED_PIN_YELLOW);
    gpio_init(LED_PIN_GREEN);

    // Configurar os pinos como saída
    gpio_set_dir(LED_PIN_RED, true);
    gpio_set_dir(LED_PIN_YELLOW, true);
    gpio_set_dir(LED_PIN_GREEN, true);

    // Tempo para alternância dos LEDs (3 segundos)
    uint32_t intervalo_leds = 3000;

    // Tempo para impressão no terminal (1 segundo)
    uint32_t intervalo_msg = 1000;

    // Definir os tempos futuros
    absolute_time_t proximo_tempo_leds = delayed_by_us(get_absolute_time(), intervalo_leds * 1000);
    absolute_time_t proximo_tempo_msg = delayed_by_us(get_absolute_time(), intervalo_msg * 1000);

    while (true) {
        // Verifica se é hora de alternar os LEDs (a cada 3 segundos)
        if (time_reached(proximo_tempo_leds)) {
            // Alterna o estado do semáforo
            if (estado == 0) { // Vermelho
                gpio_put(LED_PIN_RED, 1);
                gpio_put(LED_PIN_YELLOW, 0);
                gpio_put(LED_PIN_GREEN, 0);
            } else if (estado == 1) { // Amarelo
                gpio_put(LED_PIN_RED, 0);
                gpio_put(LED_PIN_YELLOW, 1);
                gpio_put(LED_PIN_GREEN, 0);
            } else { // Verde
                gpio_put(LED_PIN_RED, 0);
                gpio_put(LED_PIN_YELLOW, 0);
                gpio_put(LED_PIN_GREEN, 1);
            }
            
            // Atualiza o estado (ciclo: vermelho -> amarelo -> verde)
            estado = (estado + 1) % 3;

            // Define o próximo tempo para alternância dos LEDs
            proximo_tempo_leds = delayed_by_us(proximo_tempo_leds, intervalo_leds * 1000);
        }

        // Verifica se é hora de imprimir a mensagem (a cada 1 segundo)
        if (time_reached(proximo_tempo_msg)) {
            printf("Semáforo funcionando...\n");
            proximo_tempo_msg = delayed_by_us(proximo_tempo_msg, intervalo_msg * 1000);
        }

        // Pequena pausa para evitar uso excessivo da CPU
        sleep_ms(1);
    }
}
