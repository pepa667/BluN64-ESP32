#include "main.h"
#include "driver/gpio.h" // Necessário para ler os pinos

void button_task(hoja_button_data_s *button_data)
{
    // Lê o estado do nosso botão SHIFT (GPIO 39)
    // Como tem pull-up externo, LOW (0) = Pressionado
    bool shift_active = !gpio_get_level(39); 

    // --- DIRECIONAL (Arcade Stick Digital) ---
    // button_data->dpad_up    |= !gpio_get_level(21);
    // button_data->dpad_down  |= !gpio_get_level(13);
    // button_data->dpad_left  |= !gpio_get_level(14);
    // button_data->dpad_right |= !gpio_get_level(27);

    // --- LÓGICA DO SHIFT (Layers) ---
    if (shift_active) {
        // LAYER 2 (Shift Segurado)
        // A vira X, B vira Y, Start vira Home, Select vira Sync
        button_data->button_up    |= !gpio_get_level(25); // A fisico dispara X
        button_data->button_left  |= !gpio_get_level(26); // B fisico dispara Y
        button_data->button_home  |= !gpio_get_level(32); // Start fisico dispara Home
        
        // Se quiser usar Select como Screenshot no Layer 2:
        // button_data->button_minus |= !gpio_get_level(33); 
    } else {
        // LAYER 1 (Normal)
        button_data->button_right       |= !gpio_get_level(25);
        button_data->button_down        |= !gpio_get_level(26);
        button_data->button_start       |= !gpio_get_level(32); // Start -> Start
        button_data->button_select      |= !gpio_get_level(33); // Select -> Select
    }

    // --- GATILHOS (Sempre iguais) ---
    button_data->trigger_l |= !gpio_get_level(19);
    button_data->trigger_r |= !gpio_get_level(23);
}

void event_task(hoja_event_type_t type, uint8_t evt, uint8_t param)
{
    printf("bebopCORE Event: \n\ttype: %d\n\tevent: %d\n\tparam: %d\n", type, evt, param);
    if (type == HOJA_EVT_BT && evt == HEVT_BT_DISCONNECTED)
    {
        esp_restart();
    }
}

void stick_task(hoja_analog_data_s* analog_data)
{
    // Começamos no centro (Neutro)
    analog_data->ls_x = 1020;
    analog_data->ls_y = 1020;

    // Eixo X (Esquerda / Direita)
    if (!gpio_get_level(27)) { // RIGHT_PIN
        analog_data->ls_x = 2040;
    } else if (!gpio_get_level(14)) { // LEFT_PIN
        analog_data->ls_x = 0;
    }

    // Eixo Y (Cima / Baixo)
    // No Switch, UP costuma ser o valor máximo (2040)
    if (!gpio_get_level(21)) { // UP_PIN
        analog_data->ls_y = 2040;
    } else if (!gpio_get_level(13)) { // DOWN_PIN
        analog_data->ls_y = 0;
    }
}

void app_main(void)
{
    printf("bebopCORE Arcade Mode. HEAP=%#010lx\n", esp_get_free_heap_size());

    // 1. A ORDEM CERTA DA NVS (Mata o erro 0x2)
    hoja_init();

    // 2. Registra os callbacks 
    // (Não registramos o analógico porque o arcade é digital)
    hoja_register_button_callback(button_task);
    hoja_register_analog_callback(stick_task);
    hoja_register_event_callback(event_task);

    // 3. Configura o Core
    hoja_set_core(HOJA_CORE_NS);
    
    // Mudamos para PROCON para compatibilidade total no Switch
    core_ns_set_subcore(NS_TYPE_PROCON); 

    // 4. Inicia
    while(hoja_start_core() != HOJA_OK)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        printf("bebopCORE. Retrying...\n");
    }
    printf("bebopCORE. Switch Connected!\n");
}