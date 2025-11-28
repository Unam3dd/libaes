# 📖 Documentation Technique - LibAES

<div align="center">

**Guide Complet des Modes d'Opération AES et Cryptographie Authentifiée**

*Une exploration détaillée d'AES-GCM, CTR et des Corps de Galois*

</div>

---

## 📋 Table des Matières

1. [Introduction à AES](#-introduction-à-aes)
2. [Instructions AES-NI d'Intel](#-instructions-aes-ni-dintel)
   - [Vue d'Ensemble](#vue-densemble)
   - [Instructions de Chiffrement](#instructions-de-chiffrement)
   - [Instructions de Déchiffrement](#instructions-de-déchiffrement)
   - [Instructions d'Expansion de Clé](#instructions-dexpansion-de-clé)
   - [Instructions Auxiliaires](#instructions-auxiliaires)
3. [Les Modes d'Opération](#-les-modes-dopération)
   - [ECB - Electronic Codebook](#ecb---electronic-codebook-)
   - [CBC - Cipher Block Chaining](#cbc---cipher-block-chaining-)
   - [CTR - Counter Mode](#ctr---counter-mode-)
   - [OFB - Output Feedback](#ofb---output-feedback-)
   - [CFB - Cipher Feedback](#cfb---cipher-feedback-)
4. [AES-GCM en Profondeur](#-aes-gcm-en-profondeur)
   - [Qu'est-ce que GCM ?](#quest-ce-que-gcm-)
   - [Architecture de GCM](#architecture-de-gcm)
   - [Le Mode CTR dans GCM](#le-mode-ctr-dans-gcm)
   - [GHASH - La Fonction d'Authentification](#ghash---la-fonction-dauthentification)
5. [Mathématiques de GCM](#-mathématiques-de-gcm)
   - [Introduction aux Corps de Galois](#introduction-aux-corps-de-galois)
   - [Le Corps GF(2^128)](#le-corps-gf2128)
   - [Multiplication dans GF(2^128)](#multiplication-dans-gf2128)
   - [Réduction par Polynôme Irréductible](#réduction-par-polynôme-irréductible)
6. [Implémentation Optimisée](#-implémentation-optimisée)
7. [Exemples Pratiques](#-exemples-pratiques)
8. [Références](#-références)

---

## 🔐 Introduction à AES

### Qu'est-ce qu'AES ?

**AES (Advanced Encryption Standard)** est un algorithme de chiffrement par bloc standardisé par le NIST en 2001. Il remplace le DES et constitue la base de la sécurité moderne.

#### Caractéristiques

- 🎯 **Taille de bloc** : 128 bits (16 octets)
- 🔑 **Tailles de clé** : 128, 192 ou 256 bits
- 🔄 **Rounds** : 10, 12 ou 14 rounds selon la taille de clé
- ⚡ **Performances** : Très rapide avec AES-NI

#### Structure d'un Round AES

```
┌─────────────────────────────┐
│  1. SubBytes (S-Box)        │  Substitution non-linéaire
├─────────────────────────────┤
│  2. ShiftRows               │  Permutation des lignes
├─────────────────────────────┤
│  3. MixColumns              │  Diffusion (sauf dernier round)
├─────────────────────────────┤
│  4. AddRoundKey             │  XOR avec la round key
└─────────────────────────────┘
```

> **Note** : AES seul ne chiffre que des blocs de 128 bits. Pour chiffrer des messages plus longs, on utilise des **modes d'opération**.

---

## ⚡ Instructions AES-NI d'Intel

### Vue d'Ensemble

**AES-NI (Advanced Encryption Standard New Instructions)** est un jeu d'instructions introduit par Intel en 2010 (architecture Westmere) pour accélérer les opérations AES via le matériel.

#### Pourquoi AES-NI ?

| Implémentation Software | Implémentation AES-NI |
|------------------------|----------------------|
| ~100-200 cycles/bloc | ~10-20 cycles/bloc |
| Vulnérable aux timing attacks | Temps constant (résistant) |
| Utilise beaucoup de cache (S-Box) | Aucun accès mémoire |
| Code complexe | Quelques instructions |

**Gain de performance** : **5x à 10x plus rapide** ! ⚡

#### Les 6 Instructions AES-NI

| Instruction | Usage | Description |
|------------|-------|-------------|
| **AESENC** | Encryption | Un round AES (sauf le dernier) |
| **AESENCLAST** | Encryption | Dernier round AES |
| **AESDEC** | Decryption | Un round AES inverse (sauf le dernier) |
| **AESDECLAST** | Decryption | Dernier round AES inverse |
| **AESKEYGENASSIST** | Key Schedule | Aide à l'expansion de clé |
| **AESIMC** | Key Schedule | Transformation InvMixColumns |

#### Instructions Complémentaires

AES-NI est souvent utilisé avec d'autres instructions :

| Instruction | Set | Usage dans AES |
|------------|-----|----------------|
| **PCLMULQDQ** | CLMUL | Multiplication carry-less (GCM/GHASH) |
| **PSHUFB** | SSSE3 | Byte shuffle (endianness, S-Box) |
| **PXOR** | SSE2 | XOR de registres 128-bit |

---

### Instructions de Chiffrement

#### AESENC - AES Encrypt Round

**Signature** :
```c
__m128i _mm_aesenc_si128(__m128i data, __m128i round_key)
```

**Opération** : Effectue **un round complet AES** (sauf le dernier round) :

```
┌─────────────────────────────┐
│  1. ShiftRows               │
├─────────────────────────────┤
│  2. SubBytes                │
├─────────────────────────────┤
│  3. MixColumns              │
├─────────────────────────────┤
│  4. AddRoundKey (XOR)       │
└─────────────────────────────┘
```

**Détails** :

1. **ShiftRows** : Rotation circulaire des lignes de la state matrix
   ```
   Avant:          Après:
   [a0 a1 a2 a3]   [a0 a1 a2 a3]
   [b0 b1 b2 b3]   [b1 b2 b3 b0]  (décalage 1)
   [c0 c1 c2 c3]   [c2 c3 c0 c1]  (décalage 2)
   [d0 d1 d2 d3]   [d3 d0 d1 d2]  (décalage 3)
   ```

2. **SubBytes** : Substitution non-linéaire via S-Box
   - Chaque byte est remplacé par sa valeur dans la S-Box
   - Résistance aux attaques linéaires/différentielles

3. **MixColumns** : Multiplication matricielle dans GF(2^8)
   ```
   [02 03 01 01]   [s0]
   [01 02 03 01] × [s1]
   [01 01 02 03]   [s2]
   [03 01 01 02]   [s3]
   ```

4. **AddRoundKey** : XOR avec la round key

**Exemple d'utilisation** :
```c
__m128i aes_encrypt_block(__m128i plaintext, __m128i *round_keys, int num_rounds)
{
    __m128i state = plaintext;
    
    // Round initial : AddRoundKey seulement
    state = _mm_xor_si128(state, round_keys[0]);
    
    // Rounds 1 à (num_rounds - 1)
    for (int i = 1; i < num_rounds; i++) {
        state = _mm_aesenc_si128(state, round_keys[i]);  // 🔥 AESENC
    }
    
    // Dernier round (voir AESENCLAST)
    state = _mm_aesenclast_si128(state, round_keys[num_rounds]);
    
    return state;
}
```

**Performance** : 
- **Latence** : 4 cycles (Skylake+) / 7 cycles (Westmere)
- **Throughput** : 1 cycle (1 instruction/cycle en pipeline)

---

#### AESENCLAST - AES Encrypt Last Round

**Signature** :
```c
__m128i _mm_aesenclast_si128(__m128i data, __m128i round_key)
```

**Opération** : Effectue le **dernier round AES** (sans MixColumns) :

```
┌─────────────────────────────┐
│  1. ShiftRows               │
├─────────────────────────────┤
│  2. SubBytes                │
├─────────────────────────────┤
│  3. AddRoundKey (XOR)       │
└─────────────────────────────┘
```

> **Différence avec AESENC** : Le dernier round **n'inclut pas MixColumns** selon la spécification AES.

**Pourquoi pas de MixColumns au dernier round ?**

- MixColumns est une transformation réversible qui diffuse les bits
- Au dernier round, ce n'est pas nécessaire (et ça simplifie le déchiffrement)
- C'est une spécification du standard AES (FIPS 197)

**Exemple** :
```c
// Chiffrement AES-128 (10 rounds)
__m128i state = plaintext;

state = _mm_xor_si128(state, round_keys[0]);  // Round 0: AddRoundKey

for (int i = 1; i < 10; i++) {
    state = _mm_aesenc_si128(state, round_keys[i]);  // Rounds 1-9
}

state = _mm_aesenclast_si128(state, round_keys[10]);  // Round 10 (dernier)

return state;  // Ciphertext
```

**Structure complète** :
```
AES-128 : 10 rounds total
  ├─ Round  0: AddRoundKey uniquement
  ├─ Rounds 1-9: AESENC (ShiftRows + SubBytes + MixColumns + AddRoundKey)
  └─ Round 10: AESENCLAST (ShiftRows + SubBytes + AddRoundKey)

AES-192 : 12 rounds total
  ├─ Round  0: AddRoundKey uniquement
  ├─ Rounds 1-11: AESENC
  └─ Round 12: AESENCLAST

AES-256 : 14 rounds total
  ├─ Round  0: AddRoundKey uniquement
  ├─ Rounds 1-13: AESENC
  └─ Round 14: AESENCLAST
```

---

### Instructions de Déchiffrement

Le déchiffrement AES utilise les opérations **inverses** dans l'ordre inverse.

#### AESDEC - AES Decrypt Round

**Signature** :
```c
__m128i _mm_aesdec_si128(__m128i data, __m128i round_key)
```

**Opération** : Effectue **un round de déchiffrement AES** (sauf le dernier) :

```
┌─────────────────────────────┐
│  1. InvShiftRows            │
├─────────────────────────────┤
│  2. InvSubBytes             │
├─────────────────────────────┤
│  3. AddRoundKey (XOR)       │
├─────────────────────────────┤
│  4. InvMixColumns           │
└─────────────────────────────┘
```

**Détails** :

1. **InvShiftRows** : Rotation inverse (vers la droite)
   ```
   Avant:          Après:
   [a0 a1 a2 a3]   [a0 a1 a2 a3]
   [b1 b2 b3 b0]   [b0 b1 b2 b3]  (décalage -1)
   [c2 c3 c0 c1]   [c0 c1 c2 c3]  (décalage -2)
   [d3 d0 d1 d2]   [d0 d1 d2 d3]  (décalage -3)
   ```

2. **InvSubBytes** : Inverse de la S-Box
   - Utilise une S-Box inverse

3. **AddRoundKey** : XOR (identique au chiffrement)

4. **InvMixColumns** : Multiplication matricielle inverse
   ```
   [0e 0b 0d 09]   [s0]
   [09 0e 0b 0d] × [s1]
   [0d 09 0e 0b]   [s2]
   [0b 0d 09 0e]   [s3]
   ```

**Exemple** :
```c
__m128i aes_decrypt_block(__m128i ciphertext, __m128i *round_keys, int num_rounds)
{
    __m128i state = ciphertext;
    
    // Déchiffrement : on commence par la DERNIÈRE round key
    state = _mm_xor_si128(state, round_keys[num_rounds]);
    
    // Rounds num_rounds-1 à 1 (ordre inverse)
    for (int i = num_rounds - 1; i > 0; i--) {
        state = _mm_aesdec_si128(state, round_keys[i]);  // 🔥 AESDEC
    }
    
    // Dernier round
    state = _mm_aesdeclast_si128(state, round_keys[0]);
    
    return state;  // Plaintext récupéré
}
```

**Performance** : Identique à AESENC (4-7 cycles de latence)

---

#### AESDECLAST - AES Decrypt Last Round

**Signature** :
```c
__m128i _mm_aesdeclast_si128(__m128i data, __m128i round_key)
```

**Opération** : Effectue le **dernier round de déchiffrement** (sans InvMixColumns) :

```
┌─────────────────────────────┐
│  1. InvShiftRows            │
├─────────────────────────────┤
│  2. InvSubBytes             │
├─────────────────────────────┤
│  3. AddRoundKey (XOR)       │
└─────────────────────────────┘
```

**Exemple complet** :
```c
// Déchiffrement AES-256 (14 rounds)
__m128i state = ciphertext;

state = _mm_xor_si128(state, round_keys[14]);  // Round 14: AddRoundKey

for (int i = 13; i > 0; i--) {
    state = _mm_aesdec_si128(state, round_keys[i]);  // Rounds 13-1
}

state = _mm_aesdeclast_si128(state, round_keys[0]);  // Round 0 (dernier)

return state;  // Plaintext
```

---

### Instructions d'Expansion de Clé

L'expansion de clé (Key Schedule) transforme la clé initiale en plusieurs round keys.

#### AESKEYGENASSIST - AES Key Generation Assist

**Signature** :
```c
__m128i _mm_aeskeygenassist_si128(__m128i key, const int rcon)
```

**Opération** : Aide à générer les round keys en effectuant :

1. **SubBytes** sur certains bytes de la clé
2. **Rotation** de 32 bits
3. **XOR avec RCON** (Round Constant)

**Paramètres** :
- `key` : La clé courante (ou partie de clé)
- `rcon` : Round constant (0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36)

**Structure du résultat** :
```
Input:  [X3 X2 X1 X0] (4 dwords de 32 bits)

Output: [SubWord(X1)⊕RCON | SubWord(X1) | SubWord(X3)⊕RCON | SubWord(X3)]
         └─────────────────┴──────────────┴──────────────────┴──────────┘
            Dword 3           Dword 2         Dword 1          Dword 0
```

**SubWord** : Application de la S-Box à chaque byte d'un dword, puis rotation.

**Exemple - Expansion AES-128** :
```c
void aes_128_key_expansion(uint8_t *user_key, __m128i *key_schedule)
{
    // Charger la clé initiale
    __m128i key = _mm_loadu_si128((__m128i*)user_key);
    key_schedule[0] = key;
    
    // Round constants
    const int rcon[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
    
    // Générer les 10 round keys
    for (int i = 0; i < 10; i++) {
        // Générer le mot clé auxiliaire
        __m128i temp = _mm_aeskeygenassist_si128(key, rcon[i]);  // 🔥 AESKEYGENASSIST
        
        // Extraire le dword nécessaire
        temp = _mm_shuffle_epi32(temp, 0xFF);  // Broadcast dernier dword
        
        // XOR en cascade (Key Expansion standard)
        key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
        key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
        key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
        key = _mm_xor_si128(key, temp);
        
        key_schedule[i + 1] = key;
    }
}
```

**Visualisation** :
```
Key[0] = User Key (128 bits)

Round 1:
  temp = AESKEYGENASSIST(Key[0], 0x01)
  Key[1] = Key[0] ⊕ expanded(temp)

Round 2:
  temp = AESKEYGENASSIST(Key[1], 0x02)
  Key[2] = Key[1] ⊕ expanded(temp)

... (10 rounds pour AES-128)
```

**Round Constants (RCON)** :

Les RCON sont des constantes utilisées pour différencier chaque round :

| Round | RCON (hex) | RCON (binaire) |
|-------|-----------|----------------|
| 1 | 0x01 | 0b00000001 |
| 2 | 0x02 | 0b00000010 |
| 3 | 0x04 | 0b00000100 |
| 4 | 0x08 | 0b00001000 |
| 5 | 0x10 | 0b00010000 |
| 6 | 0x20 | 0b00100000 |
| 7 | 0x40 | 0b01000000 |
| 8 | 0x80 | 0b10000000 |
| 9 | 0x1B | 0b00011011 |
| 10 | 0x36 | 0b00110110 |

> **Note** : RCON est calculé via multiplication par x dans GF(2^8) avec réduction modulo x^8 + x^4 + x^3 + x + 1.

---

#### AESIMC - AES Inverse Mix Columns

**Signature** :
```c
__m128i _mm_aesimc_si128(__m128i round_key)
```

**Opération** : Applique la transformation **InvMixColumns** à une round key.

**Pourquoi cette instruction ?**

Pour le déchiffrement, on utilise les round keys **dans l'ordre inverse**. Mais il y a un problème : l'ordre des opérations dans AESDEC ne correspond pas exactement à l'inverse de AESENC.

**Solution** : Transformer les round keys intermédiaires avec InvMixColumns !

```
┌─────────────────────────────────────────────────┐
│  Round Keys pour Encryption (utilisées telles)  │
│  RK[0], RK[1], RK[2], ..., RK[N]                │
└─────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────┐
│  Round Keys pour Decryption                     │
│  RK[N], IMC(RK[N-1]), ..., IMC(RK[1]), RK[0]    │
│         └──────────────────┘                    │
│            AESIMC appliqué                      │
└─────────────────────────────────────────────────┘
```

**Exemple** :
```c
void prepare_decryption_keys(__m128i *enc_keys, __m128i *dec_keys, int num_rounds)
{
    // Première et dernière clé : identiques
    dec_keys[0] = enc_keys[num_rounds];
    dec_keys[num_rounds] = enc_keys[0];
    
    // Clés intermédiaires : appliquer InvMixColumns
    for (int i = 1; i < num_rounds; i++) {
        dec_keys[i] = _mm_aesimc_si128(enc_keys[num_rounds - i]);  // 🔥 AESIMC
    }
}
```

**Alternative** : On peut aussi appliquer AESIMC à la volée pendant le déchiffrement :
```c
// Méthode 1: Pré-calculer avec AESIMC
__m128i dec_key = _mm_aesimc_si128(enc_keys[i]);
state = _mm_aesdec_si128(state, dec_key);

// Méthode 2: Utiliser la clé d'encryption directement (plus lent)
// Nécessite des manipulations supplémentaires
```

> **Optimisation** : LibAES stocke les clés de déchiffrement pré-calculées pour de meilleures performances.

---

### Instructions Auxiliaires

#### PCLMULQDQ - Carry-Less Multiplication

**Signature** :
```c
__m128i _mm_clmulepi64_si128(__m128i a, __m128i b, const int imm8)
```

**Opération** : Multiplication **sans retenue** (carry-less) de deux polynômes de 64 bits.

**Utilisation dans AES** : Essentielle pour **GHASH** (mode GCM) !

**Paramètre imm8** :
```
imm8 = 0x00 : a[0:63]   × b[0:63]    (low × low)
imm8 = 0x01 : a[0:63]   × b[64:127]  (low × high)
imm8 = 0x10 : a[64:127] × b[0:63]    (high × low)
imm8 = 0x11 : a[64:127] × b[64:127]  (high × high)
```

**Exemple - Multiplication GF(2^128)** :
```c
void gf128_multiply(__m128i a, __m128i b, __m128i *result)
{
    // Multiplication 128×128 → 256 bits via 4 multiplications 64×64
    __m128i low_low   = _mm_clmulepi64_si128(a, b, 0x00);
    __m128i low_high  = _mm_clmulepi64_si128(a, b, 0x01);
    __m128i high_low  = _mm_clmulepi64_si128(a, b, 0x10);
    __m128i high_high = _mm_clmulepi64_si128(a, b, 0x11);
    
    // Combiner (Karatsuba)
    __m128i middle = _mm_xor_si128(low_high, high_low);
    // ... (voir section GF(2^128) pour la suite)
}
```

**Performance** : 
- **Latence** : 7 cycles (moderne)
- **Essentiel pour GCM** : Sans cette instruction, GHASH serait 10x plus lent !

---

#### PSHUFB - Packed Shuffle Bytes

**Signature** :
```c
__m128i _mm_shuffle_epi8(__m128i a, __m128i mask)
```

**Opération** : Réorganise les bytes selon un masque.

**Utilisation dans AES** :
- **Byte-swapping** (big-endian ↔ little-endian)
- **Implémentation de S-Box** en software
- **Manipulations de données** pour GCM

**Exemple - Byte Swap** :
```c
// Inverser l'ordre des bytes (utile pour GCM)
const __m128i BSWAP_MASK = _mm_set_epi8(
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
);

__m128i data = _mm_loadu_si128((__m128i*)input);
data = _mm_shuffle_epi8(data, BSWAP_MASK);  // Swap !
```

**Visualisation** :
```
Input:  [F E D C B A 9 8 7 6 5 4 3 2 1 0]
Mask:   [0 1 2 3 4 5 6 7 8 9 A B C D E F]
Output: [0 1 2 3 4 5 6 7 8 9 A B C D E F]
```

---

### Comparaison : Avec vs Sans AES-NI

#### Implémentation Software (Sans AES-NI)

```c
// SubBytes nécessite une table S-Box (256 bytes)
static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    // ... 248 autres valeurs
};

void aes_round_software(uint8_t state[16], uint8_t round_key[16])
{
    uint8_t temp[16];
    
    // 1. SubBytes (256 accès mémoire !)
    for (int i = 0; i < 16; i++) {
        temp[i] = sbox[state[i]];
    }
    
    // 2. ShiftRows (manipulations de bytes)
    uint8_t shifted[16];
    shifted[0] = temp[0]; shifted[1] = temp[5];  // ...
    // ... 14 autres assignments
    
    // 3. MixColumns (16 multiplications + XORs dans GF(2^8))
    for (int i = 0; i < 4; i++) {
        uint8_t a = shifted[4*i+0];
        uint8_t b = shifted[4*i+1];
        uint8_t c = shifted[4*i+2];
        uint8_t d = shifted[4*i+3];
        
        state[4*i+0] = gf_mul(a,2) ^ gf_mul(b,3) ^ c ^ d;
        state[4*i+1] = a ^ gf_mul(b,2) ^ gf_mul(c,3) ^ d;
        // ... (complexe)
    }
    
    // 4. AddRoundKey
    for (int i = 0; i < 16; i++) {
        state[i] ^= round_key[i];
    }
}

// Résultat : ~50-100 instructions, accès mémoire multiples, ~100-200 cycles
```

#### Implémentation AES-NI (Hardware)

```c
void aes_round_aesni(__m128i *state, __m128i round_key)
{
    *state = _mm_aesenc_si128(*state, round_key);  // 1 instruction !
}

// Résultat : 1 instruction, 0 accès mémoire, ~4-7 cycles
```

**Tableau Comparatif** :

| Métrique | Software | AES-NI | Gain |
|----------|----------|--------|------|
| **Instructions** | ~50-100 | 1 | 50-100x |
| **Cycles/round** | 100-200 | 4-7 | 15-50x |
| **Accès mémoire** | ~20+ | 0 | ∞ |
| **Cache utilisé** | 256 bytes (S-Box) | 0 | - |
| **Timing attacks** | Vulnérable | Résistant | ✅ |
| **Code size** | ~500 bytes | ~50 bytes | 10x |

---

### Détection du Support AES-NI

#### Vérification au Runtime

```c
#include <cpuid.h>

int has_aes_ni(void)
{
    unsigned int eax, ebx, ecx, edx;
    
    // CPUID avec EAX=1
    if (!__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
        return 0;
    }
    
    // AES-NI est le bit 25 de ECX
    return (ecx & (1 << 25)) != 0;
}

int has_pclmulqdq(void)
{
    unsigned int eax, ebx, ecx, edx;
    
    if (!__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
        return 0;
    }
    
    // PCLMULQDQ est le bit 1 de ECX
    return (ecx & (1 << 1)) != 0;
}
```

#### Flags de Compilation

```bash
# Activer AES-NI
gcc -maes -mpclmul -mssse3 -o program program.c

# Vérifier si le CPU supporte
grep -o 'aes' /proc/cpuinfo | head -1
grep -o 'pclmulqdq' /proc/cpuinfo | head -1
```

---

### Exemple Complet : Chiffrement AES-128 avec AES-NI

```c
#include <wmmintrin.h>  // AES-NI
#include <emmintrin.h>  // SSE2
#include <string.h>

// Expansion de clé AES-128
void aes_128_key_expansion(const uint8_t *user_key, __m128i *key_schedule)
{
    __m128i key = _mm_loadu_si128((const __m128i*)user_key);
    key_schedule[0] = key;
    
    const int rcon[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
    
    for (int i = 0; i < 10; i++) {
        __m128i temp = _mm_aeskeygenassist_si128(key, rcon[i]);
        temp = _mm_shuffle_epi32(temp, 0xFF);
        key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
        key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
        key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
        key = _mm_xor_si128(key, temp);
        key_schedule[i + 1] = key;
    }
}

// Chiffrement d'un bloc AES-128
__m128i aes_128_encrypt_block(__m128i plaintext, const __m128i *key_schedule)
{
    __m128i state = plaintext;
    
    // Round initial
    state = _mm_xor_si128(state, key_schedule[0]);
    
    // Rounds 1-9
    state = _mm_aesenc_si128(state, key_schedule[1]);
    state = _mm_aesenc_si128(state, key_schedule[2]);
    state = _mm_aesenc_si128(state, key_schedule[3]);
    state = _mm_aesenc_si128(state, key_schedule[4]);
    state = _mm_aesenc_si128(state, key_schedule[5]);
    state = _mm_aesenc_si128(state, key_schedule[6]);
    state = _mm_aesenc_si128(state, key_schedule[7]);
    state = _mm_aesenc_si128(state, key_schedule[8]);
    state = _mm_aesenc_si128(state, key_schedule[9]);
    
    // Round final
    state = _mm_aesenclast_si128(state, key_schedule[10]);
    
    return state;
}

// Déchiffrement d'un bloc AES-128
__m128i aes_128_decrypt_block(__m128i ciphertext, const __m128i *key_schedule)
{
    __m128i state = ciphertext;
    
    // Round initial (dernière clé)
    state = _mm_xor_si128(state, key_schedule[10]);
    
    // Rounds 9-1 (avec clés transformées par AESIMC)
    state = _mm_aesdec_si128(state, _mm_aesimc_si128(key_schedule[9]));
    state = _mm_aesdec_si128(state, _mm_aesimc_si128(key_schedule[8]));
    state = _mm_aesdec_si128(state, _mm_aesimc_si128(key_schedule[7]));
    state = _mm_aesdec_si128(state, _mm_aesimc_si128(key_schedule[6]));
    state = _mm_aesdec_si128(state, _mm_aesimc_si128(key_schedule[5]));
    state = _mm_aesdec_si128(state, _mm_aesimc_si128(key_schedule[4]));
    state = _mm_aesdec_si128(state, _mm_aesimc_si128(key_schedule[3]));
    state = _mm_aesdec_si128(state, _mm_aesimc_si128(key_schedule[2]));
    state = _mm_aesdec_si128(state, _mm_aesimc_si128(key_schedule[1]));
    
    // Round final (première clé, sans AESIMC)
    state = _mm_aesdeclast_si128(state, key_schedule[0]);
    
    return state;
}

// Programme principal
int main(void)
{
    // Clé AES-128
    uint8_t key[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    
    // Plaintext
    uint8_t plaintext[16] = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a
    };
    
    // Expansion de clé
    __m128i key_schedule[11];
    aes_128_key_expansion(key, key_schedule);
    
    // Chiffrement
    __m128i plain = _mm_loadu_si128((__m128i*)plaintext);
    __m128i cipher = aes_128_encrypt_block(plain, key_schedule);
    
    // Déchiffrement
    __m128i decrypted = aes_128_decrypt_block(cipher, key_schedule);
    
    // Vérification
    uint8_t result[16];
    _mm_storeu_si128((__m128i*)result, decrypted);
    
    if (memcmp(plaintext, result, 16) == 0) {
        printf("✅ Success!\n");
    } else {
        printf("❌ Failed!\n");
    }
    
    return 0;
}
```

**Compilation** :
```bash
gcc -maes -O3 -o aes_demo aes_demo.c
./aes_demo
```

---

### Résumé des Instructions AES-NI

| Instruction | Input | Output | Usage | Cycles |
|------------|-------|--------|-------|--------|
| **AESENC** | state, round_key | state | Round encryption (1-9/11/13) | 4-7 |
| **AESENCLAST** | state, round_key | state | Round final encryption (10/12/14) | 4-7 |
| **AESDEC** | state, round_key | state | Round decryption (9-1/11-1/13-1) | 4-7 |
| **AESDECLAST** | state, round_key | state | Round final decryption (0) | 4-7 |
| **AESKEYGENASSIST** | key, rcon | temp | Aide expansion de clé | 10 |
| **AESIMC** | round_key | dec_key | Transformation pour déchiffrement | 14 |
| **PCLMULQDQ** | a, b, imm | product | Multiplication carry-less (GCM) | 7 |

---

### Points Clés à Retenir 🎯

✅ **AES-NI** accélère AES de **5-10x**  
✅ **AESENC/AESENCLAST** : un round AES en 1 instruction  
✅ **AESDEC/AESDECLAST** : déchiffrement symétrique  
✅ **AESKEYGENASSIST** : simplifie l'expansion de clé  
✅ **AESIMC** : prépare les clés pour le déchiffrement  
✅ **PCLMULQDQ** : essentiel pour GCM/GHASH  
✅ **Temps constant** : résistant aux timing attacks  
✅ **Zero cache** : aucun accès mémoire = rapide et sûr  

---

## 🔄 Les Modes d'Opération

Les modes d'opération définissent **comment utiliser AES** pour chiffrer des messages de taille arbitraire.

### ECB - Electronic Codebook 📗

#### Principe

Le mode le plus simple : chaque bloc est chiffré indépendamment avec la même clé.

```
Plaintext:  [Block 1] [Block 2] [Block 3] [Block 4]
               ↓          ↓          ↓          ↓
            AES(K)     AES(K)     AES(K)     AES(K)
               ↓          ↓          ↓          ↓
Ciphertext: [Block 1] [Block 2] [Block 3] [Block 4]
```

#### Formule

```
C_i = E_K(P_i)
P_i = D_K(C_i)
```

#### ⚠️ Problèmes

- **Patterns visibles** : Blocs identiques → chiffrés identiques
- **Pas de diffusion** entre les blocs
- **Non recommandé** pour des données > 1 bloc

#### ✅ Usage Légitime

- Chiffrement de clés aléatoires
- Données strictement aléatoires
- Vecteurs d'initialisation

---

### CBC - Cipher Block Chaining 🔗

#### Principe

Chaque bloc de plaintext est **XORé avec le bloc chiffré précédent** avant chiffrement.

```
     IV
      ↓
P_1 → ⊕ → AES(K) → C_1
             ↓
P_2 → ⊕ → AES(K) → C_2
             ↓
P_3 → ⊕ → AES(K) → C_3
```

#### Formule

```
Chiffrement:
  C_0 = IV
  C_i = E_K(P_i ⊕ C_{i-1})

Déchiffrement:
  P_i = D_K(C_i) ⊕ C_{i-1}
```

#### ✅ Avantages

- Blocs identiques → chiffrés différemment
- Sécurité prouvée (mode standard)
- Support du padding PKCS#7

#### ⚠️ Inconvénients

- **Séquentiel** au chiffrement (pas de parallélisation)
- Erreur se propage au bloc suivant
- Nécessite un IV aléatoire

#### 🎯 Usage

- Chiffrement de fichiers
- Transmission de données bulk
- Mode par défaut pour beaucoup d'applications

---

### CTR - Counter Mode 🔢

#### Principe

Transforme AES en **chiffrement par flux** (stream cipher). On chiffre des **compteurs** et on XOR avec le plaintext.

```
Counter 1 → AES(K) → Keystream_1 ⊕ P_1 → C_1
Counter 2 → AES(K) → Keystream_2 ⊕ P_2 → C_2
Counter 3 → AES(K) → Keystream_3 ⊕ P_3 → C_3
Counter 4 → AES(K) → Keystream_4 ⊕ P_4 → C_4
```

#### Structure du Compteur

```
┌────────────────┬────────────┐
│  Nonce (96b)   │ Counter(32)│
│   (Aléatoire)  │ (1,2,3...) │
└────────────────┴────────────┘
      12 octets      4 octets
```

#### Formule

```
Chiffrement:
  Counter_i = Nonce || i
  C_i = P_i ⊕ E_K(Counter_i)

Déchiffrement:
  P_i = C_i ⊕ E_K(Counter_i)
```

> **Important** : Chiffrement et déchiffrement utilisent la **même opération** !

#### ✅ Avantages

- **Parallélisable** complètement (encryption ET decryption)
- Pas de padding nécessaire
- Accès aléatoire aux blocs possible
- Erreur ne se propage pas
- Déchiffrement = chiffrement (symétrique)

#### ⚠️ Inconvénients

- **Ne JAMAIS réutiliser** (Nonce, Key)
- Pas d'authentification
- Sensible aux attaques par manipulation

#### 🎯 Usage

- Disques chiffrés (dm-crypt, BitLocker)
- IPsec
- Base du mode GCM
- Streaming de données

---

### OFB - Output Feedback 🔁

#### Principe

Le **output** de AES est réinjecté comme input. On XOR avec le plaintext.

```
IV → AES(K) → O_1 ⊕ P_1 → C_1
       ↓
    AES(K) → O_2 ⊕ P_2 → C_2
       ↓
    AES(K) → O_3 ⊕ P_3 → C_3
```

#### Formule

```
O_0 = IV
O_i = E_K(O_{i-1})
C_i = P_i ⊕ O_i
```

#### ✅ Avantages

- Mode stream cipher
- Erreur de transmission ne se propage pas
- Pas de padding

#### ⚠️ Inconvénients

- Séquentiel (pas de parallélisation)
- Peu utilisé en pratique

---

### CFB - Cipher Feedback 🔄

#### Principe

Le **ciphertext** précédent est réinjecté dans AES.

```
IV → AES(K) → ⊕ → C_1
       ↑      P_1
       │
    C_1 → AES(K) → ⊕ → C_2
                   P_2
```

#### Formule

```
C_0 = IV
C_i = P_i ⊕ E_K(C_{i-1})
```

#### ✅ Avantages

- Mode stream cipher
- Auto-synchronisation possible
- Pas de padding

---

## 🛡️ AES-GCM en Profondeur

### Qu'est-ce que GCM ?

**GCM (Galois/Counter Mode)** est un mode d'opération **AEAD** (Authenticated Encryption with Associated Data) qui combine :

1. 🔒 **Chiffrement** via CTR mode
2. 🔐 **Authentification** via GHASH (Galois Hash)

```
┌────────────────────────────────────┐
│          AES-GCM                   │
├──────────────────┬─────────────────┤
│   CTR Mode       │     GHASH       │
│  (Encryption)    │ (Authentication)│
└──────────────────┴─────────────────┘
```

#### Pourquoi GCM ?

| Problème | Solution GCM |
|----------|--------------|
| CBC/CTR ne détectent pas les modifications | Tag d'authentification |
| Chiffrement + MAC séparés = lent | Combiné et optimisé |
| AAD impossible avec modes classiques | Support natif AAD |
| Performances | Parallélisable + CLMUL |

---

### Architecture de GCM

#### Vue d'Ensemble

```
Inputs:
  - Plaintext (P)
  - AAD (Additional Authenticated Data)
  - Key (K)
  - IV/Nonce (96 bits)

         ┌──────────────────────────────┐
         │       AES-GCM Engine         │
         └──────────────────────────────┘
                    ↓
         ┌──────────┴──────────┐
         ↓                     ↓
    CTR Mode              GHASH
    (Encrypt)          (Authenticate)
         ↓                     ↓
    Ciphertext (C)        Tag (T)

Outputs:
  - Ciphertext (C)
  - Authentication Tag (T)
```

#### Les Étapes de GCM

1. **Génération de H (Hash Subkey)**
   ```
   H = E_K(0^128)
   ```

2. **Construction de J0 (Initial Counter Block)**
   ```
   J0 = IV || 0x00000001  (pour IV de 96 bits)
   ```

3. **Chiffrement CTR** (compteur commence à J1 = J0 + 1)
   ```
   C_i = P_i ⊕ E_K(inc(J_i))
   ```

4. **Calcul GHASH**
   ```
   GHASH_H(AAD || C || len(AAD) || len(C))
   ```

5. **Tag Final**
   ```
   T = GHASH ⊕ E_K(J0)
   ```

---

### Le Mode CTR dans GCM

#### Structure du Compteur dans GCM

GCM utilise une variante spécifique de CTR :

```
┌─────────────────────────────────────────┐
│  IV (96 bits)       │  Counter (32 bits)│
│  0xcafebabe...      │    0x00000001     │
└─────────────────────────────────────────┘
   12 octets (fixe)      4 octets (incrémenté)
```

#### Le J0 Initial

**J0** est le compteur initial utilisé **uniquement pour le tag** :

```
J0 = IV || 0x00000001
```

> ⚠️ **Critique** : J0 n'est JAMAIS utilisé pour chiffrer les données !

#### Chiffrement des Données

Les données sont chiffrées avec **J1, J2, J3, ...** :

```
J1 = IV || 0x00000002  → chiffre Block 1
J2 = IV || 0x00000003  → chiffre Block 2
J3 = IV || 0x00000004  → chiffre Block 3
...
```

#### Algorithme Complet

```c
// 1. Calculer H (hash subkey)
H = AES_Encrypt(K, 0x00...00)

// 2. Construire J0
J0 = IV || 0x00000001

// 3. Calculer E(K, J0) pour le tag (mais ne pas l'utiliser pour les données!)
E_K_J0 = AES_Encrypt(K, J0)

// 4. Incrémenter à J1 pour commencer le chiffrement
counter = J0
counter[15] += 1  // Devient J1

// 5. Chiffrer les données avec CTR mode
for i in 0..num_blocks:
    keystream_block = AES_Encrypt(K, counter)
    ciphertext[i] = plaintext[i] XOR keystream_block
    counter[15] += 1  // J2, J3, J4...

// 6. Calculer GHASH
ghash = GHASH_H(AAD, ciphertext, len(AAD), len(ciphertext))

// 7. Tag final
tag = ghash XOR E_K_J0
```

#### Gestion de l'Endianness

Le compteur est en **big-endian** selon la spec NIST :

```c
// Incrémenter en big-endian
counter[15] = (counter[15] + 1) & 0xFF
if (counter[15] == 0) {
    counter[14] = (counter[14] + 1) & 0xFF
    // ... carry propagation
}

// Ou avec intrinsics x86:
uint32_t *cnt = (uint32_t*)(counter + 12);
*cnt = _bswap(*cnt);  // little → big
(*cnt)++;
*cnt = _bswap(*cnt);  // big → little

// Ou directement en big-endian:
*cnt += 0x01000000;  // Incrément du byte le plus significatif
```

---

### GHASH - La Fonction d'Authentification

#### Qu'est-ce que GHASH ?

GHASH est une **fonction de hachage universelle** qui opère dans le corps de Galois GF(2^128). Elle prend en entrée :

- Les données supplémentaires authentifiées (**AAD**)
- Le texte chiffré (**C**)
- Les longueurs de AAD et C

#### Construction de l'Input GHASH

```
Input = AAD || pad(AAD) || C || pad(C) || len(AAD) || len(C)
```

Où :
- `pad(X)` = ajoute des zéros pour aligner sur 128 bits
- `len(AAD)` = longueur en bits (64 bits)
- `len(C)` = longueur en bits (64 bits)

#### Exemple Visuel

```
AAD = "Hello" (5 bytes = 40 bits)
C = "Encrypted Data" (14 bytes = 112 bits)

Input GHASH:
┌──────────────────────────┐
│ AAD: "Hello"             │  40 bits
├──────────────────────────┤
│ Padding: 0x00...         │  88 bits (pour aligner à 128)
├──────────────────────────┤
│ C: "Encrypted Data"      │  112 bits
├──────────────────────────┤
│ Padding: 0x00...         │  16 bits (pour aligner à 128)
├──────────────────────────┤
│ len(AAD): 0x0000...0028  │  64 bits (40 en hex)
├──────────────────────────┤
│ len(C): 0x0000...0070    │  64 bits (112 en hex)
└──────────────────────────┘
```

#### Algorithme GHASH

GHASH est défini comme une série de multiplications dans GF(2^128) :

```
GHASH_H(X_1, X_2, ..., X_m) = Y_m

où:
  Y_0 = 0
  Y_i = (Y_{i-1} ⊕ X_i) • H    pour i = 1..m

• représente la multiplication dans GF(2^128)
```

#### Pseudo-Code

```python
def GHASH(H, data):
    """
    H: Hash subkey (128 bits)
    data: Données à authentifier (multiple de 128 bits)
    """
    Y = 0
    
    # Découper data en blocs de 128 bits
    blocks = split_into_128bit_blocks(data)
    
    for block in blocks:
        Y = Y XOR block
        Y = gf_mul(Y, H)  # Multiplication dans GF(2^128)
    
    return Y
```

#### Implémentation Réelle

```c
__m128i compute_ghash(__m128i hash_subkey, 
                       const uint8_t *aad, size_t aad_len,
                       const uint8_t *ct, size_t ct_len)
{
    __m128i ghash = _mm_setzero_si128();
    uint8_t block[16] = {0};
    
    // 1. Traiter AAD
    size_t aad_blocks = aad_len / 16;
    for (size_t i = 0; i < aad_blocks; i++) {
        __m128i aad_block = _mm_loadu_si128((__m128i*)(aad + i*16));
        ghash = _mm_xor_si128(ghash, aad_block);
        gfmul((uint8_t*)&ghash, (uint8_t*)&hash_subkey, (uint8_t*)&ghash);
    }
    
    // AAD partiel (padding)
    size_t aad_remainder = aad_len % 16;
    if (aad_remainder > 0) {
        memset(block, 0, 16);
        memcpy(block, aad + aad_blocks*16, aad_remainder);
        __m128i partial = _mm_loadu_si128((__m128i*)block);
        ghash = _mm_xor_si128(ghash, partial);
        gfmul((uint8_t*)&ghash, (uint8_t*)&hash_subkey, (uint8_t*)&ghash);
    }
    
    // 2. Traiter Ciphertext (même logique)
    // ...
    
    // 3. Ajouter les longueurs
    uint64_t aad_bits = aad_len * 8;
    uint64_t ct_bits = ct_len * 8;
    
    // Format: len(AAD) || len(C) en big-endian
    __m128i lengths = _mm_set_epi64x(
        __builtin_bswap64(ct_bits),   // len(C)
        __builtin_bswap64(aad_bits)   // len(AAD)
    );
    
    ghash = _mm_xor_si128(ghash, lengths);
    gfmul((uint8_t*)&ghash, (uint8_t*)&hash_subkey, (uint8_t*)&ghash);
    
    return ghash;
}
```

---

## 🧮 Mathématiques de GCM

### Introduction aux Corps de Galois

#### Qu'est-ce qu'un Corps de Galois ?

Un **corps de Galois** (ou corps fini) GF(p^n) est une structure algébrique où on peut faire des additions, soustractions, multiplications et divisions (sauf par 0).

#### Pourquoi GF(2^128) pour GCM ?

- **128 bits** = taille d'un bloc AES
- **Base 2** = opérations binaires (XOR, AND, shifts)
- **Propriétés crypto** : sécurité prouvée pour l'authentification

#### Structure de GF(2^128)

Les éléments de GF(2^128) sont des **polynômes de degré < 128** avec coefficients dans GF(2) :

```
a(x) = a_127·x^127 + a_126·x^126 + ... + a_1·x + a_0

où chaque a_i ∈ {0, 1}
```

**Représentation** : 128 bits → 1 élément de GF(2^128)

```
Bits:  1 0 1 1 0 0 1 0 ... (128 bits)
       ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓
Poly:  x^7 + x^5 + x^4 + x
```

---

### Le Corps GF(2^128)

#### Addition dans GF(2^128)

L'addition est simplement le **XOR** bit à bit :

```
a(x) + b(x) = (a_127 ⊕ b_127)·x^127 + ... + (a_0 ⊕ b_0)

Exemple:
  1011 (x^3 + x + 1)
⊕ 1101 (x^3 + x^2 + 1)
──────
  0110 (x^2 + x)
```

**Implémentation** :
```c
__m128i a = _mm_loadu_si128((__m128i*)data_a);
__m128i b = _mm_loadu_si128((__m128i*)data_b);
__m128i sum = _mm_xor_si128(a, b);  // Addition dans GF(2^128)
```

---

### Multiplication dans GF(2^128)

#### Principe

La multiplication de polynômes dans GF(2^128) se fait en **deux étapes** :

1. **Multiplication polynomiale** (carry-less)
2. **Réduction modulo polynôme irréductible**

#### Étape 1 : Multiplication Carry-less

Multiplication de polynômes **sans retenue** :

```
Exemple simple:
  a(x) = x^2 + x       (110 en binaire)
  b(x) = x + 1         (011 en binaire)

Multiplication polynomiale:
       110
     × 011
     ─────
       110   (110 × 1)
      110    (110 × x, décalé de 1)
     ─────
     1010   (XOR, car coefficients dans GF(2))

Résultat: x^3 + x = 1010
```

#### Méthode Naïve (Bit par Bit)

```c
void poly_mult_naive(uint8_t a[16], uint8_t b[16], uint8_t result[32])
{
    uint8_t temp[32] = {0};
    
    // Pour chaque bit de a
    for (int i = 0; i < 128; i++) {
        if (get_bit(a, i)) {
            // Ajouter b décalé de i positions
            xor_shifted(temp, b, i);
        }
    }
    
    memcpy(result, temp, 32);
}
```

> **Problème** : Cette méthode est **très lente** (128 itérations) !

#### Méthode Optimisée : PCLMULQDQ

Intel fournit l'instruction **PCLMULQDQ** (Carry-Less Multiplication) qui fait exactement ça en hardware !

```c
__m128i clmul_full(__m128i a, __m128i b)
{
    // PCLMULQDQ fait la multiplication carry-less de deux polynômes de 64 bits
    __m128i low_low   = _mm_clmulepi64_si128(a, b, 0x00);  // a[0:63] × b[0:63]
    __m128i low_high  = _mm_clmulepi64_si128(a, b, 0x01);  // a[0:63] × b[64:127]
    __m128i high_low  = _mm_clmulepi64_si128(a, b, 0x10);  // a[64:127] × b[0:63]
    __m128i high_high = _mm_clmulepi64_si128(a, b, 0x11);  // a[64:127] × b[64:127]
    
    // Combiner les résultats (méthode Karatsuba)
    __m128i middle = _mm_xor_si128(low_high, high_low);
    __m128i middle_low  = _mm_slli_si128(middle, 8);
    __m128i middle_high = _mm_srli_si128(middle, 8);
    
    low_low = _mm_xor_si128(low_low, middle_low);
    high_high = _mm_xor_si128(high_high, middle_high);
    
    // Résultat sur 256 bits (mais on ne retourne que 128 bits hauts/bas)
    return ...; // Voir implémentation complète ci-dessous
}
```

#### Résultat Intermédiaire

Après multiplication carry-less de deux polynômes de 128 bits, on obtient un polynôme de **degré ≤ 254** (donc 255 bits max).

```
a(x) · b(x) = c(x)  où degré(c) ≤ 254
```

Ce résultat doit être **réduit** pour rentrer dans GF(2^128).

---

### Réduction par Polynôme Irréductible

#### Le Polynôme Irréductible de GCM

GCM utilise le polynôme irréductible suivant :

```
R(x) = x^128 + x^7 + x^2 + x + 1
```

En binaire (129 bits) :
```
1 0000...0000 10000111
↑              ↑  ↑ ↑ ↑
128           7  2 1 0
```

#### Pourquoi Irréductible ?

Un polynôme est **irréductible** s'il ne peut pas être factorisé (comme un nombre premier). Cela garantit que chaque élément non-nul de GF(2^128) a un inverse multiplicatif.

#### Principe de la Réduction

On veut réduire un polynôme de degré ≤ 254 modulo R(x) :

```
c(x) mod R(x)
```

**Équivalence** : 
```
x^128 ≡ x^7 + x^2 + x + 1  (mod R(x))
```

Donc si on a un terme x^k avec k ≥ 128, on peut le remplacer :
```
x^k = x^{k-128} · x^128 
    ≡ x^{k-128} · (x^7 + x^2 + x + 1)
```

#### Algorithme de Réduction

```python
def reduce_mod_R(c):
    """
    c: polynôme de 256 bits (résultat de la multiplication)
    R = x^128 + x^7 + x^2 + x + 1
    
    Retourne: c mod R (128 bits)
    """
    # Séparer c en deux parties
    c_high = c[128:256]  # Bits 128-255
    c_low = c[0:128]     # Bits 0-127
    
    # c_high contient les termes x^128 et plus
    # On doit les réduire
    
    for i in range(127, -1, -1):
        if bit(c_high, i):
            # x^(128+i) ≡ x^i · (x^7 + x^2 + x + 1)
            c_low ^= (1 << i)      # x^i
            if i + 7 < 128:
                c_low ^= (1 << (i+7))  # x^(i+7)
            if i + 2 < 128:
                c_low ^= (1 << (i+2))  # x^(i+2)
            if i + 1 < 128:
                c_low ^= (1 << (i+1))  # x^(i+1)
    
    return c_low
```

#### Réduction Optimisée (Intel)

Intel fournit une méthode optimisée utilisant des shifts et XORs :

```c
__m128i gf128_reduce(__m128i high, __m128i low)
{
    // high: 128 bits hauts du produit (terme x^128 et plus)
    // low: 128 bits bas du produit
    
    // Étape 1: Réduire les bits 128-255
    __m128i tmp = _mm_srli_epi32(high, 31);
    __m128i tmp2 = _mm_srli_epi32(high, 30);
    __m128i tmp3 = _mm_srli_epi32(high, 25);
    
    // XOR des réductions pour x^7, x^2, x
    tmp = _mm_xor_si128(tmp, tmp2);
    tmp = _mm_xor_si128(tmp, tmp3);
    
    // Aligner et appliquer au low
    tmp2 = _mm_slli_si128(tmp, 4);
    low = _mm_xor_si128(low, tmp2);
    
    // Étape 2: Continuer la réduction
    tmp = _mm_srli_epi32(low, 1);
    tmp2 = _mm_srli_epi32(low, 2);
    tmp3 = _mm_srli_epi32(low, 7);
    
    tmp = _mm_xor_si128(tmp, tmp2);
    tmp = _mm_xor_si128(tmp, tmp3);
    
    return _mm_xor_si128(low, tmp);
}
```

---

### Implémentation Complète de GFMUL

Voici l'implémentation complète de la multiplication dans GF(2^128) utilisée dans LibAES :

```c
void gfmul(const uint8_t a[16], const uint8_t b[16], uint8_t r[16])
{
    // Masque pour byte-swap (GCM utilise big-endian)
    const __m128i BSWAP_MASK = _mm_set_epi8(
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    );
    
    // Charger les opérandes
    __m128i xmm0 = _mm_loadu_si128((const __m128i*)a);
    __m128i xmm1 = _mm_loadu_si128((const __m128i*)b);
    
    // Byte-swap pour big-endian
    xmm0 = _mm_shuffle_epi8(xmm0, BSWAP_MASK);
    xmm1 = _mm_shuffle_epi8(xmm1, BSWAP_MASK);
    
    // ═══════════════════════════════════════════════════
    // PARTIE 1: MULTIPLICATION CARRY-LESS (256 bits)
    // ═══════════════════════════════════════════════════
    
    // Multiplier les 4 combinaisons de 64-bit chunks
    __m128i tmp3 = _mm_clmulepi64_si128(xmm0, xmm1, 0x00); // a_low × b_low
    __m128i tmp4 = _mm_clmulepi64_si128(xmm0, xmm1, 0x10); // a_high × b_low
    __m128i tmp5 = _mm_clmulepi64_si128(xmm0, xmm1, 0x01); // a_low × b_high
    __m128i tmp6 = _mm_clmulepi64_si128(xmm0, xmm1, 0x11); // a_high × b_high
    
    // Combiner les parties moyennes (Karatsuba)
    tmp4 = _mm_xor_si128(tmp4, tmp5);
    tmp5 = _mm_slli_si128(tmp4, 8);  // Décaler à gauche
    tmp4 = _mm_srli_si128(tmp4, 8);  // Décaler à droite
    tmp3 = _mm_xor_si128(tmp3, tmp5); // low += middle_low
    tmp6 = _mm_xor_si128(tmp6, tmp4); // high += middle_high
    
    // Maintenant: tmp3 = 128 bits bas, tmp6 = 128 bits hauts
    
    // ═══════════════════════════════════════════════════
    // PARTIE 2: RÉDUCTION MODULO R(x) = x^128+x^7+x^2+x+1
    // ═══════════════════════════════════════════════════
    
    // Étape 1: Shifter et aligner
    __m128i tmp7 = _mm_srli_epi32(tmp3, 31);
    __m128i tmp8 = _mm_srli_epi32(tmp6, 31);
    tmp3 = _mm_slli_epi32(tmp3, 1);
    tmp6 = _mm_slli_epi32(tmp6, 1);
    
    __m128i tmp9 = _mm_srli_si128(tmp7, 12);
    tmp8 = _mm_slli_si128(tmp8, 4);
    tmp7 = _mm_slli_si128(tmp7, 4);
    tmp3 = _mm_or_si128(tmp3, tmp7);
    tmp6 = _mm_or_si128(tmp6, tmp8);
    tmp6 = _mm_or_si128(tmp6, tmp9);
    
    // Étape 2: Appliquer la réduction (x^128 → x^7 + x^2 + x + 1)
    tmp7 = _mm_slli_epi32(tmp3, 31); // Pour x^7
    tmp8 = _mm_slli_epi32(tmp3, 30); // Pour x^2
    tmp9 = _mm_slli_epi32(tmp3, 25); // Pour x + 1
    
    tmp7 = _mm_xor_si128(tmp7, tmp8);
    tmp7 = _mm_xor_si128(tmp7, tmp9);
    tmp8 = _mm_srli_si128(tmp7, 4);
    tmp7 = _mm_slli_si128(tmp7, 12);
    tmp3 = _mm_xor_si128(tmp3, tmp7);
    
    // Étape 3: Finaliser la réduction
    __m128i tmp2 = _mm_srli_epi32(tmp3, 1);
    tmp4 = _mm_srli_epi32(tmp3, 2);
    tmp5 = _mm_srli_epi32(tmp3, 7);
    tmp2 = _mm_xor_si128(tmp2, tmp4);
    tmp2 = _mm_xor_si128(tmp2, tmp5);
    tmp2 = _mm_xor_si128(tmp2, tmp8);
    tmp3 = _mm_xor_si128(tmp3, tmp2);
    tmp6 = _mm_xor_si128(tmp6, tmp3);
    
    // Byte-swap de retour pour output
    tmp6 = _mm_shuffle_epi8(tmp6, BSWAP_MASK);
    
    // Stocker le résultat
    _mm_storeu_si128((__m128i*)r, tmp6);
}
```

#### Explication Détaillée

**Partie 1 : Multiplication** (lignes de `tmp3` à `tmp6`)
- On utilise **PCLMULQDQ** pour multiplier deux polynômes de 128 bits
- Résultat intermédiaire : 256 bits (tmp6:tmp3)

**Partie 2 : Réduction** (reste du code)
- On réduit les 256 bits modulo R(x) = x^128 + x^7 + x^2 + x + 1
- Utilise des shifts et XORs pour implémenter la réduction
- Résultat final : 128 bits

---

## 💡 Exemple Complet : Multiplication GF(2^128)

### Cas Simple

```
Entrée:
  a = 0x00000000000000000000000000000002  (x)
  b = 0x00000000000000000000000000000003  (x + 1)

Multiplication polynomiale:
  x · (x + 1) = x^2 + x

Résultat:
  c = 0x00000000000000000000000000000006  (x^2 + x)
```

### Cas avec Réduction

```
Entrée:
  a = 0x80000000000000000000000000000000  (x^127)
  b = 0x00000000000000000000000000000002  (x)

Multiplication:
  x^127 · x = x^128

Réduction (x^128 ≡ x^7 + x^2 + x + 1):
  x^128 mod R(x) = x^7 + x^2 + x + 1
                 = 0x00000000000000000000000000000087
```

---

## ⚡ Implémentation Optimisée

### Pipeline GCM Complet

```c
aes_status_t aes_gcm_enc(aes_gcm_counter_t *out, 
                          const iv_t nonce,
                          const byte_t *aad, size_t aad_len,
                          const byte_t *in, size_t i_sz,
                          const aes_ctx_t *ctx)
{
    // ═══════════════════════════════════════
    // PHASE 1: INITIALISATION
    // ═══════════════════════════════════════
    
    // 1.1 Créer Hash Subkey H = E_K(0)
    __m128i hash_subkey = _mm_setzero_si128();
    hash_subkey = aes_block_enc(hash_subkey, &ctx->key, NR);
    
    // 1.2 Créer J0 = IV || 0x00000001
    byte_t nonce_copy[16];
    memcpy(nonce_copy, nonce, 16);
    uint32_t *cnt = (uint32_t *)(nonce_copy + 0xC);
    
    // 1.3 Calculer E(K, J0) pour le tag final
    __m128i j0 = _mm_loadu_si128((__m128i*)nonce_copy);
    __m128i j0_encrypted = aes_block_enc(j0, &ctx->key, NR);
    
    // 1.4 Incrémenter à J1 pour le chiffrement
    *cnt += 0x01000000;  // Big-endian increment
    
    // ═══════════════════════════════════════
    // PHASE 2: CHIFFREMENT CTR
    // ═══════════════════════════════════════
    
    size_t blocks = (i_sz + 15) / 16;
    
    for (size_t i = 0; i < blocks; i++) {
        // Prefetch pour optimiser le cache
        _mm_prefetch(in + (i+1)*16, _MM_HINT_T0);
        
        // Charger plaintext
        __m128i plaintext = _mm_loadu_si128(((__m128i*)in) + i);
        
        // Générer keystream avec J_i
        __m128i counter = _mm_loadu_si128((__m128i*)nonce_copy);
        __m128i keystream = aes_block_enc(counter, &ctx->key, NR);
        
        // XOR pour chiffrer
        __m128i ciphertext = _mm_xor_si128(plaintext, keystream);
        _mm_storeu_si128(((__m128i*)out->out) + i, ciphertext);
        
        // Incrémenter compteur
        *cnt += 0x01000000;
    }
    
    // ═══════════════════════════════════════
    // PHASE 3: CALCUL GHASH
    // ═══════════════════════════════════════
    
    __m128i ghash = compute_ghash(hash_subkey, aad, aad_len, 
                                   out->out, i_sz);
    
    // ═══════════════════════════════════════
    // PHASE 4: TAG FINAL
    // ═══════════════════════════════════════
    
    out->tag = _mm_xor_si128(ghash, j0_encrypted);
    
    return AES_OK;
}
```

### Optimisations Clés

1. **AES-NI** : Hardware acceleration pour AES
2. **PCLMULQDQ** : Hardware multiplication dans GF(2^128)
3. **Prefetching** : Optimisation du cache CPU
4. **SIMD** : Opérations vectorielles parallèles

---

## 📝 Exemples Pratiques

### Exemple 1 : Chiffrement Simple

```c
#include "aes.h"

int main(void)
{
    // Configuration
    aes_ctx_t ctx = {0};
    ctx.key_size = AES_KEY_128;
    
    uint8_t key[16] = {
        0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c,
        0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08
    };
    memcpy(ctx.key.key_128, key, 16);
    aes_128_key_expansion(&ctx.key);
    
    // Nonce (IV || counter)
    uint8_t nonce[16] = {
        0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad,
        0xde, 0xca, 0xf8, 0x88, 0x00, 0x00, 0x00, 0x01
    };
    
    // Plaintext
    uint8_t plaintext[] = "Secret Message!";
    uint8_t ciphertext[32];
    
    // Output structure
    aes_gcm_counter_t gcm_out;
    gcm_out.out = ciphertext;
    gcm_out.size = sizeof(ciphertext);
    
    // Chiffrement (sans AAD)
    aes_gcm_enc(&gcm_out, nonce, NULL, 0, 
                plaintext, strlen(plaintext), &ctx);
    
    // Afficher le tag
    uint8_t tag[16];
    _mm_storeu_si128((__m128i*)tag, gcm_out.tag);
    
    printf("Tag: ");
    for (int i = 0; i < 16; i++)
        printf("%02x", tag[i]);
    printf("\n");
    
    return 0;
}
```

### Exemple 2 : Avec AAD

```c
// AAD : données authentifiées mais non chiffrées
uint8_t aad[] = {
    0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
    0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef
};

// Chiffrement avec AAD
aes_gcm_enc(&gcm_out, nonce, aad, sizeof(aad),
            plaintext, strlen(plaintext), &ctx);

// L'AAD sera authentifié mais pas chiffré
// Utile pour headers, métadonnées, etc.
```

---

## 🔍 Visualisation Complète de GCM

```
┌─────────────────────────────────────────────────────────────┐
│                      AES-GCM ENCRYPTION                      │
└─────────────────────────────────────────────────────────────┘

INPUT:
├─ Key (K): 128/192/256 bits
├─ IV: 96 bits (nonce)
├─ AAD: Variable length (Additional Authenticated Data)
└─ Plaintext (P): Variable length

STEP 1: Generate Hash Subkey
┌──────────────────┐
│  E_K(0^128) → H  │  Hash subkey pour GHASH
└──────────────────┘

STEP 2: Setup Initial Counter
┌────────────────────────────────┐
│ J0 = IV || 0x00000001          │
│ E(K, J0) → Save for tag        │
│ J1 = IV || 0x00000002 (start)  │
└────────────────────────────────┘

STEP 3: CTR Mode Encryption
    J1           J2           J3
     ↓            ↓            ↓
  E(K,J1)      E(K,J2)      E(K,J3)
     ↓            ↓            ↓
    XOR          XOR          XOR
     ↑            ↑            ↑
    P1           P2           P3
     ↓            ↓            ↓
    C1           C2           C3

STEP 4: GHASH Computation
┌────────────────────────────────────────┐
│ Input = AAD || pad || C || pad ||     │
│         len(AAD) || len(C)             │
│                                        │
│ Y_0 = 0                                │
│ Y_i = (Y_{i-1} ⊕ X_i) • H  [GF(2^128)]│
│                                        │
│ Output = Y_m                           │
└────────────────────────────────────────┘

STEP 5: Final Tag
┌─────────────────────────────┐
│ T = GHASH ⊕ E(K, J0)        │
└─────────────────────────────┘

OUTPUT:
├─ Ciphertext (C): Same length as P
└─ Tag (T): 128 bits (or truncated)
```

---

## 📚 Références

### Standards et Spécifications

- **[NIST SP 800-38D](https://csrc.nist.gov/publications/detail/sp/800-38d/final)** - Recommendation for Block Cipher Modes of Operation: Galois/Counter Mode (GCM) and GMAC
- **[NIST FIPS 197](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf)** - Advanced Encryption Standard (AES)
- **[NIST SP 800-38A](https://csrc.nist.gov/publications/detail/sp/800-38a/final)** - Recommendation for Block Cipher Modes of Operation

### Intel Documentation

- **[Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/)** - AES-NI and CLMUL instructions
- **[Intel White Paper: AES-NI](https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf)** - Advanced Encryption Standard Instructions
- **[Intel: Carry-Less Multiplication](https://www.intel.com/content/dam/develop/external/us/en/documents/clmul-wp-rev-2-02-2014-04-20.pdf)** - PCLMULQDQ instruction and applications

### Articles Académiques

- **McGrew & Viega (2004)** - *The Galois/Counter Mode of Operation (GCM)*
- **Gueron & Kounavis (2010)** - *Intel Carry-Less Multiplication Instruction and its Usage for Computing the GCM Mode*

### Ressources en Ligne

- [Wikipedia - Galois/Counter Mode](https://en.wikipedia.org/wiki/Galois/Counter_Mode)
- [Wikipedia - Finite Field Arithmetic](https://en.wikipedia.org/wiki/Finite_field_arithmetic)
- [Cryptography Stack Exchange](https://crypto.stackexchange.com/) - Questions/réponses sur GCM

### Implémentations de Référence

- **OpenSSL** - Implémentation de référence GCM
- **BoringSSL** - Fork de OpenSSL par Google
- **Crypto++** - Bibliothèque C++ de cryptographie

---

## 🎓 Conclusion

### Récapitulatif

1. **AES** est un chiffrement par bloc (128 bits)
2. Les **modes d'opération** permettent de chiffrer des messages arbitraires
3. **CTR** transforme AES en stream cipher parallélisable
4. **GCM** combine CTR + GHASH pour chiffrement authentifié
5. **GHASH** utilise la multiplication dans **GF(2^128)**
6. Les **corps de Galois** fournissent les propriétés mathématiques nécessaires
7. **AES-NI** et **PCLMULQDQ** accélèrent tout ça en hardware

### Points Clés à Retenir

✅ **GCM = CTR (chiffrement) + GHASH (authentification)**  
✅ **GHASH opère dans GF(2^128) avec multiplication optimisée**  
✅ **J0 est pour le tag, J1+ pour les données**  
✅ **AAD permet d'authentifier sans chiffrer**  
✅ **Le polynôme irréductible assure les propriétés du corps**  
✅ **PCLMULQDQ rend GCM très rapide**  

### Pour Aller Plus Loin

- Étudier l'implémentation d'OpenSSL pour comparaison
- Explorer les variantes : GCM-SIV, ChaCha20-Poly1305
- Comprendre les attaques (nonce reuse, timing attacks)
- Analyser les performances avec différents CPU

---

<div align="center">

**📚 Documentation rédigée avec ❤️ pour LibAES**

*Pour toute question ou suggestion, n'hésitez pas à ouvrir une issue !*

🔐 **Stay Secure, Stay Encrypted!** 🔐

</div>

