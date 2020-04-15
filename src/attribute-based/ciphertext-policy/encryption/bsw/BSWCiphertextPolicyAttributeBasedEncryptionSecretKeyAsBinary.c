#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary.h"

void BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_destroy(BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkey)
{
	affineAsBinary_destroy(secretkey->d);
	for(int i = 0; i < secretkey->numAttributes; i++)
    {
    	affineAsBinary_destroy(secretkey->dJ[i]);
    	affineAsBinary_destroy(secretkey->dJa[i]);
    }
    free(secretkey->attributes);
    free(secretkey->dJ);
    free(secretkey->dJa);
    free(secretkey->attributeLengths);
}

void bswChiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionSecretKey(BSWCiphertextPolicyAttributeBasedEncryptionSecretKey *secretKey, const BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary *secretKeyAsBinary)
{
	affineAsBinary_toAffine(&(secretKey->d), secretKeyAsBinary->d);
	secretKey->numAttributes = secretKeyAsBinary->numAttributes;
	secretKey->attributes = malloc(sizeof(char*) * secretKey->numAttributes);
    secretKey->dJ = malloc(sizeof(AffinePoint) * secretKey->numAttributes);
    secretKey->dJa = malloc(sizeof(AffinePoint) * secretKey->numAttributes);
	for(int i = 0; i < secretKey->numAttributes; i++)
    {
    	affineAsBinary_toAffine(&(secretKey->dJ[i]), secretKeyAsBinary->dJ[i]);
    	affineAsBinary_toAffine(&(secretKey->dJa[i]), secretKeyAsBinary->dJa[i]);
        secretKey->attributes[i] = malloc(secretKeyAsBinary->attributeLengths[i] + 1);
        strcpy(secretKey->attributes[i], secretKeyAsBinary->attributes[i]);
    }
    secretKey->publickey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionPublicKey));
	bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionPublicKey(secretKey->publickey, secretKeyAsBinary->publickey);
}

void bswChiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionSecretKey(BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary *secretKeyAsBinary, const BSWCiphertextPolicyAttributeBasedEncryptionSecretKey *secretKey)
{
	affineAsBinary_fromAffine(&(secretKeyAsBinary->d), secretKey->d);
	secretKeyAsBinary->numAttributes = secretKey->numAttributes;
	secretKeyAsBinary->attributes = malloc(sizeof(char*) * secretKeyAsBinary->numAttributes);
    secretKeyAsBinary->attributeLengths = malloc(sizeof(int) * secretKeyAsBinary->numAttributes);
    secretKeyAsBinary->dJ = malloc(sizeof(AffinePointAsBinary) * secretKeyAsBinary->numAttributes);
    secretKeyAsBinary->dJa = malloc(sizeof(AffinePointAsBinary) * secretKeyAsBinary->numAttributes);
	for(int i = 0; i < secretKey->numAttributes; i++)
    {
    	affineAsBinary_fromAffine(&(secretKeyAsBinary->dJ[i]), secretKey->dJ[i]);
    	affineAsBinary_fromAffine(&(secretKeyAsBinary->dJa[i]), secretKey->dJa[i]);
        secretKeyAsBinary->attributeLengths[i] = strlen(secretKey->attributes[i]);
        secretKeyAsBinary->attributes[i] = malloc(strlen(secretKey->attributes[i]) + 1);
        strcpy(secretKeyAsBinary->attributes[i], secretKey->attributes[i]);
    }
    secretKeyAsBinary->publickey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary));
	bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionPublicKey(secretKeyAsBinary->publickey, secretKey->publickey);
}