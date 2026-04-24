#ifndef N64_PAD_H
#define N64_PAD_H

#include "driver/gpio.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_system.h"

#if CONFIG_BLUN64_PINOUT_TYPE_ORIGINAL
    // Buttons
    #define BUTTON_A_PIN        GPIO_NUM_25
    #define BUTTON_B_PIN        GPIO_NUM_26

    // DPAD
    #define DPAD_UP_PIN         GPIO_NUM_12
    #define DPAD_DOWN_PIN       GPIO_NUM_13
    #define DPAD_LEFT_PIN       GPIO_NUM_14
    #define DPAD_RIGHT_PIN      GPIO_NUM_27

    // Triggers
    #define TRIGGER_L_PIN       GPIO_NUM_19
    #define TRIGGER_R_PIN       GPIO_NUM_23
    // #define TRIGGER_Z_PIN       GPIO_NUM_NC
    // #define TRIGGER_ZR_PIN      GPIO_NUM_NC

    // MISC
    #define BUTTON_START_PIN    GPIO_NUM_32
    #define BUTTON_SELECT_PIN    GPIO_NUM_33
    #define BUTTON_HOME_PIN     0

    // C-Buttons
    // #define C_UP_PIN            GPIO_NUM_NC
    // #define C_DOWN_PIN          GPIO_NUM_NC
    // #define C_LEFT_PIN          GPIO_NUM_NC
    // #define C_RIGHT_PIN         GPIO_NUM_NC

    // // Joystick
    // #define JOYSTICK_X_INT_PIN  0
    // #define JOYSTICK_X_Q_PIN    0
    // #define JOYSTICK_Y_INT_PIN  0
    // #define JOYSTICK_Y_Q_PIN    0

    // LEDs
    // #define LED_MODE_PIN        GPIO_NUM_NC
#else
    // Buttons
    #define BUTTON_A_PIN        GPIO_NUM_25
    #define BUTTON_B_PIN        GPIO_NUM_26

    // DPAD
    #define DPAD_UP_PIN         GPIO_NUM_12
    #define DPAD_DOWN_PIN       GPIO_NUM_13
    #define DPAD_LEFT_PIN       GPIO_NUM_14
    #define DPAD_RIGHT_PIN      GPIO_NUM_27

    // Triggers
    #define TRIGGER_L_PIN       GPIO_NUM_19
    #define TRIGGER_R_PIN       GPIO_NUM_23
    // #define TRIGGER_Z_PIN       GPIO_NUM_NC
    // #define TRIGGER_ZR_PIN      GPIO_NUM_NC

    // MISC
    #define BUTTON_START_PIN    GPIO_NUM_32
    #define BUTTON_SELECT_PIN    GPIO_NUM_33
    #define BUTTON_HOME_PIN     0

    // C-Buttons
    // #define C_UP_PIN            GPIO_NUM_NC
    // #define C_DOWN_PIN          GPIO_NUM_NC
    // #define C_LEFT_PIN          GPIO_NUM_NC
    // #define C_RIGHT_PIN         GPIO_NUM_NC

    // Joystick
    // #define JOYSTICK_X_INT_PIN  GPIO_NUM_NC
    // #define JOYSTICK_X_Q_PIN    GPIO_NUM_NC
    // #define JOYSTICK_Y_INT_PIN  GPIO_NUM_NC
    // #define JOYSTICK_Y_Q_PIN    GPIO_NUM_NC

    // LEDs

#endif

// #define JOYSTICK_MAX_Y      40
// #define JOYSTICK_MAX_X      40

// #define JOYSTICK_ABS_MAX    2040

// Bit Masks
#define BUTTONS_BIT_MASK        ((1ULL<<BUTTON_A_PIN) | (1ULL<<BUTTON_B_PIN) | (1ULL<<DPAD_UP_PIN) | (1ULL<<DPAD_DOWN_PIN) | (1ULL<<DPAD_LEFT_PIN) | (1ULL<<DPAD_RIGHT_PIN) | (1ULL<<TRIGGER_L_PIN) | (1ULL<<TRIGGER_R_PIN) | (1ULL<<BUTTON_START_PIN) | (1ULL<<BUTTON_SELECT_PIN) | (1ULL<<BUTTON_HOME_PIN))
// #define JOYSTICK_Q_BIT_MASK     ((1ULL<<JOYSTICK_X_Q_PIN) | (1ULL<<JOYSTICK_Y_Q_PIN))
// #define JOYSTICK_INT_BIT_MASK   ((1ULL<<JOYSTICK_X_INT_PIN) | (1ULL<<JOYSTICK_Y_INT_PIN))

// Flags
// #define JOYSTICK_X_AXIS 0
// #define JOYSTICK_Y_AXIS 1

#define MODE_SWITCH     1
#define MODE_BLUERETRO  2

void n64_init(void);
// int n64_get_joystick_x(void);
// int n64_get_joystick_y(void);

#endif
