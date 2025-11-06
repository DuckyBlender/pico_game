#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
// #include "hd44780/HD44780_LCD_PCF8574.hpp"

#define TOTAL_ROUNDS 5
#define BUTTON_GPIO 15

volatile bool waiting_for_press = false;
volatile bool waiting_for_game_start = false;
volatile int current_round = 0;
volatile int reaction_times[TOTAL_ROUNDS];
volatile absolute_time_t start_time;


// // LCD configuration
// #define CLOCK_PIN 5
// #define DATA_PIN  4
// #define CLOCK_SPEED 100
// #define I2C_ADDRESS 0x27

// // Create LCD object
// HD44780LCD myLCD(I2C_ADDRESS, i2c1, CLOCK_SPEED, DATA_PIN, CLOCK_PIN);

void button_callback(uint gpio, uint32_t events);

int main() {
    stdio_init_all();

    // Initialize GPIO 0 as output for LED
    gpio_init(0);
    gpio_set_dir(0, GPIO_OUT);

    // Initialize BUTTON_GPIO as input for button with pull-up
    gpio_init(BUTTON_GPIO);
    gpio_set_dir(BUTTON_GPIO, GPIO_IN);
    gpio_pull_up(BUTTON_GPIO);

    // Set up interrupt for button press (falling edge)
    gpio_set_irq_enabled_with_callback(BUTTON_GPIO, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    // Seed random number generator
    srand(to_us_since_boot(get_absolute_time()));

    // Give time to set up serial monitor
    sleep_ms(1000);

    while (true) {
        // Wait for user to start the game
        printf("Press the button to start a new game with %d rounds.\n", TOTAL_ROUNDS);
        waiting_for_game_start = true;
        while (waiting_for_game_start) {
            tight_loop_contents();
        }

        for (current_round = 0; current_round < TOTAL_ROUNDS; current_round++) {
            gpio_put(0, 1);
            printf("Round %d/%d: Get ready...\n", current_round + 1, TOTAL_ROUNDS);

            // Random delay between 1 and 6 seconds
            int delay_ms = (rand() % 5000) + 1000;
            sleep_ms(delay_ms);

            // Turn off LED
            gpio_put(0, 0);

            // Record start time
            start_time = get_absolute_time();

            // Wait for button press
            waiting_for_press = true;
            while (waiting_for_press) {
                tight_loop_contents();
            }
        }

        // Calculate and display average reaction time
        int64_t total_time = 0;
        for (int i = 0; i < TOTAL_ROUNDS; i++) {
            total_time += reaction_times[i];
        }
        int64_t average_time = total_time / TOTAL_ROUNDS;
        printf("Average reaction time: %lld ms\n", average_time);
        // Sleep to allow button to debounce before next game
        sleep_ms(500);
    }
}

void button_callback(uint gpio, uint32_t events) {
    if (gpio != BUTTON_GPIO) 
        return;
    

    if (waiting_for_press && !waiting_for_game_start) {
        // Record reaction time
        absolute_time_t press_time = get_absolute_time();
        int64_t diff_us = absolute_time_diff_us(start_time, press_time);
        int64_t diff_ms = diff_us / 1000;
        printf("Reaction time: %lld ms\n", diff_ms);
        reaction_times[current_round] = diff_ms;
        waiting_for_press = false;
    } else if (waiting_for_game_start) {
        // Start new game
        waiting_for_game_start = false;
    }
}
