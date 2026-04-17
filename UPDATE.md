# UPDATE.md — Histórico de Alterações do Projeto bebopCORE

Este arquivo documenta as principais alterações, correções e atualizações realizadas nos projetos do firmware bebopCORE (bbpC-switch, bbpC-blueretro, bbpC-sdk) para ESP32, incluindo fixes automatizados, rebrandings e melhorias de build.

---

## Histórico de Atualizações

### [bbpC-switch], [bbpC-blueretro], [bbpC-sdk]

- **Rebranding completo:**
  - Renomeação dos projetos e componentes para a nova identidade visual e de código.
  - Atualização de nomes em CMakeLists, arquivos de configuração e código-fonte.

- **Atualização de submódulos:**
  - Redirecionamento dos submódulos para forks próprios (pepa667).
  - Atualização e sincronização dos submódulos para garantir compatibilidade.

- **Fixes automatizados para build ESP-IDF v5.4 / GCC 14.2.0:**
  - Remoção de dependências incompatíveis (ex: 'riscv' em HOJA-LIB-ESP32).
  - Correção de comparações inválidas com ponteiros (`bd_addr == NULL`).
  - Ajustes em callbacks e assinaturas de funções (`void(void)` para tasks).
  - Correção de referências a variáveis globais e nomes de eventos (ex: `HEVT_BT_DISCONNECT` → `HEVT_BT_DISCONNECTED`).
  - Substituição de valores `NULL` por `0x00` onde necessário.
  - Ajuste de passagem de ponteiros para tasks (`&task`).

- **Scripts de build e manutenção:**
  - Criação do `fix-build.sh` para aplicar todos os patches necessários automaticamente após um clone limpo.
  - Documentação detalhada do processo de build inicial em `first-build.md`.

- **Melhorias de configuração:**
  - Adição de escolha de pinout (Original/Yakara) via menuconfig (Kconfig).
  - Atualização dos arquivos de configuração e headers para suportar múltiplos layouts de hardware.

- **Outros:**
  - Atualização de licenças e README.md.
  - Inclusão de imagens de montagem, esquemáticos e walkthroughs.
  - Correção de bugs menores e melhorias de compatibilidade.

---

### Como aplicar os fixes

1. Execute o script de correção após clonar o repositório:

   ```bash
   cd firmware/bbpC-switch
   bash fix-build.sh
   ```

2. Siga as instruções do `first-build.md` para garantir um build limpo.

---

**Gerado automaticamente por GitHub Copilot em 2026.**
