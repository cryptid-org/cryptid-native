#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary.h"

void bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_destroy(
    bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary
        *encryptedAsBinary) {
  bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary *lastSet =
      encryptedAsBinary->cTildeSet;
  while (lastSet->last == ABE_CTILDE_SET_NOT_LAST) {
    bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary *prev =
        lastSet;
    complexAsBinary_destroy(lastSet->cTilde);
    lastSet = lastSet->cTildeSet;
    free(prev);
  }
  free(lastSet);
  affineAsBinary_destroy(encryptedAsBinary->c);
  bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_destroy(
      encryptedAsBinary->tree);
  free(encryptedAsBinary);
}

void bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_toBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(
    bswCiphertextPolicyAttributeBasedEncryptionCtildeSet *ctildeSet,
    const bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary
        *ctildeSetAsBinary) {
  ctildeSet->last = ctildeSetAsBinary->last;
  if (ctildeSet->last == ABE_CTILDE_SET_NOT_LAST) {
    complexAsBinary_toComplex(&(ctildeSet->cTilde), ctildeSetAsBinary->cTilde);
  }
  bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary *lastSet =
      ctildeSetAsBinary->cTildeSet;
  if (ctildeSetAsBinary->last == ABE_CTILDE_SET_NOT_LAST) {
    bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary *prev =
        lastSet;
    ctildeSet->cTildeSet =
        (bswCiphertextPolicyAttributeBasedEncryptionCtildeSet *)malloc(
            sizeof(bswCiphertextPolicyAttributeBasedEncryptionCtildeSet));
    bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_toBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(
        ctildeSet->cTildeSet, prev);
  }
}

void bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_fromBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(
    bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary
        *ctildeSetAsBinary,
    const bswCiphertextPolicyAttributeBasedEncryptionCtildeSet *ctildeSet) {
  ctildeSetAsBinary->last = ctildeSet->last;
  if (ctildeSet->last == ABE_CTILDE_SET_NOT_LAST) {
    complexAsBinary_fromComplex(&(ctildeSetAsBinary->cTilde),
                                ctildeSet->cTilde);
  }
  bswCiphertextPolicyAttributeBasedEncryptionCtildeSet *lastSet =
      ctildeSet->cTildeSet;
  if (ctildeSet->last == ABE_CTILDE_SET_NOT_LAST) {
    bswCiphertextPolicyAttributeBasedEncryptionCtildeSet *prev = lastSet;
    ctildeSetAsBinary->cTildeSet =
        (bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary *)malloc(
            sizeof(
                bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary));
    bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_fromBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(
        ctildeSetAsBinary->cTildeSet, prev);
  }
}

void bswChiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionEncryptedMessage(
    bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessage
        *encryptedMessage,
    const bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary
        *encryptedMessageAsBinary) {
  encryptedMessage->tree =
      (bswCiphertextPolicyAttributeBasedEncryptionAccessTree *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionAccessTree));
  bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionAccessTree(
      encryptedMessage->tree, encryptedMessageAsBinary->tree);
  encryptedMessage->cTildeSet =
      (bswCiphertextPolicyAttributeBasedEncryptionCtildeSet *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionCtildeSet));
  bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_toBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(
      encryptedMessage->cTildeSet, encryptedMessageAsBinary->cTildeSet);
  affineAsBinary_toAffine(&(encryptedMessage->c), encryptedMessageAsBinary->c);
}

void bswChiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionEncryptedMessage(
    bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary
        *encryptedMessageAsBinary,
    const bswCiphertextPolicyAttributeBasedEncryptionEncryptedMessage
        *encryptedMessage) {
  encryptedMessageAsBinary->tree =
      (bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary *)malloc(
          sizeof(
              bswCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary));
  bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionAccessTree(
      encryptedMessageAsBinary->tree, encryptedMessage->tree);
  encryptedMessageAsBinary->cTildeSet =
      (bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary *)malloc(
          sizeof(bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary));
  bswCiphertextPolicyAttributeBasedEncryptionCtildeSetAsBinary_fromBswCiphertextPolicyAttributeBasedEncryptionCtildeSet(
      encryptedMessageAsBinary->cTildeSet, encryptedMessage->cTildeSet);
  affineAsBinary_fromAffine(&(encryptedMessageAsBinary->c),
                            encryptedMessage->c);
}