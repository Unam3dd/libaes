# Tests AES - Documentation

Ce document décrit la suite de tests pour la bibliothèque libaes.

## Tests AES-GCM (Galois Counter Mode)

### Nouveaux tests ajoutés

1. **aes_128_gcm_test1.c** - Test vecteur NIST #1
   - Test avec clé et nonce à zéro
   - Pas de AAD, pas de plaintext
   - Vérifie la génération correcte du tag d'authentification
   - Vecteur de test officiel du NIST SP 800-38D

2. **aes_128_gcm_test2.c** - Test vecteur NIST #2
   - Test avec un bloc de plaintext (16 bytes)
   - Pas de AAD
   - Vérifie le chiffrement et le tag
   - Compare avec les valeurs attendues du NIST

3. **aes_128_gcm_test3_aad.c** - Test avec AAD
   - Test avec Additional Authenticated Data (20 bytes)
   - Plaintext de 60 bytes (multi-blocs)
   - Vérifie l'intégration de l'AAD dans le calcul GHASH
   - Clé et nonce non-triviaux

4. **aes_128_gcm_test4_multiblock.c** - Test multi-blocs
   - 3 blocs complets (48 bytes)
   - Test d'encryption/decryption
   - Vérifie que les tags correspondent
   - Vérifie que le déchiffrement retrouve le plaintext original

5. **aes_128_gcm_test5_partial_block.c** - Test bloc partiel
   - Plaintext de 23 bytes (non aligné sur 16)
   - AAD non aligné également
   - Test le padding implicite dans GHASH
   - Vérifie le traitement des données partielles

6. **aes_192_gcm_test.c** - Test AES-192-GCM
   - Test avec clé 192 bits
   - 32 bytes de plaintext
   - Vérifie le fonctionnement avec AES-192

7. **aes_256_gcm_test.c** - Test AES-256-GCM
   - Test avec clé 256 bits
   - 64 bytes de plaintext (4 blocs)
   - AAD de longueur variable
   - Vérifie le fonctionnement avec AES-256

## Tests supplémentaires pour autres modes

### AES-ECB
- **aes_128_ecb_two.c** - Test additionnel avec pattern différent

### AES-CBC
- **aes_256_cbc_two.c** - Test AES-256-CBC avec 96 bytes

### AES-CTR
- **aes_256_ctr_one.c** - Déjà existant (vérifié)

### AES-OFB
- **aes_256_ofb_two.c** - Test AES-256-OFB avec données non alignées

### AES-CFB
- **aes_256_cfb_two.c** - Test AES-256-CFB avec pattern complexe

## Exécution des tests

### Compiler tous les tests
```bash
meson setup build
meson compile -C build
```

### Exécuter tous les tests
```bash
meson test -C build
```

### Exécuter uniquement les tests GCM
```bash
meson test -C build --suite aes-gcm-test-suite
```

### Exécuter un test spécifique
```bash
meson test -C build aes_128_gcm_test1
```

### Tests avec sortie verbose
```bash
meson test -C build --verbose
```

## Couverture des tests

### AES-GCM
- ✅ Test avec vecteurs NIST officiels
- ✅ Test avec AAD (Additional Authenticated Data)
- ✅ Test multi-blocs
- ✅ Test blocs partiels / non alignés
- ✅ Test AES-128, AES-192, AES-256
- ✅ Test encryption/decryption symétrique
- ✅ Vérification des tags d'authentification

### Modes testés
- ✅ ECB (Electronic Codebook)
- ✅ CBC (Cipher Block Chaining)
- ✅ CFB (Cipher Feedback)
- ✅ OFB (Output Feedback)
- ✅ CTR (Counter)
- ✅ GCM (Galois Counter Mode)

### Tailles de clés testées
- ✅ AES-128 (128 bits)
- ✅ AES-192 (192 bits)
- ✅ AES-256 (256 bits)

## Structure des tests

Chaque test suit cette structure :
1. **Setup** - Initialisation du contexte, clés, IV/nonce
2. **Encryption** - Chiffrement des données
3. **Verification** - Vérification du ciphertext/tag
4. **Decryption** - Déchiffrement
5. **Validation** - Comparaison avec le plaintext original

## Notes importantes

### AES-GCM
- L'implémentation actuelle suppose un nonce de 128 bits (16 bytes)
- Les 96 premiers bits sont fixes, les 32 derniers servent de compteur
- Le tag d'authentification fait 128 bits
- L'AAD peut être NULL si sa longueur est 0

### Vecteurs de test NIST
Les tests `aes_128_gcm_test1` et `aes_128_gcm_test2` utilisent des vecteurs officiels du NIST SP 800-38D pour garantir la conformité avec le standard.

## Améliorations futures possibles

- [ ] Tests avec nonces de taille variable (96 bits standard)
- [ ] Tests avec tags de taille variable (96, 104, 112, 120, 128 bits)
- [ ] Tests de performance/benchmark
- [ ] Tests avec des données de grande taille (> 1 MB)
- [ ] Tests de vérification d'authentification (tag invalide)
- [ ] Tests de cas d'erreur (pointeurs NULL, tailles invalides)

## Références

- NIST Special Publication 800-38D - Galois/Counter Mode (GCM)
- FIPS 197 - Advanced Encryption Standard (AES)
- Intel AES-NI White Papers


