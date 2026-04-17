#!/usr/bin/env bash
# =============================================================================
# fix-build.sh — bbpC-switch first build patches
# Built with the help of GitHub Copilot
#
# Aplica todas as correções necessárias para compilar o firmware bbpC-switch
# com ESP-IDF v5.4 / GCC 14.2.0 (xtensa-esp32-elf) a partir de um clone limpo.
#
# Uso:
#   cd firmware/bbpC-switch
#   bash fix-build.sh
# =============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Cores ANSI
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
RESET='\033[0m'

ok()   { echo -e "${GREEN}✅  $*${RESET}"; }
warn() { echo -e "${YELLOW}⚠️   $*${RESET}"; }
err()  { echo -e "${RED}❌  $*${RESET}"; exit 1; }

apply_patch() {
    # apply_patch <file> <description> <python_script>
    local file="$1"
    local desc="$2"
    local script="$3"
    [[ -f "$file" ]] || err "Arquivo não encontrado: $file"
    python3 - "$file" <<< "$script" && ok "$desc" || warn "Padrão não encontrado (já aplicado?): $desc"
}

echo ""
echo "======================================================"
echo "  fix-build.sh — bbpC-switch patches"
echo "  GitHub Copilot · ESP-IDF v5.4 · ESP32 (Xtensa)"
echo "======================================================"
echo ""

# ------------------------------------------------------------------------------
# 1. Limpar cache CMake obsoleto
# ------------------------------------------------------------------------------
echo "→ [1/6] Limpando cache CMake..."
if [[ -f "build/CMakeCache.txt" ]] || [[ -d "build/CMakeFiles" ]]; then
    rm -rf build/CMakeCache.txt build/CMakeFiles
    ok "Cache CMake removido"
else
    warn "Cache CMake não encontrado (já limpo)"
fi

# ------------------------------------------------------------------------------
# 2. Remover dependência 'riscv' do HOJA-LIB-ESP32/CMakeLists.txt
# ------------------------------------------------------------------------------
echo "→ [2/6] Removendo dependência 'riscv' (incompatível com Xtensa/ESP32)..."
python3 - <<'PYEOF'
import sys
path = "components/HOJA-LIB-ESP32/CMakeLists.txt"
old = '    REQUIRES "bt" "hal" "xtensa"\n    "riscv" "esp_rom" "driver" "esp_common" '
new = '    REQUIRES "bt" "hal" "xtensa"\n    "esp_rom" "driver" "esp_common" '
with open(path, 'r') as f:
    content = f.read()
if old in content:
    with open(path, 'w') as f:
        f.write(content.replace(old, new))
    print("patched")
else:
    print("skipped")
PYEOF

# ------------------------------------------------------------------------------
# 3. util_bt_hid.c — remover comparação 'bd_addr == NULL'
# ------------------------------------------------------------------------------
echo "→ [3/6] Corrigindo comparação 'bd_addr == NULL' em util_bt_hid.c..."
python3 - <<'PYEOF'
path = "components/HOJA-LIB-ESP32/utilities/util_bt_hid.c"
old = (
    '            if (param->register_app.status == ESP_HIDD_SUCCESS) {\n'
    '                ESP_LOGI(TAG, "Register HIDD app parameters success!");\n'
    '                if(param->register_app.bd_addr == NULL)\n'
    '                {\n'
    '                    ESP_LOGI(TAG, "bd_addr is undefined!");\n'
    '                }\n'
    '            } else {'
)
new = (
    '            if (param->register_app.status == ESP_HIDD_SUCCESS) {\n'
    '                ESP_LOGI(TAG, "Register HIDD app parameters success!");\n'
    '            } else {'
)
with open(path, 'r') as f:
    content = f.read()
if old in content:
    with open(path, 'w') as f:
        f.write(content.replace(old, new))
    print("patched")
else:
    print("skipped")
PYEOF

