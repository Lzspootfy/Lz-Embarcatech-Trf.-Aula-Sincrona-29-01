#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Definição dos pinos dos LEDs
#define LED_PIN_RED 13
#define LED_PIN_GREEN 12
#define LED_PIN_BLUE 11

// Definição do botão (Pushbutton)
#define BUTTON_PIN 5

// Estado da execução do temporizador
bool temporizador_ativo = false;

// Protótipos das funções de callback
int desligar_led2_callback(alarm_id_t id, void *user_data);
int desligar_led3_callback(alarm_id_t id, void *user_data);

// Função callback para iniciar a sequência de desligamento
int desligar_led1_callback(alarm_id_t id, void *user_data) {
    gpio_put(LED_PIN_RED, 0);
    printf("LED vermelho desligado!\n");
    add_alarm_in_ms(3000, desligar_led2_callback, NULL, false);
    return 0;
}

// Callback para desligar o segundo LED (azul)
int desligar_led2_callback(alarm_id_t id, void *user_data) {
    gpio_put(LED_PIN_BLUE, 0);
    printf("LED azul desligado!\n");
    add_alarm_in_ms(3000, desligar_led3_callback, NULL, false);
    return 0;
}

// Callback para desligar o terceiro LED (verde) e liberar o botão
int desligar_led3_callback(alarm_id_t id, void *user_data) {
    gpio_put(LED_PIN_GREEN, 0);
    printf("LED verde desligado! Sistema pronto para nova ativação.\n");
    temporizador_ativo = false; // Libera o botão para nova ativação
    return 0;
}

// Função para detectar o pressionamento do botão com debounce
bool detectar_botao() {
    if (!gpio_get(BUTTON_PIN)) { // Se o botão for pressionado (nível baixo)
        sleep_ms(50); // Debounce
        if (!gpio_get(BUTTON_PIN)) { // Confirma o pressionamento
            while (!gpio_get(BUTTON_PIN)); // Espera soltar o botão
            return true;
        }
    }
    return false;
}

int main() {
    stdio_init_all(); // Inicializa a comunicação serial

    // Inicializar os pinos GPIO dos LEDs e do botão
    gpio_init(LED_PIN_BLUE);
    gpio_init(LED_PIN_RED);
    gpio_init(LED_PIN_GREEN);
    gpio_init(BUTTON_PIN);

    // Configurar os pinos como saída (LEDs) e entrada (Botão)
    gpio_set_dir(LED_PIN_BLUE, true);
    gpio_set_dir(LED_PIN_RED, true);
    gpio_set_dir(LED_PIN_GREEN, true);
    gpio_set_dir(BUTTON_PIN, false);
    gpio_pull_up(BUTTON_PIN); // Habilita pull-up interno no botão

    printf("Sistema pronto. Pressione o botão para iniciar.\n");

    while (true) {
        if (!temporizador_ativo && detectar_botao()) { // Se o botão for pressionado e o sistema estiver pronto
            printf("Botão pressionado! LEDs ligados.\n");

            // Liga os 3 LEDs
            gpio_put(LED_PIN_BLUE, 1);
            gpio_put(LED_PIN_RED, 1);
            gpio_put(LED_PIN_GREEN, 1);

            // Bloqueia novas ativações até o ciclo terminar
            temporizador_ativo = true;

            // Inicia o temporizador para desligamento
            add_alarm_in_ms(3000, desligar_led1_callback, NULL, false);
        }
        sleep_ms(10); // Pequena pausa para evitar alto uso da CPU
    }
}
