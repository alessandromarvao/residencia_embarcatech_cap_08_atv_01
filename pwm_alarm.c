#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pwm_alarm.h"

volatile bool is_led_on = false;

static void set_pwm_freq(uint gpio_pin, float freq)
{
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);
    uint channel = pwm_gpio_to_channel(gpio_pin);

    // Divisor de clock fixo para manter wrap dentro de 16 bits
    const float clock_divider = 8.0f;
    pwm_set_clkdiv(slice_num, clock_divider);

    // Cálculo do valor de wrap
    uint32_t wrap = (uint32_t)(125000000.0f / (clock_divider * freq)) - 1;
    pwm_set_wrap(slice_num, wrap);

    // Duty cycle de 50% para melhor qualidade de som
    // Para ajustar o volume: mude o duty cycle (ex: wrap / 4 para 25%)
    pwm_set_chan_level(slice_num, channel, wrap / 2);

    pwm_set_enabled(slice_num, true);
}

// Aciona o som do buffer com efeito de sirene
void play_alarm()
{
    // Toca enquanto estiver com a flag do alarme ativada

    // Subindo frequência (sobe tom)
    for (uint32_t f = MIN_FREQ; f <= MAX_FREQ; f += STEP)
    {
        set_pwm_freq(BUZZER_PIN, f);
        sleep_ms(STEP_DELAY_MS);
    }

    // Descendo frequência (desce tom)
    for (uint32_t f = MAX_FREQ; f > MIN_FREQ; f -= STEP)
    {
        set_pwm_freq(BUZZER_PIN, f);

        if (f == MAX_FREQ)
        {
            sleep_ms(2000);
        }
        else
        {
            sleep_ms(STEP_DELAY_MS);
        }
    }
}

void stop_alarm()
{
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice_num, false);            // Desativa o PWM
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_SIO); // Retorna o pino para GPIO padrão
    gpio_put(BUZZER_PIN, 0);                      // Garante que o pino esteja em nível baixo (sem som)
}
