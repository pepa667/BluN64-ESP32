#include "main.h"

void button_task(void)
{
    // Buttons
    //   A
    hoja_button_data.button_down            |= !gpio_get_level(BUTTON_A_PIN);
    //   B
    hoja_button_data.button_left            |= !gpio_get_level(BUTTON_B_PIN);

    // Dpad
    hoja_button_data.dpad_up                |= !gpio_get_level(DPAD_UP_PIN);
    hoja_button_data.dpad_down              |= !gpio_get_level(DPAD_DOWN_PIN);
    hoja_button_data.dpad_left              |= !gpio_get_level(DPAD_LEFT_PIN);
    hoja_button_data.dpad_right             |= !gpio_get_level(DPAD_RIGHT_PIN);
    
    // Triggers
    hoja_button_data.trigger_zl             |= !gpio_get_level(TRIGGER_Z_PIN);
    //Uncomment the following line if you want to use a dedicated button for ZR instead of a combination (Experimental)
    //hoja_button_data.button_stick_left      |= !gpio_get_level(TRIGGER_ZR_PIN);

    // C-Buttons
    hoja_button_data.trigger_l              |= !gpio_get_level(C_LEFT_PIN);
    hoja_button_data.button_up              |= !gpio_get_level(C_UP_PIN);
    hoja_button_data.button_right           |= !gpio_get_level(C_DOWN_PIN);
    hoja_button_data.trigger_r              |= !gpio_get_level(C_RIGHT_PIN);

    // MISC
    //Uncomment the following line if you want to use a dedicated button for Home instead of a combination (Experimental)
    //hoja_button_data.button_home            |= !gpio_get_level(BUTTON_HOME_PIN);

    // Combinations
    if (!gpio_get_level(BUTTON_START_PIN) && !gpio_get_level(TRIGGER_L_PIN))
    {
        ESP_LOGI("BluN64", "Start + L combo (Home)");
        hoja_button_data.button_home        |= 1; //Home
    }
    else if (!gpio_get_level(BUTTON_START_PIN) && !gpio_get_level(TRIGGER_R_PIN))
    {
        ESP_LOGI("BluN64", "Start + R combo (ZR)");
        hoja_button_data.button_stick_left  |= 1; //ZR
    }
    else
    {
        //We should only check these buttons if it's not a combination
        hoja_button_data.button_select      |= !gpio_get_level(TRIGGER_L_PIN);
        hoja_button_data.trigger_zr         |= !gpio_get_level(TRIGGER_R_PIN);
        hoja_button_data.button_start       |= !gpio_get_level(BUTTON_START_PIN);

        //We should also reset the combo buttons
        hoja_button_data.button_home        |= 0; //Home
        hoja_button_data.button_stick_left  |= 0; //ZR
    }

    // NC
    hoja_button_data.button_stick_right     = 0;
}

void event_task(hoja_event_type_t type, uint8_t evt, uint8_t param)
{
    printf("BlueN64 Control Event: \n\ttype: %d\n\tevent: %d\n\tparam: %d\n", type, evt, param);
    if (type == HOJA_EVT_BT && evt == HEVT_BT_DISCONNECTED)
    {
        //This shouldn't be needed, but HOJA glitches after disconnecting the Switch... so... ¯\_(ツ)_/¯
        esp_restart();
    }
}

// Separate task to read sticks.
// This is essential to have as a separate component as ADC scans typically take more time and this is only
// scanned once between each polling interval. This varies from core to core.
void stick_task(void)
{
    // Joystick
    int x_data = n64_get_joystick_x();
    int y_data = n64_get_joystick_y();
	
    // Next two if statement sets a deadzone to avoid phantom readings when x and y values are within the range +-10
    if (abs(x_data) <= 10)
    {
        x_data = 0;
    }
    
    if (abs(y_data) <= 10)
    {
        y_data = 0;
    }
    
    hoja_analog_data.ls_x = (x_data + JOYSTICK_MAX_X) * JOYSTICK_ABS_MAX / JOYSTICK_MAX_X;
    hoja_analog_data.ls_y = (y_data + JOYSTICK_MAX_Y) * JOYSTICK_ABS_MAX / JOYSTICK_MAX_Y;

    return;
}

void app_main(void)
{
    printf("BlueN64 Control Switch Mode. HEAP=%#010lx\n", esp_get_free_heap_size());

    hoja_register_button_callback(button_task);
    hoja_register_analog_callback(stick_task);
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
