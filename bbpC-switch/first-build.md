# First Build — bbpC-switch

> **Built with the help of [GitHub Copilot](https://github.com/features/copilot)**

![GitHub Copilot](https://img.shields.io/badge/GitHub-Copilot-4078c0?logo=github&logoColor=white)
![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.4-blue?logo=espressif&logoColor=white)
![Target](https://img.shields.io/badge/Target-ESP32-E7352C?logo=espressif&logoColor=white)
![Architecture](https://img.shields.io/badge/Arch-Xtensa-lightgrey)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen?logo=cmake)
![License](https://img.shields.io/badge/License-MIT-green)

---

## Contexto

Este documento registra todas as alterações realizadas para obter o primeiro build bem-sucedido do firmware **bbpC-switch** (ESP32 → Nintendo Switch via BLE) com o toolchain **ESP-IDF v5.4** e compilador **GCC 14.2.0 (xtensa-esp32-elf)**.

O processo foi guiado pelo **GitHub Copilot**, que identificou e corrigiu automaticamente todos os erros de compilação em sequência.

---

## Problema 1 — Cache CMake incompatível

**Arquivo:** `build/CMakeCache.txt` + `build/CMakeFiles/`

**Erro:**

```shell
CMake Error: generator: Ninja
Does not match the generator used previously: Unix Makefiles
```

**Causa:** O diretório `build/` continha um cache gerado por um build anterior usando o gerador `Unix Makefiles`. O ESP-IDF atual usa `Ninja` por padrão.

**Solução:** Remoção do cache obsoleto.

```bash
rm -rf build/CMakeCache.txt build/CMakeFiles
```

---

## Problema 2 — Dependência `riscv` incompatível com ESP32

**Arquivo:** `components/HOJA-LIB-ESP32/CMakeLists.txt`

**Erro:**

```shell
Failed to resolve component 'riscv' required by component 'HOJA-LIB-ESP32':
component not registered.
```

**Causa:** O componente HOJA-LIB declarava dependência nos componentes `xtensa` **e** `riscv` simultaneamente. O ESP32 usa arquitetura Xtensa — o componente `riscv` não existe para esse target.

**Diff:**

```diff
-    REQUIRES "bt" "hal" "xtensa"
-    "riscv" "esp_rom" "driver" "esp_common"
+    REQUIRES "bt" "hal" "xtensa"
+    "esp_rom" "driver" "esp_common"
```

---

## Problema 3 — Comparação `bd_addr == NULL` inválida (GCC 12+)

**Arquivos:**

- `components/HOJA-LIB-ESP32/utilities/util_bt_hid.c` (linha ~32)
- `components/HOJA-LIB-ESP32/cores/core_switch_backend.c` (linha ~84)

**Erro:**

```shell
error: the comparison will always evaluate as 'false' for the address of
'bd_addr' will never be NULL [-Werror=address]
```

**Causa:** `bd_addr` é declarado como `esp_bd_addr_t bd_addr` (um array `uint8_t[6]`) dentro de uma struct — o endereço de um array nunca é `NULL`. O GCC 12+ promoveu esse warning a erro (`-Werror=address`).

**Diff (aplicado nos dois arquivos):**

```diff
 if (param->register_app.status == ESP_HIDD_SUCCESS) {
     ESP_LOGI(TAG, "Register HIDD app parameters success!");
-    if(param->register_app.bd_addr == NULL)
-    {
-        ESP_LOGI(TAG, "bd_addr is undefined!");
-    }
 } else {
```

---

## Problema 4 — Assinatura dos callbacks incompatível com a API HOJA atualizada

**Arquivo:** `main/main.c`

**Erros:**

```shell
error: passing argument 1 of 'hoja_register_button_callback' from
incompatible pointer type
  expected 'hoja_button_callback_t' {aka 'void (*)(void)'}
  but argument is of type 'void (*)(hoja_button_data_s *)'

error: passing argument 1 of 'hoja_register_analog_callback' from
incompatible pointer type
  expected 'hoja_analog_callback_t' {aka 'void (*)(void)'}
  but argument is of type 'void (*)(hoja_analog_data_s *)'
```

**Causa:** A HOJA-LIB foi atualizada para callbacks sem parâmetros (`void (*)(void)`). Os Dados de botões e analógicos são acessados diretamente pelas variáveis globais `hoja_button_data` e `hoja_analog_data` (declaradas em `hoja_backend.h`), em vez de serem passados como argumento.

**Diff:**

```diff
-void button_task(hoja_button_data_s *button_data)
+void button_task(void)
 {
-    button_data->button_down |= !gpio_get_level(BUTTON_A_PIN);
+    hoja_button_data.button_down |= !gpio_get_level(BUTTON_A_PIN);
     // ... (todas as referências button_data-> substituídas por hoja_button_data.)
 }

-void stick_task(hoja_analog_data_s* analog_data)
+void stick_task(void)
 {
-    analog_data->ls_x = ...;
-    analog_data->ls_y = ...;
+    hoja_analog_data.ls_x = ...;
+    hoja_analog_data.ls_y = ...;
 }
```

---

## Problema 5 — Constante `HEVT_BT_DISCONNECT` inexistente

**Arquivo:** `main/main.c` (linha ~62)

**Erro:**

```shell
error: 'HEVT_BT_DISCONNECT' undeclared (first use in this function);
did you mean 'HEVT_BT_DISCONNECTED'?
```

**Causa:** Typo — a constante correta é `HEVT_BT_DISCONNECTED`.

**Diff:**

```diff
-    if (type == HOJA_EVT_BT && evt == HEVT_BT_DISCONNECT)
+    if (type == HOJA_EVT_BT && evt == HEVT_BT_DISCONNECTED)
```

---

## Problema 6 — `NULL` em argumento `uint8_t` e `&` faltando em task handle

**Arquivo:** `components/HOJA-LIB-ESP32/utilities/util_wired_detect.c`

**Erros:**

```shell
error: passing argument 3 of 'hoja_event_cb' makes integer from pointer
without a cast [-Wint-conversion]

error: passing argument 6 of 'xTaskCreatePinnedToCore' from incompatible
pointer type [-Wincompatible-pointer-types]
```

**Causa 1:** `hoja_event_cb` espera `uint8_t` no terceiro argumento, mas estava recebendo `NULL` (ponteiro).

**Causa 2:** `xTaskCreatePinnedToCore` espera `TaskHandle_t *` (ponteiro para handle) no argumento 6, mas estava recebendo o handle diretamente sem `&`.

**Diff:**

```diff
-    hoja_event_cb(HOJA_EVT_WIRED, HEVT_WIRED_JOYBUS_DETECT, NULL);
+    hoja_event_cb(HOJA_EVT_WIRED, HEVT_WIRED_JOYBUS_DETECT, 0x00);

-    hoja_event_cb(HOJA_EVT_WIRED, HEVT_WIRED_SNES_DETECT, NULL);
+    hoja_event_cb(HOJA_EVT_WIRED, HEVT_WIRED_SNES_DETECT, 0x00);

-    xTaskCreatePinnedToCore(..., util_wired_loop_task, HOJA_CORE_CPU);
+    xTaskCreatePinnedToCore(..., &util_wired_loop_task, HOJA_CORE_CPU);
```

---

## Resultado

```shell
Successfully created esp32 image.
Project build complete. To flash, run:
 idf.py -p (PORT) flash
```

---

## Script de Reprodução

Para reaplicar todas as correções em um clone/cópia limpa do repositório, execute:

```bash
bash fix-build.sh
```

Ver: [fix-build.sh](./fix-build.sh)

---

## Arquivos Modificados

| Arquivo                                                   | Tipo de mudança                                          |
| --------------------------------------------------------- | -------------------------------------------------------- |
| `components/HOJA-LIB-ESP32/CMakeLists.txt`                | Removida dependência `riscv`                             |
| `components/HOJA-LIB-ESP32/utilities/util_bt_hid.c`       | Removida comparação `bd_addr == NULL`                    |
| `components/HOJA-LIB-ESP32/cores/core_switch_backend.c`   | Removida comparação `bd_addr == NULL`                    |
| `components/HOJA-LIB-ESP32/utilities/util_wired_detect.c` | `NULL→0x00` e `task→&task`                               |
| `main/main.c`                                             | Callbacks void(void), globals diretos, typo `DISCONNECT` |
