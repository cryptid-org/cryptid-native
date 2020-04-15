#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary.h"

void BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_destroy(BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary* encryptedAsBinary)
{
	BSWCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary* lastSet = encryptedAsBinary->cTildeSet;
	while(lastSet->last == ABE_CTILDE_SET_NOT_LAST)
	{
		BSWCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary* prev = lastSet;
		complexAsBinary_destroy(lastSet->cTilde);
		lastSet = lastSet->cTildeSet;
		free(prev);
	}
	free(lastSet);
	affineAsBinary_destroy(encryptedAsBinary->c);
	bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_destroy(encryptedAsBinary->tree);
	free(encryptedAsBinary);
}

void bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_toBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet *ctildeSet, const BSWCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary *ctildeSetAsBinary)
{
	ctildeSet->last = ctildeSetAsBinary->last;
	if(ctildeSet->last == ABE_CTILDE_SET_NOT_LAST)
	{
		complexAsBinary_toComplex(&(ctildeSet->cTilde), ctildeSetAsBinary->cTilde);
	}
	BSWCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary* lastSet = ctildeSetAsBinary->cTildeSet;
	if(ctildeSetAsBinary->last == ABE_CTILDE_SET_NOT_LAST)
	{
		BSWCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary* prev = lastSet;
		ctildeSet->cTildeSet = (BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet*) malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet));
		bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_toBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(ctildeSet->cTildeSet, prev);
	}
}

void bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_fromBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(BSWCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary *ctildeSetAsBinary, const BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet *ctildeSet)
{
	ctildeSetAsBinary->last = ctildeSet->last;
	if(ctildeSet->last == ABE_CTILDE_SET_NOT_LAST)
	{
		complexAsBinary_fromComplex(&(ctildeSetAsBinary->cTilde), ctildeSet->cTilde);
	}
	BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* lastSet = ctildeSet->cTildeSet;
	if(ctildeSet->last == ABE_CTILDE_SET_NOT_LAST)
	{
		BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* prev = lastSet;
		ctildeSetAsBinary->cTildeSet = (BSWCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary*) malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary));
		bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_fromBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(ctildeSetAsBinary->cTildeSet, prev);
	}
}

void bswChiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionEncryptedMessage(BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage *encryptedMessage, const BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary *encryptedMessageAsBinary)
{
	encryptedMessage->tree = malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree));
	bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionAccessTree(encryptedMessage->tree, encryptedMessageAsBinary->tree);
	encryptedMessage->cTildeSet = (BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet*) malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet));
	bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_toBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(encryptedMessage->cTildeSet, encryptedMessageAsBinary->cTildeSet);
	affineAsBinary_toAffine(&(encryptedMessage->c), encryptedMessageAsBinary->c);
}

void bswChiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionEncryptedMessage(BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary *encryptedMessageAsBinary, const BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage *encryptedMessage)
{
	encryptedMessageAsBinary->tree = malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary));
	bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionAccessTree(encryptedMessageAsBinary->tree, encryptedMessage->tree);
	encryptedMessageAsBinary->cTildeSet = (BSWCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary*) malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary));
	bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_fromBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(encryptedMessageAsBinary->cTildeSet, encryptedMessage->cTildeSet);
	affineAsBinary_fromAffine(&(encryptedMessageAsBinary->c), encryptedMessage->c);
}