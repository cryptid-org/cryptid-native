#include "identity-based/encryption/formal-language/FormalLanguageIdentityBasedEncryption.h"
#include "identity-based/encryption/boneh-franklin/BonehFranklinIdentityBasedEncryption.h"
#include "identity-based/signature/hess/HessIdentityBasedSignature.h"

CryptidStatus cryptid_ibe_formalLanguage_setup(
    BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary
        *masterSecretBFAsBinary,
    BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        *publicParametersBFAsBinary,
    HessIdentityBasedSignatureMasterSecretAsBinary
        *masterSecretHessAsBinary,
    HessIdentityBasedSignaturePublicParametersAsBinary
        *publicParametersHessAsBinary,
    const SecurityLevel securityLevel) {

        CryptidStatus status = cryptid_ibe_bonehFranklin_setup(masterSecretBFAsBinary, publicParametersBFAsBinary, securityLevel);

        if(status)
            return status;

        status = cryptid_ibs_hess_setup(masterSecretHessAsBinary, publicParametersHessAsBinary, securityLevel);

        return status;
    }

    CryptidStatus cryptid_ibe_formalLanguage_evaluate(int *result, const CryptidLogicalExpressionTree* authorizationFormula, const char *const identity, const size_t identityLength) {
        *result = 1;
        return CRYPTID_SUCCESS;
    }

CryptidStatus cryptid_ibe_formalLanguage_extract(AffinePointAsBinary *result, const CryptidLogicalExpressionTree* authorizationFormula, const char *const authorizationFormulaString, const size_t authorizationFormulaStringLength, const HessIdentityBasedSignatureSignatureAsBinary *authorizationFormulaSignature, const char *const identityAlpha, const size_t identityAlphaLength, const char *const identityBeta, const size_t identityBetaLength, const char *const encryptionKey, const size_t encryptionKeyLength, const BonehFranklinIdentityBasedEncryptionMasterSecretAsBinary masterSecretBFAsBinary, const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary publicParametersBFAsBinary, const HessIdentityBasedSignaturePublicParametersAsBinary publicParametersHessAsBinary) {

    CryptidStatus status = cryptid_ibs_hess_verify(authorizationFormulaString, authorizationFormulaStringLength, *authorizationFormulaSignature, identityAlpha, identityAlphaLength, publicParametersHessAsBinary);

    if(status)
        return status;

    int isBetaAuthozized;

    status = cryptid_ibe_formalLanguage_evaluate(&isBetaAuthozized, authorizationFormula, identityBeta, identityBetaLength);

    if(status)
        return status;

    if(!isBetaAuthozized)
        return CRYPTID_NOT_AUTHORIZED_IDENTITY;

    status = cryptid_ibe_bonehFranklin_extract(result, encryptionKey, encryptionKeyLength, masterSecretBFAsBinary, publicParametersBFAsBinary);

    if(status)
        return status;

    return CRYPTID_SUCCESS;
}

CryptidStatus cryptid_ibe_formalLanguage_signFormula(HessIdentityBasedSignatureSignatureAsBinary *result, const char *const authorizationFormulaString, const size_t authorizationFormulaStringLength, const char *const identity, const size_t identityLength, const AffinePointAsBinary privateKeyAsBinary, const HessIdentityBasedSignaturePublicParametersAsBinary publicParametersAsBinary) {

    return cryptid_ibs_hess_sign(result, authorizationFormulaString, authorizationFormulaStringLength, identity, identityLength, privateKeyAsBinary, publicParametersAsBinary);
}

CryptidStatus cryptid_ibe_formalLanguage_generateEncryptionKey(char** result, const char *const identity, const size_t identityLength) {
    return CRYPTID_SUCCESS;
}


CryptidStatus cryptid_ibe_formalLanguage_encrypt(BonehFranklinIdentityBasedEncryptionCiphertextAsBinary *result,
    const char *const message, const size_t messageLength,
    const char *const identity, const size_t identityLength,
    const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        publicParametersAsBinary) {

    return cryptid_ibe_bonehFranklin_encrypt(result, message, messageLength, identity, identityLength, publicParametersAsBinary);

}

CryptidStatus cryptid_ibe_formalLanguage_decrypt(char **result,
    const BonehFranklinIdentityBasedEncryptionCiphertextAsBinary
        ciphertextAsBinary,
    const AffinePointAsBinary privateKeyAsBinary,
    const BonehFranklinIdentityBasedEncryptionPublicParametersAsBinary
        publicParametersAsBinary) {
    
    return cryptid_ibe_bonehFranklin_decrypt(result, ciphertextAsBinary, privateKeyAsBinary, publicParametersAsBinary);

}