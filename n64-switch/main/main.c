#include "main.h"

static bool g_shift_active = false;

void button_task(void)
{
    memset(&hoja_button_data, 0, sizeof(hoja_button_data_s));

    g_shift_active = !gpio_get_level(BUTTON_SHIFT_PIN);

    if (g_shift_active)
    {
        // LAYER 2 (Shift Segurado)
        // Face buttons: A -> X, B -> Y; Start -> Home; Select -> Pair
        hoja_button_data.button_up = !gpio_get_level(BUTTON_A_PIN);
        hoja_button_data.button_left = !gpio_get_level(BUTTON_B_PIN);
        hoja_button_data.button_home = !gpio_get_level(BUTTON_START_PIN);
        hoja_button_data.button_pair = !gpio_get_level(BUTTON_SELECT_PIN);

        // Direcional -> D-pad digital
        hoja_button_data.dpad_up = !gpio_get_level(DPAD_UP_PIN);
        hoja_button_data.dpad_down = !gpio_get_level(DPAD_DOWN_PIN);
        hoja_button_data.dpad_left = !gpio_get_level(DPAD_LEFT_PIN);
        hoja_button_data.dpad_right = !gpio_get_level(DPAD_RIGHT_PIN);

        // Gatilhos -> ZL / ZR
        hoja_button_data.trigger_zl = !gpio_get_level(TRIGGER_L_PIN);
        hoja_button_data.trigger_zr = !gpio_get_level(TRIGGER_R_PIN);
    }
    else
    {
        // LAYER 1 (Normal)
        // Face buttons: A -> A, B -> B; Start/Select normais
        hoja_button_data.button_right = !gpio_get_level(BUTTON_A_PIN);
        hoja_button_data.button_down = !gpio_get_level(BUTTON_B_PIN);
        hoja_button_data.button_start = !gpio_get_level(BUTTON_START_PIN);
        hoja_button_data.button_select = !gpio_get_level(BUTTON_SELECT_PIN);

        // Gatilhos -> L / R
        hoja_button_data.trigger_l = !gpio_get_level(TRIGGER_L_PIN);
        hoja_button_data.trigger_r = !gpio_get_level(TRIGGER_R_PIN);

        // Direcional -> analógico falso (gerenciado em stick_task via g_shift_active)
    }
}

void event_task(hoja_event_type_t type, uint8_t evt, uint8_t param)
{

    printf("bebopCORE Event: \n\ttype: %d\n\tevent: %d\n\tparam: %d\n", type, evt, param);
    if (type == HOJA_EVT_BT && evt == HEVT_BT_DISCONNECTED)
    {
        esp_restart();
    }
}

void stick_task(void)
{
    // Centro neutro
    hoja_analog_data.ls_x = 1020;
    hoja_analog_data.ls_y = 1020;

    // Shift ativo -> direcional vai pro D-pad (button_task cuida disso), analógico fica neutro
    if (g_shift_active)
        return;

    // Shift inativo -> direcional vira analógico falso
    if (!gpio_get_level(DPAD_RIGHT_PIN))
        hoja_analog_data.ls_x = 2040;
    else if (!gpio_get_level(DPAD_LEFT_PIN))
        hoja_analog_data.ls_x = 0;

    if (!gpio_get_level(DPAD_UP_PIN))
        hoja_analog_data.ls_y = 2040;
    else if (!gpio_get_level(DPAD_DOWN_PIN))
        hoja_analog_data.ls_y = 0;
}

void app_main(void)
{
    printf("bebopCORE GB Mode. HEAP=%#010lx\n", esp_get_free_heap_size());

    // 1. HARDWARE PRIMEIRO
    n64_init();

    // 2. REGISTRA OS CALLBACKS (OBRIGATÓRIO ser antes do init)
    hoja_register_button_callback(button_task);
    hoja_register_analog_callback(stick_task);
    hoja_register_event_callback(event_task);

    // 3. INICIALIZA O HOJA (Agora ele vai ver os callbacks e não vai dar erro)
    hoja_err_t err = hoja_init();
    if (err != HOJA_OK)
    {
        printf("Falha no hoja_init: %d\n", err);
        return;
    }

    // 4. CONFIGURA O CORE
    hoja_set_core(HOJA_CORE_NS);
    core_ns_set_subcore(NS_TYPE_PROCON);

    // 5. SOBE O BLUETOOTH
    printf("Iniciando Bluetooth...\n");
    if (hoja_start_core() != HOJA_OK)
    {
        printf("Erro no start_core. Tentando novamente...\n");
        esp_restart();
    }
}