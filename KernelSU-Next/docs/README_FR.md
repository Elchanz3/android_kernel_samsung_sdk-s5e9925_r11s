[English](README.md) | [简体中文](README_CN.md) | [繁體中文](README_TW.md) | [Türkçe](README_TR.md) | [Português (Brasil)](README_PT-BR.md) | [한국어](README_KO.md) | **Français**  | [Bahasa Indonesia](README_ID.md) | [Русский](README_RU.md) | [ภาษาไทย](README_TH.md)

# KernelSU Next

<img src="/assets/kernelsu_next.png" style="width: 96px;" alt="logo">

Une solution root basée sur le noyau pour les appareils Android.

[![Dernière version](https://img.shields.io/github/v/release/rifsxd/KernelSU-Next?label=Release&logo=github)](https://github.com/rifsxd/KernelSU-Next/releases/latest)
[![Version Nightly](https://img.shields.io/badge/Nightly%20Release-gray?logo=hackthebox&logoColor=fff)](https://nightly.link/rifsxd/KernelSU-Next/workflows/build-manager/next/manager)
[![Licence : GPL v2](https://img.shields.io/badge/License-GPL%20v2-orange.svg?logo=gnu)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![Licence GitHub](https://img.shields.io/github/license/rifsxd/KernelSU-Next?logo=gnu)](/LICENSE)

## Fonctionnalités

1. Gestion des accès root et de la commande `su` basée sur le noyau.
2. Système de modules basé sur le système de montage dynamique [Magic Mount](https://topjohnwu.github.io/Magisk/details.html#magic-mount) / [OverlayFS](https://en.wikipedia.org/wiki/OverlayFS).
3. [Profil d'application](https://kernelsu.org/guide/app-profile.html) : Enfermez la puissance du root dans une cage.

## État de compatibilité

KernelSU Next prend officiellement en charge la plupart des noyaux Android de la version 4.4 à la version 6.6.
 - Les noyaux GKI 2.0 (5.10+) peuvent exécuter des images pré-construites et des modules LKM/KMI.
 - Les noyaux GKI 1.0 (4.19 - 5.4) doivent être reconstruits avec le pilote KernelSU.
 - Les noyaux EOL (<4.14) doivent également être reconstruits avec le pilote KernelSU (3.18+ est expérimental et peut nécessiter des rétroportages fonctionnels).

Actuellement, seul `arm64-v8a` est pris en charge.

## Utilisation

- [Instructions d'installation](https://rifsxd.github.io/KernelSU-Next/)

## Sécurité

Pour signaler des vulnérabilités de sécurité dans KernelSU, consultez [SECURITY.md](/SECURITY.md).

## Licence

- Les fichiers du répertoire `kernel` sont sous licence [GPL-2.0-only](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
- Toutes les autres parties, sauf le répertoire `kernel`, sont sous licence [GPL-3.0-or-later](https://www.gnu.org/licenses/gpl-3.0.html).

## Crédits

- [kernel-assisted-superuser](https://git.zx2c4.com/kernel-assisted-superuser/about/) : l'idée de KernelSU.
- [Magisk](https://github.com/topjohnwu/Magisk) : l'outil root puissant.
- [genuine](https://github.com/brevent/genuine/) : validation de signature apk v2.
- [Diamorphine](https://github.com/m0nad/Diamorphine) : quelques techniques de rootkit.
- [KernelSU](https://github.com/tiann/KernelSU) : merci à tiann, sans qui KernelSU Next n'existerait même pas.
- [Magic Mount Port](https://github.com/5ec1cff/KernelSU/blob/main/userspace/ksud/src/magic_mount.rs) : 💜 5ec1cff pour avoir sauvé KernelSU !
