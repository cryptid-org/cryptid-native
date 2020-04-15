#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary.h"

void BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_destroy(BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary* publickey)
{
	ellipticCurveAsBinary_destroy(publickey->ellipticCurve);
	affineAsBinary_destroy(publickey->g);
	affineAsBinary_destroy(publickey->h);
	affineAsBinary_destroy(publickey->f);
	complexAsBinary_destroy(publickey->eggalpha);
	free(publickey->q);
}

void bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionPublicKey(BSWCiphertextPolicyAttributeBasedEncryptionPublicKey *publickey, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publickeyAsBinary)
{
	publickey->hashFunction = publickeyAsBinary->hashFunction;
	mpz_init(publickey->q);
	mpz_import(publickey->q, publickeyAsBinary->qLength, 1, 1, 0, 0, publickeyAsBinary->q);
	affineAsBinary_toAffine(&(publickey->g), publickeyAsBinary->g);
	affineAsBinary_toAffine(&(publickey->h), publickeyAsBinary->h);
	affineAsBinary_toAffine(&(publickey->f), publickeyAsBinary->f);
	complexAsBinary_toComplex(&(publickey->eggalpha), publickeyAsBinary->eggalpha);
	ellipticCurveAsBinary_toEllipticCurve(&(publickey->ellipticCurve), publickeyAsBinary->ellipticCurve);
}

void bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionPublicKey(BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary *publickeyAsBinary, const BSWCiphertextPolicyAttributeBasedEncryptionPublicKey *publickey)
{
	publickeyAsBinary->hashFunction = publickey->hashFunction;
	publickeyAsBinary->q = mpz_export(NULL, &publickeyAsBinary->qLength, 1, 1, 0, 0, publickey->q);
	affineAsBinary_fromAffine(&(publickeyAsBinary->g), publickey->g);
	affineAsBinary_fromAffine(&(publickeyAsBinary->h), publickey->h);
	affineAsBinary_fromAffine(&(publickeyAsBinary->f), publickey->f);
	complexAsBinary_fromComplex(&(publickeyAsBinary->eggalpha), publickey->eggalpha);
	ellipticCurveAsBinary_fromEllipticCurve(&(publickeyAsBinary->ellipticCurve), publickey->ellipticCurve);
}