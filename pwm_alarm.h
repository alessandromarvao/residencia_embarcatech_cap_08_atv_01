#ifndef PWM_ALARM_H
#define PWM_ALARM_H

#define BUZZER_PIN 21
#define MIN_FREQ 329
#define MAX_FREQ 590
#define STEP 2
#define STEP_DELAY_MS 10

// Aciona o som do buffer com efeito de sirene
void play_alarm();

// Para o som de alarme
void stop_alarm();

#endif