# ------------------------------------------------------------------------------
# 4. core_switch_backend.c — remover comparação 'bd_addr == NULL'
# ------------------------------------------------------------------------------
echo "→ [4/6] Corrigindo comparação 'bd_addr == NULL' em core_switch_backend.c..."
python3 - <<'PYEOF'
path = "components/HOJA-LIB-ESP32/cores/core_switch_backend.c"
old = (
    '            if (param->register_app.status == ESP_HIDD_SUCCESS) {\n'
    '                ESP_LOGI(TAG, "Register HIDD app parameters success!");\n'
    '                if(param->register_app.bd_addr == NULL)\n'
    '                {\n'
    '                    ESP_LOGI(TAG, "bd_addr is undefined!");\n'
    '                }\n'
    '            } else {'
)
new = (
    '            if (param->register_app.status == ESP_HIDD_SUCCESS) {\n'
    '                ESP_LOGI(TAG, "Register HIDD app parameters success!");\n'
    '            } else {'
)
with open(path, 'r') as f:
    content = f.read()
if old in content:
    with open(path, 'w') as f:
        f.write(content.replace(old, new))
    print("patched")
else:
    print("skipped")
PYEOF

# ------------------------------------------------------------------------------
# 5. util_wired_detect.c — NULL→0x00 e task→&task
# ------------------------------------------------------------------------------
echo "→ [5/6] Corrigindo util_wired_detect.c (NULL e &task handle)..."
python3 - <<'PYEOF'
path = "components/HOJA-LIB-ESP32/utilities/util_wired_detect.c"
with open(path, 'r') as f:
    content = f.read()

patched = False

pairs = [
    (
        'hoja_event_cb(HOJA_EVT_WIRED, HEVT_WIRED_JOYBUS_DETECT, NULL);',
        'hoja_event_cb(HOJA_EVT_WIRED, HEVT_WIRED_JOYBUS_DETECT, 0x00);'
    ),
    (
        'hoja_event_cb(HOJA_EVT_WIRED, HEVT_WIRED_SNES_DETECT, NULL);',
        'hoja_event_cb(HOJA_EVT_WIRED, HEVT_WIRED_SNES_DETECT, 0x00);'
    ),
    (
        'xTaskCreatePinnedToCore(util_wired_detect_task, "Wired Utility Detect Loop", 4048, NULL, 2, util_wired_loop_task, HOJA_CORE_CPU);',
        'xTaskCreatePinnedToCore(util_wired_detect_task, "Wired Utility Detect Loop", 4048, NULL, 2, &util_wired_loop_task, HOJA_CORE_CPU);'
    ),
]

for old, new in pairs:
    if old in content:
        content = content.replace(old, new)
        patched = True

if patched:
    with open(path, 'w') as f:
        f.write(content)
    print("patched")
else:
    print("skipped")
PYEOF

# ------------------------------------------------------------------------------
# 6. main.c — callbacks void(void), globais diretos, typo DISCONNECT
# ------------------------------------------------------------------------------
echo "→ [6/6] Corrigindo main.c (callbacks, globais e typo)..."
python3 - <<'PYEOF'
path = "main/main.c"
with open(path, 'r') as f:
    content = f.read()

patched = False

# 6a. Assinatura button_task
old = 'void button_task(hoja_button_data_s *button_data)'
new = 'void button_task(void)'
if old in content:
    content = content.replace(old, new)
    patched = True

# 6b. Assinatura stick_task
old = 'void stick_task(hoja_analog_data_s* analog_data)'
new = 'void stick_task(void)'
if old in content:
    content = content.replace(old, new)
    patched = True

# 6c. Referências button_data-> → hoja_button_data.
import re
new_c, n = re.subn(r'\bbutton_data->', 'hoja_button_data.', content)
if n:
    content = new_c
    patched = True

# 6d. Referências analog_data-> → hoja_analog_data.
new_c, n = re.subn(r'\banalog_data->', 'hoja_analog_data.', content)
if n:
    content = new_c
    patched = True

# 6e. Typo HEVT_BT_DISCONNECT → HEVT_BT_DISCONNECTED
old = 'HEVT_BT_DISCONNECT)'
new = 'HEVT_BT_DISCONNECTED)'
if old in content:
    # Evitar substituição dupla se já corrigido
    if 'HEVT_BT_DISCONNECTED)' not in content.replace(old, new):
        pass
    content = content.replace(old, new)
    patched = True

if patched:
    with open(path, 'w') as f:
        f.write(content)
    print("patched")
else:
    print("skipped")
PYEOF

echo ""
echo "======================================================"
ok "Todos os patches aplicados!"
echo ""
echo "Execute agora:"
echo "  idf.py build"
echo "======================================================"
echo ""
