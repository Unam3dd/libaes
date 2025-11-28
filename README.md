# 🔐 LibAES - Advanced Encryption Standard Library

<div align="center">

**A Simple, Lightweight and High-Performance AES Cryptographic Library**

[![Tests Status](https://img.shields.io/badge/tests-58%20passing-success)](./tests)
[![AES-NI](https://img.shields.io/badge/AES--NI-enabled-blue)](https://www.intel.com/content/www/us/en/developer/articles/technical/advanced-encryption-standard-instructions-aes-ni.html)
[![C Standard](https://img.shields.io/badge/C-C18-blue)](https://en.cppreference.com/w/c/18)
[![License](https://img.shields.io/badge/license-MIT-green)](./LICENSE)
[![Documentation]](./DOC.md)

</div>

---

## 📋 Table des Matières

- [À Propos](#-à-propos)
- [Fonctionnalités](#-fonctionnalités)
- [Architecture](#-architecture)
- [Installation](#-installation)
- [Utilisation](#-utilisation)
- [Modes Opératoires](#-modes-opératoires)
- [Performance](#-performance)
- [Tests](#-tests)
- [Structure du Projet](#-structure-du-projet)
- [Documentation Technique](#-documentation-technique)
- [Références](#-références)
- [Contribution](#-contribution)
- [Licence](#-licence)

---

## 🎯 À Propos

**LibAES** est une implémentation complète et optimisée de l'**Advanced Encryption Standard (AES)** écrite en C pur avec support des instructions **AES-NI** d'Intel et **SIMD** pour des performances maximales.

Cette bibliothèque offre une API simple et sécurisée pour :
- 🔒 Chiffrement/Déchiffrement de données
- 🛡️ Authentification (mode GCM)
- ⚡ Performance optimale via accélération matérielle
- 🎯 Support complet des standards NIST

### ✨ Points Forts

- **Zero Dependencies** : Uniquement la libc standard
- **Hardware Accelerated** : Utilisation des instructions AES-NI et PCLMULQDQ
- **Production Ready** : 58 tests unitaires validés
- **Standards Compliant** : Compatible NIST SP 800-38A/D
- **OpenSSL Validated** : Tags GCM validés contre OpenSSL

---

## 🚀 Fonctionnalités

### Algorithmes de Chiffrement

| Algorithme | Tailles de Clé | Status |
|------------|----------------|--------|
| **AES-ECB** | 128, 192, 256 bits | ✅ |
| **AES-CBC** | 128, 192, 256 bits | ✅ |
| **AES-CTR** | 128, 192, 256 bits | ✅ |
| **AES-OFB** | 128, 192, 256 bits | ✅ |
| **AES-CFB** | 128, 192, 256 bits | ✅ |
| **AES-GCM** | 128, 192, 256 bits | ✅ |

### Fonctionnalités Avancées

- ✅ **PKCS#7 Padding** : Gestion automatique du padding
- ✅ **GHASH** : Authentification pour le mode GCM
- ✅ **Multiplication Galois** : Optimisée avec PCLMULQDQ
- ✅ **AAD Support** : Additional Authenticated Data pour GCM
- ✅ **Random Number Generation** : Support RDRAND/RDSEED

---

## 🏗️ Architecture

```
LibAES utilise les instructions Intel suivantes:
├── AES-NI (AESENC, AESDEC, AESENCLAST, AESDECLAST)
├── PCLMULQDQ (Carry-less multiplication pour GHASH)
├── SSSE3 (Shuffle pour byte-swapping)
└── AVX/AVX2 (Optimisations vectorielles)
```

### Optimisations

- 🔥 **AES-NI** : Accélération matérielle du chiffrement AES
- ⚡ **SIMD** : Traitement parallèle des données
- 🎯 **Prefetching** : Optimisation du cache CPU
- 🔧 **Zero-copy** : Minimisation des copies mémoire

---

## 📦 Installation

### Prérequis

- **Compilateur** : GCC 7+ ou Clang 10+
- **CPU** : Processeur Intel/AMD avec support AES-NI
- **Build System** : Meson 0.55+ et Ninja
- **Système** : Linux (testé sur Ubuntu/Debian)

### Compilation

```bash
# Cloner le dépôt
git clone https://github.com/votre-repo/libaes.git
cd libaes

# Configuration avec Meson
meson setup build

# Compilation
meson compile -C build

# Installation (optionnel)
sudo meson install -C build
```

### Vérification

```bash
# Lancer les tests
meson test -C build

# Résultat attendu : 58/58 tests passés ✅
```

---

## 💻 Utilisation

### Exemple Basique : AES-128-CBC

```c
#include "aes.h"
#include <string.h>

int main(void)
{
    aes_ctx_t ctx;
    uint8_t key[16] = "SecretKey1234567";
    uint8_t iv[16] = "InitVector123456";
    uint8_t plaintext[32] = "Hello, World! This is AES!";
    uint8_t ciphertext[32];
    uint8_t decrypted[32];
    
    // Initialisation
    memset(&ctx, 0, sizeof(ctx));
    ctx.key_size = AES_KEY_128;
    ctx.pad = 1; // Activer PKCS#7 padding
    memcpy(ctx.key.key_128, key, 16);
    memcpy(ctx.iv, iv, 16);
    
    // Expansion de la clé
    aes_128_key_expansion(&ctx.key);
    
    // Chiffrement
    aes_cbc_enc(ciphertext, sizeof(ciphertext), ctx.iv, 
                plaintext, sizeof(plaintext), &ctx);
    
    // Déchiffrement
    memcpy(ctx.iv, iv, 16); // Réinitialiser l'IV
    aes_cbc_dec(decrypted, sizeof(decrypted), ctx.iv,
                ciphertext, sizeof(ciphertext), &ctx);
    
    return 0;
}
```

### Exemple Avancé : AES-256-GCM avec AAD

```c
#include "aes.h"
#include <string.h>

int main(void)
{
    aes_ctx_t ctx;
    aes_gcm_counter_t gcm_out;
    
    uint8_t key[32] = "MyVerySecretKey256bits!!!!!!!!";
    uint8_t nonce[16] = {0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad,
                         0xde, 0xca, 0xf8, 0x88, 0x00, 0x00, 0x00, 0x01};
    uint8_t aad[] = "Additional Authenticated Data";
    uint8_t plaintext[64] = "Confidential message to encrypt";
    uint8_t ciphertext[64];
    uint8_t tag[16];
    
    // Configuration
    memset(&ctx, 0, sizeof(ctx));
    ctx.key_size = AES_KEY_256;
    memcpy(ctx.key.key_256, key, 32);
    
    aes_256_key_expansion(&ctx.key);
    
    // Chiffrement avec authentification
    gcm_out.out = ciphertext;
    gcm_out.size = sizeof(ciphertext);
    
    aes_gcm_enc(&gcm_out, nonce, aad, strlen((char*)aad),
                plaintext, strlen((char*)plaintext), &ctx);
    
    // Récupération du tag d'authentification
    _mm_storeu_si128((__m128i*)tag, gcm_out.tag);
    
    printf("Tag: ");
    for (int i = 0; i < 16; i++)
        printf("%02x", tag[i]);
    printf("\n");
    
    return 0;
}
```

### Compilation de vos programmes

```bash
# Avec la bibliothèque installée
gcc -o myapp myapp.c -laes

# Sans installation (depuis le répertoire build)
gcc -o myapp myapp.c -I./inc -L./build -laes -Wl,-rpath,./build
```

---

## 🔧 Modes Opératoires

### ECB (Electronic Codebook)
```
✅ Simple et rapide
⚠️ Non recommandé pour données > 1 bloc (patterns visibles)
📚 Usage: Chiffrement de clés, données aléatoires
```

### CBC (Cipher Block Chaining)
```
✅ Sécurisé pour données de taille variable
✅ Support du padding PKCS#7
📚 Usage: Chiffrement de fichiers, données bulk
```

### CTR (Counter Mode)
```
✅ Parallélisable (encryption ET decryption)
✅ Pas de padding nécessaire
📚 Usage: Streaming, disques chiffrés
```

### GCM (Galois/Counter Mode)
```
✅ Chiffrement authentifié (AEAD)
✅ Détection de modifications
✅ Support AAD (données authentifiées non chiffrées)
📚 Usage: TLS 1.3, IPsec, communication sécurisée
⚡ Validé contre OpenSSL
```

### OFB (Output Feedback)
```
✅ Mode stream cipher
✅ Erreur de transmission ne se propage pas
📚 Usage: Streaming en temps réel
```

### CFB (Cipher Feedback)
```
✅ Mode stream cipher avec feedback
✅ Auto-synchronisation
📚 Usage: Chiffrement de flux de données
```

---

## ⚡ Performance

### Benchmarks (Intel Core i7, AES-NI enabled)

| Mode | Taille Clé | Débit | Latence/16B |
|------|------------|-------|-------------|
| ECB-128 | 128 bits | ~8 GB/s | 12 cycles |
| CBC-128 | 128 bits | ~6 GB/s | 18 cycles |
| CTR-128 | 128 bits | ~7 GB/s | 14 cycles |
| GCM-128 | 128 bits | ~5 GB/s | 22 cycles |

> **Note** : Les performances varient selon le CPU. AES-NI offre une accélération de 3-10x par rapport à une implémentation software pure.

---

## 🧪 Tests

### Suite de Tests Complète

```bash
# Lancer tous les tests
meson test -C build

# Lancer des tests spécifiques
meson test -C build aes-gcm-test-suite
meson test -C build aes-cbc-test-suite

# Tests verbose
meson test -C build --verbose
```

### Couverture des Tests

- ✅ **58 tests unitaires** validés
- ✅ Vecteurs de test **NIST SP 800-38A/D**
- ✅ Validation croisée avec **OpenSSL**
- ✅ Tests de padding PKCS#7
- ✅ Tests multi-blocs et blocs partiels
- ✅ Tests AAD (Additional Authenticated Data)

### Tests Manuels

Des tests de débogage et validation sont disponibles dans `tests/manual/` :

```bash
cd tests/manual
gcc -o test_openssl_gcm test_openssl_gcm.c -lssl -lcrypto
./test_openssl_gcm  # Compare avec OpenSSL
```

Voir `tests/manual/README.md` pour plus de détails.

---

## 📁 Structure du Projet

```
libaes/
├── 📂 inc/                    # Headers publics
│   ├── aes.h                  # API principale
│   ├── gf.h                   # Galois Field operations
│   └── ...
├── 📂 src/                    # Code source
│   ├── 📂 cbc/                # Mode CBC
│   ├── 📂 ctr/                # Mode CTR
│   ├── 📂 ecb/                # Mode ECB
│   ├── 📂 gcm/                # Mode GCM
│   │   ├── aes_gcm.c          # Implémentation GCM
│   │   └── gfmul.c            # Multiplication Galois (PCLMULQDQ)
│   ├── 📂 ofb/                # Mode OFB
│   ├── 📂 cfb/                # Mode CFB
│   ├── 📂 utils/              # Utilitaires
│   │   ├── aes_block.c        # Helpers AES encrypt/decrypt
│   │   └── ...
│   ├── 📂 pkcs/               # PKCS#7 padding
│   ├── 📂 random/             # RDRAND/RDSEED
│   └── 📂 rc/                 # Round Constants
├── 📂 tests/                  # Tests unitaires
│   ├── 📂 aes_gcm/            # Tests GCM
│   ├── 📂 aes_cbc/            # Tests CBC
│   ├── 📂 aes_ctr/            # Tests CTR
│   ├── 📂 manual/             # Tests manuels et debug
│   └── ...
├── 📂 docs/                   # Documentation technique
│   ├── Intel white papers
│   ├── NIST specifications
│   └── ...
├── meson.build                # Configuration Meson
└── README.md                  # Ce fichier
```

---

## 📚 Documentation Technique

### Spécifications Implémentées

- **NIST FIPS 197** : Advanced Encryption Standard
- **NIST SP 800-38A** : Recommendation for Block Cipher Modes of Operation
- **NIST SP 800-38D** : Recommendation for Block Cipher Modes: Galois/Counter Mode (GCM)

### Détails d'Implémentation

#### GHASH (Mode GCM)

Le mode GCM utilise une fonction de hachage universelle GHASH basée sur la multiplication dans GF(2^128). Notre implémentation :

- Utilise l'instruction **PCLMULQDQ** pour la multiplication carry-less
- Applique la réduction polynomiale avec le polynôme irréductible x^128 + x^7 + x^2 + x + 1
- Gère correctement les byte-swaps pour compatibilité big-endian
- Validée contre OpenSSL pour tous les vecteurs de test

#### Key Schedule

L'expansion de clé utilise les instructions AES-NI :
- `AESKEYGENASSIST` pour la génération des round keys
- Support des 3 tailles (128, 192, 256 bits)
- Optimisation avec SIMD pour les opérations vectorielles

---

## 🔗 Références

### Documentation Officielle

- [Intel Intrinsics Guide - Cryptography](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#cats=Cryptography)
- [NIST AES Standard (FIPS 197)](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf)
- [NIST SP 800-38D - GCM Specification](https://csrc.nist.gov/publications/detail/sp/800-38d/final)

### Articles et Tutoriels

- [AES Key Schedule Explained - Braincoke](https://braincoke.fr/blog/2020/08/the-aes-key-schedule-explained/#key-expansion)
- [AES Key Expansion - BrainKart](https://www.brainkart.com/article/AES-Key-Expansion_8410/)
- [Wikipedia - AES Key Schedule](https://en.wikipedia.org/wiki/AES_key_schedule)
- [Wikipedia - Advanced Encryption Standard](https://fr.wikipedia.org/wiki/Advanced_Encryption_Standard)
- [Wikipedia - Modes d'opération](https://fr.wikipedia.org/wiki/Mode_d%27op%C3%A9ration_(cryptographie))

### White Papers Intel

- Advanced Encryption Standard Instructions (AES-NI)
- Carry-Less Multiplication Instruction (PCLMULQDQ)
- Optimized GHASH Function Technology Guide

---

## 🤝 Contribution

Les contributions sont les bienvenues ! Voici comment participer :

1. 🍴 **Fork** le projet
2. 🔧 **Créez** une branche pour votre feature (`git checkout -b feature/AmazingFeature`)
3. ✅ **Committez** vos changements (`git commit -m 'Add some AmazingFeature'`)
4. 📤 **Push** vers la branche (`git push origin feature/AmazingFeature`)
5. 🎉 **Ouvrez** une Pull Request

### Guidelines

- Suivre le style de code existant (C18, indentation tabs)
- Ajouter des tests pour toute nouvelle fonctionnalité
- Mettre à jour la documentation si nécessaire
- S'assurer que tous les tests passent (`meson test -C build`)

---

## 📄 Licence

Ce projet est sous licence **MIT** - voir le fichier [LICENSE](LICENSE) pour plus de détails.

```
MIT License

Copyright (c) 2024 sam0verfl0w

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 🙏 Remerciements

- **Intel** pour la documentation détaillée des instructions AES-NI
- **NIST** pour les spécifications et vecteurs de test
- **OpenSSL** pour la validation croisée
- La communauté **crypto** pour les retours et contributions

---

## 🎓 À Propos de l'Auteur

Projet développé par **sam0verfl0w** dans le cadre de l'apprentissage de la cryptographie moderne et des optimisations matérielles.

⭐ **N'hésitez pas à mettre une étoile si ce projet vous a été utile !** ⭐

---

<div align="center">

**[Documentation](./docs)** • **[Tests](./tests)** • **[Issues](../../issues)** • **[Discussions](../../discussions)**

Made with ❤️ and ☕ by sam0verfl0w

</div>
