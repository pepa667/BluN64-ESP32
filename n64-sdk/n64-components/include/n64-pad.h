#ifndef N64_PAD_H
#define N64_PAD_H

#include "driver/gpio.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_system.h"

// N64 Controller Pinout (NO JOYSTICK ON HARDWARE)
#define DPAD_UP_PIN         GPIO_NUM_21
#define DPAD_DOWN_PIN       GPIO_NUM_13
#define DPAD_LEFT_PIN       GPIO_NUM_14
#define DPAD_RIGHT_PIN      GPIO_NUM_27
#define BUTTON_A_PIN        GPIO_NUM_25
#define BUTTON_B_PIN        GPIO_NUM_26
#define TRIGGER_L_PIN       GPIO_NUM_19
#define TRIGGER_R_PIN       GPIO_NUM_23
#define BUTTON_START_PIN    GPIO_NUM_32
#define BUTTON_SELECT_PIN   GPIO_NUM_33
#define BUTTON_SHIFT_PIN    GPIO_NUM_39
#define BUTTON_PROFILE_PIN  GPIO_NUM_35
#define BATTERY_SENSE_PIN   GPIO_NUM_34


//LEDs for mode indication
#define LED_POWER_PIN       GPIO_NUM_2
#define LED_SWITCH_PIN      GPIO_NUM_4
#define LED_RETROBLU_PIN    GPIO_NUM_18
#define LED_SHIFT_PIN       GPIO_NUM_16

// Bit Masks for button setup
#define BUTTONS_BIT_MASK        ((1ULL<<BUTTON_A_PIN) | (1ULL<<BUTTON_B_PIN) | (1ULL<<DPAD_UP_PIN) | \
                                (1ULL<<DPAD_DOWN_PIN) | (1ULL<<DPAD_LEFT_PIN) | (1ULL<<DPAD_RIGHT_PIN) | \
                                (1ULL<<TRIGGER_L_PIN) | (1ULL<<TRIGGER_R_PIN) | (1ULL<<BUTTON_START_PIN) | \
                                (1ULL<<BUTTON_SELECT_PIN) | (1ULL<<BUTTON_SHIFT_PIN) | (1ULL<<BUTTON_PROFILE_PIN) | (1ULL<<BATTERY_SENSE_PIN))

#define MODE_SWITCH     1
#define MODE_BLUERETRO  2

void n64_init(void);

// Joystick functionality has been completely removed in this branch: hardware does not provide analog stick.
// Do NOT add joystick code unless hardware is upgraded.

#endif