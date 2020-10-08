#ifndef __CRYPTID_FORMAL_LANGUAGE_IDENTITY_BASED_ENCRYPTION_H
#define __CRYPTID_FORMAL_LANGUAGE_IDENTITY_BASED_ENCRYPTION_H

#ifdef __CRYPTID_FORMAL_LANGUAGE_IDENTITY_BASED_ENCRYPTION

#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary.h"
#include "identity-based/signature/hess/HessIdentityBasedSignatureMasterSecretAsBinary.h"
#include "identity-based/signature/hess/HessIdentityBasedSignaturePublicParametersAsBinary.h"
#include "util/SecurityLevel.h"
#include "util/Status.h"

CryptidStatus cryptid_ibe_formalLanguage_setup(
    BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary
        *masterSecretAsBinary,
    BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        *publicParametersAsBinary,
    HessIdentityBasedSignatureMasterSecretAsBinary
        *masterSecretAsBinary,
    HessIdentityBasedSignaturePublicParametersAsBinary
        *publicParametersAsBinary
    const SecurityLevel securityLevel);

CryptidStatus cryptid_ibe_formalLanguage_evaluate(boolean *result, char* authorizationFormula, stuff tokenizedAuthorizationFormula, const char *const identity, const size_t identityLength);

CryptidStatus cryptid_ibe_formalLanguage_extract();

CryptidStatus cryptid_ibe_formalLanguage_signFormula();

CryptidStatus cryptid_ibe_formalLanguage_generateEncryptionKey();

CryptidStatus cryptid_ibe_formalLanguage_encrypt();

CryptidStatus cryptid_ibe_formalLanguage_decrypt();

#endif

#endif