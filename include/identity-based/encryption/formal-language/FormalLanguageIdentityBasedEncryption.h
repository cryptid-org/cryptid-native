#ifndef __CRYPTID_FORMAL_LANGUAGE_IDENTITY_BASED_ENCRYPTION_H
#define __CRYPTID_FORMAL_LANGUAGE_IDENTITY_BASED_ENCRYPTION_H

#ifdef __CRYPTID_FORMAL_LANGUAGE_IDENTITY_BASED_ENCRYPTION

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionCiphertextAsBinary.h"

#include "identity-based/signature/hess/HessIdentityBasedSignatureMasterSecretAsBinary.h"
#include "identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h"
#include "identity-based/signature/hess/HessIdentityBasedSignatureSignatureAsBinary.h"

#include "util/SecurityLevel.h"
#include "util/Status.h"
#include "util/LogicalExpressionTree.h"

CryptidStatus cryptid_ibe_formalLanguage_setup(
    BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary
        *masterSecretBFAsBinary,
    BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        *publicParametersBFAsBinary,
    HessIdentityBasedSignatureMasterSecretAsBinary
        *masterSecretHessAsBinary,
    HessIdentityBasedSignaturePublicParametersAsBinary
        *publicParametersHessAsBinary,
    const SecurityLevel securityLevel);

CryptidStatus cryptid_ibe_formalLanguage_evaluate(int *result, const CryptidLogicalExpressionTree* authorizationFormula, const char *const identity, const size_t identityLength);

CryptidStatus cryptid_ibe_formalLanguage_extract(AffinePointAsBinary *result, const CryptidLogicalExpressionTree* authorizationFormula, const char *const authorizationFormulaString, const size_t authorizationFormulaStringLength, const HessIdentityBasedSignatureSignatureAsBinary *authorizationFormulaSignature, const char *const identityAlpha, const size_t identityAlphaLength, const char *const identityBeta, const size_t identityBetaLength, const char *const encryptionKey, const size_t encryptionKeyLength, const BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary masterSecretBFAsBinary, const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParametersBFAsBinary, const HessIdentityBasedSignaturePublicParametersAsBinary publicParametersHessAsBinary);

CryptidStatus cryptid_ibe_formalLanguage_signFormula(HessIdentityBasedSignatureSignatureAsBinary *result, const char *const authorizationFormulaString, const size_t authorizationFormulaStringLength, const char *const identity, const size_t identityLength, const AffinePointAsBinary privateKeyAsBinary, const HessIdentityBasedSignaturePublicParametersAsBinary publicParametersAsBinary);

CryptidStatus cryptid_ibe_formalLanguage_generateEncryptionKey(char** result, const char *const identity, const size_t identityLength);

CryptidStatus cryptid_ibe_formalLanguage_encrypt(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary *result,
    const char *const message, const size_t messageLength,
    const char *const identity, const size_t identityLength,
    const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        publicParametersAsBinary);

CryptidStatus cryptid_ibe_formalLanguage_decrypt(char **result,
    const BonehFranklinIdentityBasedEncryptionCiphertextAsBinary
        ciphertextAsBinary,
    const AffinePointAsBinary privateKeyAsBinary,
    const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        publicParametersAsBinary);

#endif

#endif