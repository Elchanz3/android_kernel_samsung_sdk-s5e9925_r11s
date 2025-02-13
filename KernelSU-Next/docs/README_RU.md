[English](README.md) | [简体中文](README_CN.md) | [繁體中文](README_TW.md) | [Türkçe](README_TR.md) | [Português (Brasil)](README_PT-BR.md) | [한국어](README_KO.md) | [Français](README_FR.md) | [Bahasa Indonesia](README_ID.md) | **Русский** | [ภาษาไทย](README_TH.md)

# KernelSU Next

<img src="/assets/kernelsu_next.png" style="width: 96px;" alt="logo">

Root-решение для Android на базе ядра. 

[![Последний релиз](https://img.shields.io/github/v/release/rifsxd/KernelSU-Next?label=Release&logo=github)](https://github.com/rifsxd/KernelSU-Next/releases/latest)  
[![Ночные сборки](https://img.shields.io/badge/Nightly%20Release-gray?logo=hackthebox&logoColor=fff)](https://nightly.link/rifsxd/KernelSU-Next/workflows/build-manager-ci/next/manager)  
[![Лицензия: GPL v2](https://img.shields.io/badge/License-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)  
[![Лицензия GitHub](https://img.shields.io/github/license/rifsxd/KernelSU-Next?logo=gnu)](/LICENSE)

## Функции

1. Реализация `su` и управление root-доступом прямо на уровне ядра.  
2. Динамическая система модулей, построенная на [Magic Mount](https://topjohnwu.github.io/Magisk/details.html#magic-mount) / [OverlayFS](https://en.wikipedia.org/wiki/OverlayFS).  
3. [Профиль для приложений](https://kernelsu.org/guide/app-profile.html): позволяет ограничить root-доступ в песочницу для отдельных приложений.

## Совместимость

KernelSU Next работает с большинством ядер Android (4.4 - 6.6):  
- GKI 2.0 (5.10+) могут использовать предсобранные образы и LKM/KMI.  
- GKI 1.0 (4.19 - 5.4) требуют пересборки с драйвером KernelSU.  
- EOL (<4.14) также требуют пересборки с драйвером KernelSU (версии 3.18+ экспериментальные и могут потребовать некоторые функции бэкпортов).  

Сейчас поддерживается только `arm64-v8a`.

## Использование

- [Инструкция по установке](https://rifsxd.github.io/KernelSU-Next/)

## Безопасность

Если нашли баг, посмотри [SECURITY.md](/SECURITY.md) — там гайд, как сообщить о проблеме.

## Лицензия

- Всё, что в директории `kernel`, — [GPL-2.0-only](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).  
- Остальной код, кроме директории `kernel`, под [GPL-3.0-or-later](https://www.gnu.org/licenses/gpl-3.0.html).

## Благодарность

- [kernel-assisted-superuser](https://git.zx2c4.com/kernel-assisted-superuser/about/): идея KernelSU.  
- [Magisk](https://github.com/topjohnwu/Magisk): топовый инструмент для root.  
- [genuine](https://github.com/brevent/genuine/): валидация подписи APK v2.  
- [Diamorphine](https://github.com/m0nad/Diamorphine): некоторые навыки rootkit.  
- [KernelSU](https://github.com/tiann/KernelSU): спасибо tiann, без него KernelSU Next не релизнулся бы.  
- [Magic Mount Port](https://github.com/5ec1cff/KernelSU/blob/main/userspace/ksud/src/magic_mount.rs): 💜 5ec1cff за сохранение KernelSU!