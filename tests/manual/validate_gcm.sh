#!/bin/bash

# Script pour valider l'implémentation AES-GCM avec les vecteurs NIST
# Usage: ./validate_gcm.sh

set -e

echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║       Validation AES-GCM avec vecteurs NIST SP 800-38D       ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""

# Vérifier si le build existe
if [ ! -d "build" ]; then
    echo "📦 Configuration initiale de Meson..."
    meson setup build
    echo ""
fi

# Compiler
echo "🔨 Compilation de la bibliothèque et des tests..."
meson compile -C build
echo ""

# Exécuter le test de validation NIST
echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║                     Test de Validation NIST                   ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""

if ./build/tests/aes_gcm/aes_gcm_nist_validation.out; then
    echo ""
    echo "╔═══════════════════════════════════════════════════════════════╗"
    echo "║  ✅ SUCCÈS: L'implémentation AES-GCM est conforme NIST!      ║"
    echo "╚═══════════════════════════════════════════════════════════════╝"
    exit 0
else
    echo ""
    echo "╔═══════════════════════════════════════════════════════════════╗"
    echo "║  ❌ ÉCHEC: L'implémentation nécessite des corrections        ║"
    echo "╚═══════════════════════════════════════════════════════════════╝"
    exit 1
fi

