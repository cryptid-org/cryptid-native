#include <benchmark/benchmark.h>

#include <string.h>

#include "util/SecurityLevel.h"
#include "complex/Complex.h"
#include "elliptic/AffinePoint.h"
#include "elliptic/EllipticCurve.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryption.h"

void boneh_franklin_ibe_benchmark_encrypt(benchmark::State &state) {

    const SecurityLevel securityLevel = (SecurityLevel)state.range(0);
    const char *message = "abcdefghijklmnop"; //16 bytes
    const char *identity = "darth.plagueis@sith.com";

    BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParameters;
    BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary masterSecret;

    CryptidStatus status = cryptid_ibe_bonehFranklin_setup(
      &masterSecret, &publicParameters, securityLevel);

    AffinePointAsBinary privateKey;
    status = cryptid_ibe_bonehFranklin_extract(
        &privateKey, identity, strlen(identity), masterSecret, publicParameters);

    BonehFranklinIdentityBasedEncryptionCiphertextAsBinary ciphertext;

    for (auto _ : state) {
        status = cryptid_ibe_bonehFranklin_encrypt(
            &ciphertext, message, strlen(message), identity, strlen(identity),
            publicParameters);
    }

    //char *plaintext;
    //status = cryptid_ibe_bonehFranklin_decrypt(&plaintext, ciphertext, privateKey,
    //                                            publicParameters);
    //free(plaintext);

    bonehFranklinIdentityBasedEncryptionCiphertextAsBinary_destroy(ciphertext);
    affineAsBinary_destroy(privateKey);
    free(masterSecret.masterSecret);
    bonehFranklinIdentityBasedEncryptionPublicParametersAsBinary_destroy(
        publicParameters);
}

BENCHMARK(boneh_franklin_ibe_benchmark_encrypt)->Arg(MEDIUM)->Iterations(20);

BENCHMARK_MAIN();