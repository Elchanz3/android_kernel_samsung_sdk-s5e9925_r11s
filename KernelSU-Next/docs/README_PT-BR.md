[English](README.md) | [简体中文](README_CN.md) | [繁體中文](README_TW.md) | [Türkçe](README_TR.md) | **Português (Brasil)** | [한국어](README_KO.md) | [Français](README_FR.md) | [Bahasa Indonesia](README_ID.md) | [Русский](README_RU.md) | [ภาษาไทย](README_TH.md)

# KernelSU Next

<img src="/assets/kernelsu_next.png" style="width: 96px;" alt="logo">

Uma solução root baseada em kernel para dispositivos Android.

[![Latest Release](https://img.shields.io/github/v/release/rifsxd/KernelSU-Next?label=Release&logo=github)](https://github.com/rifsxd/KernelSU-Next/releases/latest)
[![Nightly Release](https://img.shields.io/badge/Nightly%20Release-gray?logo=hackthebox&logoColor=fff)](https://nightly.link/rifsxd/KernelSU-Next/workflows/build-manager/next/manager)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub License](https://img.shields.io/github/license/rifsxd/KernelSU-Next?logo=gnu)](/LICENSE)

## Características

1. `su` e gerenciamento de acesso root baseado em kernel.
2. Sistema de módulos baseado em sistema de montagem dinâmica [Magic Mount](https://topjohnwu.github.io/Magisk/details.html#magic-mount) / [OverlayFS](https://en.wikipedia.org/wiki/OverlayFS).
3. [Perfil do Aplicativo](https://kernelsu.org/pt_BR/guide/app-profile.html): Tranque o poder root em uma gaiola.

## Estado de compatibilidade

KernelSU Next suporta oficialmente a maioria dos kernels Android a partir de 4.4 até 6.6.
 - Os kernels GKI 2.0 (5.10+) podem executar imagens pré-construídas e LKM/KMI.
 - Os kernels GKI 1.0 (4.19 - 5.4) precisam ser reconstruídos com o driver KernelSU.
 - Os kernels EOL (<4.14) também precisam ser reconstruídos com o driver KernelSU (3.18+ é experimental e pode precisar portar algumas funções).

Atualmente, apenas `arm64-v8a` é suportado.

## Uso

- [Instruções de instalação](https://rifsxd.github.io/KernelSU-Next/)

## Segurança

Para obter informações sobre como relatar vulnerabilidades de segurança do KernelSU, consulte [SECURITY.md](/SECURITY.md).

## Licença

- Os arquivos no diretório `kernel` são [GPL-2.0-only](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
- Todas as outras partes, exceto o diretório `kernel` são [GPL-3.0-or-later](https://www.gnu.org/licenses/gpl-3.0.html).

## Créditos

- [kernel-assisted-superuser](https://git.zx2c4.com/kernel-assisted-superuser/about/): a ideia do KernelSU.
- [Magisk](https://github.com/topjohnwu/Magisk): a poderosa ferramenta root.
- [genuine](https://github.com/brevent/genuine/): validação de assinatura apk v2.
- [Diamorphine](https://github.com/m0nad/Diamorphine): algumas habilidades de rootkit.
- [KernelSU](https://github.com/tiann/KernelSU): obrigado a tiann, ou então o KernelSU Next nem existiria.
- [Magic Mount Port](https://github.com/5ec1cff/KernelSU/blob/main/userspace/ksud/src/magic_mount.rs): 💜 5ec1cff por salvar o KernelSU!
