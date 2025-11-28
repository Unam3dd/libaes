# Tests Manuels et de Debug

Ce répertoire contient des tests manuels utilisés pour le développement et le débogage de l'implémentation AES-GCM.

## Tests de Multiplication Galois (gfmul)

- **test_gfmul_debug.c** : Tests de base pour vérifier gfmul
- **test_gfmul_nist.c** : Tests gfmul avec vecteurs NIST
- **test_gfmul_vectors.c** : Tests gfmul avec différents vecteurs

## Tests GHASH

- **test_ghash_detailed.c** : Test détaillé étape par étape de GHASH
- **test_ghash_step.c** : Test de la génération du hash subkey H
- **test_lengths_encoding.c** : Vérification de l'encodage des longueurs AAD/CT

## Tests de Validation

- **test_openssl_debug.c** : Comparaison détaillée avec OpenSSL
- **test_openssl_gcm.c** : Validation contre OpenSSL GCM (référence)
- **test_with_96bit_iv.c** : Test avec IV de 96 bits (standard GCM)

## Compilation

Ces tests peuvent être compilés individuellement :

```bash
cc -o test_name test_name.c -I ../../inc -L ../../build -laes -Wl,-rpath,../../build -maes -mpclmul -msse2 -mssse3
```

Pour les tests OpenSSL :

```bash
cc -o test_openssl_gcm test_openssl_gcm.c -lssl -lcrypto
```

## Résultat de Validation

✅ **L'implémentation AES-GCM est validée contre OpenSSL**

Notre implémentation produit les mêmes résultats qu'OpenSSL pour les vecteurs de test NIST :
- Ciphertext : ✅ Identique
- Tag d'authentification : ✅ Identique (`da80ce830cfda02da2a218a1744f4c76`)


