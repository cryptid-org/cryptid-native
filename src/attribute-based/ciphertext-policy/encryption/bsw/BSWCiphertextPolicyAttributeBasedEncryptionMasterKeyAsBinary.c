#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary.h"

void BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_destroy(BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary* masterkey)
{
	free(masterkey->beta);
	affineAsBinary_destroy(masterkey->g_alpha);
	free(masterkey);
}

void bswChiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionMasterKey(BSWCiphertextPolicyAttributeBasedEncryptionMasterKey *masterKey, const BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *masterKeyAsBinary)
{
	mpz_init(masterKey->beta);
	mpz_import(masterKey->beta, masterKeyAsBinary->betaLength, 1, 1, 0, 0, masterKeyAsBinary->beta);
	affineAsBinary_toAffine(&(masterKey->g_alpha), masterKeyAsBinary->g_alpha);
	masterKey->publickey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionPublicKey));
	bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionPublicKey(masterKey->publickey, masterKeyAsBinary->publickey);
}

void bswChiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionMasterKey(BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary *masterKeyAsBinary, const BSWCiphertextPolicyAttributeBasedEncryptionMasterKey *masterKey)
{
	masterKeyAsBinary->beta = mpz_export(NULL, &masterKeyAsBinary->betaLength, 1, 1, 0, 0, masterKey->beta);
	affineAsBinary_fromAffine(&(masterKeyAsBinary->g_alpha), masterKey->g_alpha);
	masterKeyAsBinary->publickey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary));
	bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionPublicKey(masterKeyAsBinary->publickey, masterKey->publickey);
}