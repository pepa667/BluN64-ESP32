#include "main.h"

void button_task(hoja_button_data_s *button_data)
{
    // Buttons
    button_data->button_down     |= !gpio_get_level(BUTTON_A_PIN);
    button_data->button_left     |= !gpio_get_level(BUTTON_B_PIN);

    // Dpad
    button_data->dpad_up         |= !gpio_get_level(DPAD_UP_PIN);
    button_data->dpad_down       |= !gpio_get_level(DPAD_DOWN_PIN);
    button_data->dpad_left       |= !gpio_get_level(DPAD_LEFT_PIN);
    button_data->dpad_right      |= !gpio_get_level(DPAD_RIGHT_PIN);

    // Triggers
    button_data->trigger_r       |= !gpio_get_level(TRIGGER_R_PIN);
    button_data->button_select   |= !gpio_get_level(BUTTON_SELECT_PIN);

    // Combinations
    if (!gpio_get_level(BUTTON_START_PIN) && !gpio_get_level(TRIGGER_L_PIN)) {
        ESP_LOGI("BluN64", "Start + L combo (Home)");
        button_data->button_home |= 1; // Home
    } else {
        button_data->button_start |= !gpio_get_level(BUTTON_START_PIN);
        button_data->trigger_l    |= !gpio_get_level(TRIGGER_L_PIN);
        button_data->button_home  |= 0; // Home
    }
}

void event_task(hoja_event_type_t type, uint8_t evt, uint8_t param)
{
    printf("BlueN64 Control Event: \n\ttype: %d\n\tevent: %d\n\tparam: %d\n", type, evt, param);
    if (type == HOJA_EVT_BT && evt == HEVT_BT_DISCONNECT)
    {
        esp_restart();
    }
}

// No stick_task needed: no joystick in hardware.

void app_main(void)
{
    printf("BlueN64 Control Switch Mode. HEAP=%#010lx\n", esp_get_free_heap_size());

    hoja_register_button_callback(button_task);
    hoja_register_event_callback(event_task);

    blucontrol_mode_init(false);
    n64_init();

    hoja_init();
    hoja_set_core(HOJA_CORE_NS);
    core_ns_set_subcore(NS_TYPE_N64);

    while(hoja_start_core() != HOJA_OK)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        printf("BlueN64 Control. Retrying...\n");
    }
    printf("BlueN64 Control. Switch Connected!\n");
}
