
#include <string.h>
#include <stdlib.h>

#include "attribute-based/ciphertext-policy/encryption/bsw/BSWCiphertextPolicyAttributeBasedEncryption.h"
#include "elliptic/TatePairing.h"
#include "util/RandBytes.h"
#include "util/Utils.h"
#include "util/PrimalityTest.h"

static const unsigned int SOLINAS_GENERATION_ATTEMPT_LIMIT = 100;
static const unsigned int POINT_GENERATION_ATTEMPT_LIMIT = 100;

static const unsigned int Q_LENGTH_MAPPING[] = { 160, 224, 256, 384, 512 };
static const unsigned int P_LENGTH_MAPPING[] = { 512, 1024, 1536, 3840, 7680 };

// Returns a publickey and a masterkey with the specified securityLevel
CryptidStatus cryptid_abe_bsw_setup(const SecurityLevel securityLevel, BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary* publickeyAsBinary, BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary* masterkeyAsBinary)
{
    BSWCiphertextPolicyAttributeBasedEncryptionMasterKey* masterkey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionMasterKey));
    // Construct the elliptic curve and its subgroup of interest
    // Select a random \f$n_q\f$-bit Solinas prime \f$q\f$.
    mpz_t q;
    mpz_init(q);

    CryptidStatus status = random_solinasPrime(q, Q_LENGTH_MAPPING[(int) securityLevel], SOLINAS_GENERATION_ATTEMPT_LIMIT);

    if (status)
    {
        mpz_clear(q);
        return status;
    }

    // Select a random integer \f$r\f$, such that \f$p = 12 \cdot r \cdot q - 1\f$ is an \f$n_p\f$-bit prime.
    unsigned int lengthOfR = P_LENGTH_MAPPING[(int) securityLevel] - Q_LENGTH_MAPPING[(int) securityLevel] - 3;
    mpz_t r, p;
    mpz_inits(r, p, NULL);
    do
    {
        random_mpzOfLength(r, lengthOfR);
        mpz_mul_ui(p, r, 12);
        mpz_mul(p, p, q);
        mpz_sub_ui(p, p, 1);
    }
    while (!primalityTest_isProbablePrime(p));

    mpz_t zero, one;
    mpz_init_set_ui(zero, 0);
    mpz_init_set_ui(one, 1);

    EllipticCurve ec;
    ellipticCurve_init(&ec, zero, one, p);


    // Select a point \f$P\f$ of order \f$q\f$ in \f$E(F_p)\f$.
    AffinePoint pointP;
    do
    {
        AffinePoint pointPprime;
        status = random_affinePoint(&pointPprime, ec, POINT_GENERATION_ATTEMPT_LIMIT);

        if (status)
        {
            mpz_clears(p, q, r, NULL);
            ellipticCurve_destroy(ec);
            return status;
        }

        mpz_t rMul;
        mpz_init_set(rMul, r);
        mpz_mul_ui(rMul, rMul, 12);

        status = affine_wNAFMultiply(&pointP, pointPprime, rMul, ec);

        if (status)
        {
            mpz_clears(p, q, r, rMul, NULL);
            ellipticCurve_destroy(ec);
            affine_destroy(pointPprime);
            return status;
        }

        mpz_clear(rMul);
        affine_destroy(pointPprime);
    }
    while (affine_isInfinity(pointP));

    mpz_t pMinusOne;
    mpz_init(pMinusOne);
    mpz_sub_ui(pMinusOne, p, 1);

    mpz_t alpha;
    mpz_init(alpha);
    random_mpzInRange(alpha, pMinusOne);

    mpz_t beta;
    mpz_init(beta);
    random_mpzInRange(beta, pMinusOne);

    BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionPublicKey));

    publickey->ellipticCurve = ec;
    publickey->g = pointP;

    status = affine_wNAFMultiply(&publickey->h, publickey->g, beta, publickey->ellipticCurve);
    if(status)
    {
        mpz_clears(p, q, r, pMinusOne, alpha, beta, NULL);
        ellipticCurve_destroy(ec);
        return status;
    }

    mpz_t betaInverse;
    mpz_init(betaInverse);
    mpz_invert(betaInverse, beta, q);

    status = affine_wNAFMultiply(&publickey->f, publickey->g, betaInverse, publickey->ellipticCurve);
    if(status)
    {
        mpz_clears(p, q, r, pMinusOne, alpha, beta, betaInverse, NULL);
        return status;
    }

    mpz_init(masterkey->beta);
    mpz_set(masterkey->beta, beta);

    status = affine_wNAFMultiply(&masterkey->g_alpha, publickey->g, alpha, publickey->ellipticCurve);
    if(status)
    {
        mpz_clears(p, q, r, pMinusOne, alpha, beta, betaInverse, NULL);
        return status;
    }

    mpz_init(publickey->q);
    mpz_set(publickey->q, q);

    hashFunction_initForSecurityLevel(&(publickey->hashFunction), securityLevel);

    Complex pairValue;
    status = tate_performPairing(&pairValue, pointP, pointP, 2, q, ec);
    if(status)
    {
        complex_destroy(pairValue);
        return status;
    }

    Complex eggalpha;
    complex_modPow(&eggalpha, pairValue, alpha, publickey->ellipticCurve.fieldOrder);
    publickey->eggalpha = eggalpha;
    complex_destroy(pairValue);

    masterkey->publickey = publickey;

    mpz_clears(zero, one, NULL);
    mpz_clears(p, q, r, pMinusOne, alpha, beta, betaInverse, NULL);

    bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionPublicKey(publickeyAsBinary, publickey);
    bswChiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionMasterKey(masterkeyAsBinary, masterkey);
    //BSWCiphertextPolicyAttributeBasedEncryptionPublicKey_destroy(publickey);

    return CRYPTID_SUCCESS;
}

// Encrypts message with the specified accessTree and publicKey to encrypted
CryptidStatus cryptid_abe_bsw_encrypt(BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary* encryptedAsBinary,
                                      const char *const message, const size_t messageLength,
                                      const BSWCiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary* publickeyAsBinary, BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary* accessTreeAsBinary)
{
    BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage));

    if(!message)
    {
        return CRYPTID_MESSAGE_NULL_ERROR;
    }

    if(messageLength == 0)
    {
        return CRYPTID_MESSAGE_LENGTH_ERROR;
    }

    if(!publickeyAsBinary) {
        return CRYPTID_MESSAGE_NULL_ERROR;
    }

    if(!accessTreeAsBinary) {
        return CRYPTID_MESSAGE_NULL_ERROR;
    }

    BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionPublicKey));
    bswChiphertextPolicyAttributeBasedEncryptionPublicKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionPublicKey(publickey, publickeyAsBinary);

    BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* accessTree = malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree));
    bswChiphertextPolicyAttributeBasedEncryptionAccessTreeAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionAccessTree(accessTree, accessTreeAsBinary);

    mpz_t pMinusOne;
    mpz_init(pMinusOne);
    mpz_sub_ui(pMinusOne, publickey->ellipticCurve.fieldOrder, 1);

    mpz_t s;
    mpz_init(s);
    random_mpzInRange(s, pMinusOne);
    BSWCiphertextPolicyAttributeBasedEncryptionAccessTreeCompute(accessTree, s, publickey);

    encrypted->tree = accessTree;
    Complex eggalphas;
    complex_modPow(&eggalphas, publickey->eggalpha, s, publickey->ellipticCurve.fieldOrder);

    mpz_t M;
    mpz_init(M);
    size_t n = messageLength;
    size_t startFrom = 0;
    char* msg = malloc(sizeof(char) * messageLength);
    strncpy(msg, message, messageLength);
    BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* prevSet = (BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet*) malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet));
    encrypted->cTildeSet = prevSet;
    prevSet->last = ABE_CTILDE_SET_NOT_COMPUTED;

    // Splitting message to parts if M >= ellipticCurve.fieldOrder
    // Splitted parts of the message is stored in a set (BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet)
    while(n > 0)
    {
        while(prevSet->last == ABE_CTILDE_SET_NOT_COMPUTED || mpz_cmp(M, publickey->ellipticCurve.fieldOrder) >= 0)
        {
            if(prevSet->last == ABE_CTILDE_SET_NOT_LAST) {
                n--;
            }
            prevSet->last = ABE_CTILDE_SET_NOT_LAST;
            char* msgPart = malloc(sizeof(char)*(n+1));
            strncpy(msgPart, msg + 0, n);
            msgPart[n] = '\0';
            mpz_import (M, n+1, 1, 1, 0, 0, msgPart);
            free(msgPart);
        }
        Complex cTilde;
        complex_modMulScalar(&cTilde, eggalphas, M, publickey->ellipticCurve.fieldOrder);
        prevSet->cTilde = cTilde;
        startFrom += n;
        n = messageLength-startFrom;
        prevSet->cTildeSet = (BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet*) malloc(sizeof(BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet));
        prevSet = prevSet->cTildeSet;
        prevSet->last = ABE_CTILDE_SET_NOT_COMPUTED;
        strncpy(msg, message + startFrom, n);
    }
    free(msg);
    complex_destroy(eggalphas);
    prevSet->cTildeSet = NULL;
    prevSet->last = ABE_CTILDE_SET_LAST;

    CryptidStatus status = affine_wNAFMultiply(&encrypted->c, publickey->h, s, publickey->ellipticCurve);
    if(status)
    {
        mpz_clear(M);
        mpz_clears(pMinusOne, s, NULL);
        affine_destroy(encrypted->c);
        return status;
    }

    mpz_clear(M);
    mpz_clears(pMinusOne, s, NULL);
    
    //BSWCiphertextPolicyAttributeBasedEncryptionPublicKey_destroy(publickey);
    bswChiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionEncryptedMessage(encryptedAsBinary, encrypted);

    BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage_destroy(encrypted);

    return CRYPTID_SUCCESS;
}

// Generates a secretkey with the specified attributes using masterkey
CryptidStatus cryptid_abe_bsw_keygen(const BSWCiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary* masterkeyAsBinary, char** attributes, const int numAttributes, BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary)
{
    BSWCiphertextPolicyAttributeBasedEncryptionMasterKey* masterkey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionMasterKey));
    bswChiphertextPolicyAttributeBasedEncryptionMasterKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionMasterKey(masterkey, masterkeyAsBinary);

    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionSecretKey));

    BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey = masterkey->publickey;

    AffinePoint gR;

    mpz_t r;
    mpz_init(r);
    BSWCiphertextPolicyAttributeBasedEncryptionRandomNumber(r, publickey);

    CryptidStatus status = affine_wNAFMultiply(&gR, publickey->g, r, publickey->ellipticCurve);
    if(status)
    {
        return status;
    }

    // Equivalent to g^(a+r)
    AffinePoint gar;
    affine_add(&gar, masterkey->g_alpha, gR, publickey->ellipticCurve);

    AffinePoint garBi;

    mpz_t betaInverse;
    mpz_init(betaInverse);
    mpz_invert(betaInverse, masterkey->beta, publickey->q);

    // Equivalent to g^((a+r)/beta)
    status = affine_wNAFMultiply(&garBi, gar, betaInverse, publickey->ellipticCurve);
    if(status)
    {
        affine_destroy(garBi);
        mpz_clear(r);
        mpz_clear(betaInverse);

        affine_destroy(gR);
        affine_destroy(gar);
        return status;
    }

    secretkey->numAttributes = numAttributes;

    secretkey->d = garBi;

    secretkey->attributes = malloc(sizeof(char*) * numAttributes);
    secretkey->dJ = malloc(sizeof(AffinePoint) * numAttributes);
    secretkey->dJa = malloc(sizeof(AffinePoint) * numAttributes);

    for(int i = 0; i < numAttributes; i++)
    {
        int attributeLength = strlen(attributes[i]);

        mpz_t rj;
        mpz_init(rj);
        BSWCiphertextPolicyAttributeBasedEncryptionRandomNumber(rj, publickey);

        // H(j)
        AffinePoint Hj;

        status = hashToPoint(&Hj, attributes[i], attributeLength, publickey->q, publickey->ellipticCurve, publickey->hashFunction);

        if(status)
        {
            return status;
        }


        // H(j)^rj in CPABE publication
        AffinePoint HjRj;

        status = affine_wNAFMultiply(&HjRj, Hj, rj, publickey->ellipticCurve);
        if(status)
        {
            return status;
        }

        // (H(j)^rj)*(g^r) in CPABE publication
        AffinePoint dJ;
        affine_add(&dJ, HjRj, gR, publickey->ellipticCurve);
        secretkey->dJ[i] = dJ;

        // g^(rj) in CPABE publication
        AffinePoint dJa;
        status = affine_wNAFMultiply(&dJa, publickey->g, rj, publickey->ellipticCurve);
        if(status)
        {
            return status;
        }
        secretkey->dJa[i] = dJa;

        secretkey->attributes[i] = malloc(strlen(attributes[i]) + 1);
        strcpy(secretkey->attributes[i], attributes[i]);

        secretkey->publickey = masterkey->publickey;

        affine_destroy(Hj);
        affine_destroy(HjRj);

        mpz_clear(rj);
    }

    mpz_clear(r);
    mpz_clear(betaInverse);

    affine_destroy(gR);
    affine_destroy(gar);

    BSWCiphertextPolicyAttributeBasedEncryptionMasterKey_destroy(masterkey);

    bswChiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionSecretKey(secretkeyAsBinary, secretkey);

    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey_destroy(secretkey);

    return CRYPTID_SUCCESS;
}

// Delegates to another secretkeyNew from secretkey with attributes being a subset of attributes(secretkey)
CryptidStatus cryptid_abe_bsw_delegate(const BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary, char** attributes, const int numAttributes, BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinaryNew)
{
    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionSecretKey));
    bswChiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionSecretKey(secretkey, secretkeyAsBinary);

    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkeyNew = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionSecretKey));

    BSWCiphertextPolicyAttributeBasedEncryptionPublicKey* publickey = secretkey->publickey;

    AffinePoint fR;
    AffinePoint gR;

    mpz_t r;
    mpz_init(r);
    BSWCiphertextPolicyAttributeBasedEncryptionRandomNumber(r, publickey);

    CryptidStatus status = affine_wNAFMultiply(&fR, publickey->f, r, publickey->ellipticCurve);
    if(status)
    {
        return status;
    }

    status = affine_wNAFMultiply(&gR, publickey->g, r, publickey->ellipticCurve);
    if(status)
    {
        return status;
    }

    secretkeyNew->numAttributes = numAttributes;

    affine_add(&secretkeyNew->d, secretkey->d, fR, publickey->ellipticCurve);

    secretkeyNew->attributes = malloc(sizeof(char*) * numAttributes);
    secretkeyNew->dJ = malloc(sizeof(AffinePoint) * numAttributes);
    secretkeyNew->dJa = malloc(sizeof(AffinePoint) * numAttributes);

    for(int i = 0; i < numAttributes; i++)
    {
        int attributeLength = strlen(attributes[i]);

        int otherID = -1;
        for(int o = 0; o < secretkey->numAttributes; o++)
        {
            if(strcmp(secretkey->attributes[o], attributes[i]) == 0)
            {
                otherID = o;
                break;
            }
        }
        if(otherID == -1)
        {
            return CRYPTID_ILLEGAL_PRIVATE_KEY_ERROR;
        }

        mpz_t rj;
        mpz_init(rj);
        BSWCiphertextPolicyAttributeBasedEncryptionRandomNumber(rj, publickey);

        // H(j)
        AffinePoint Hj;

        status = hashToPoint(&Hj, attributes[i], attributeLength, publickey->q, publickey->ellipticCurve, publickey->hashFunction);

        if(status)
        {
            return status;
        }


        // H(j)^rj in CPABE publication
        AffinePoint HjRj;

        status = affine_wNAFMultiply(&HjRj, Hj, rj, publickey->ellipticCurve);
        if(status)
        {
            return status;
        }

        // (H(j)^rj)*dJ in CPABE publication
        AffinePoint dJ;
        affine_add(&dJ, HjRj, gR, publickey->ellipticCurve);

        AffinePoint dJDk;
        affine_add(&dJDk, dJ, secretkey->dJ[otherID], publickey->ellipticCurve);
        secretkeyNew->dJ[i] = dJDk;

        AffinePoint dJa;
        status = affine_wNAFMultiply(&dJa, publickey->g, rj, publickey->ellipticCurve);
        if(status)
        {
            return status;
        }

        AffinePoint dJadJK;
        affine_add(&dJadJK, dJa, secretkey->dJa[otherID], publickey->ellipticCurve);
        secretkeyNew->dJa[i] = dJadJK;

        secretkeyNew->attributes[i] = malloc(strlen(attributes[i]) + 1);
        strcpy(secretkeyNew->attributes[i], attributes[i]);

        secretkeyNew->publickey = secretkey->publickey;

        affine_destroy(Hj);
        affine_destroy(HjRj);
        affine_destroy(dJ);
        affine_destroy(dJa);

        mpz_clear(rj);
    }

    mpz_clear(r);

    affine_destroy(fR);
    affine_destroy(gR);

    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey_destroy(secretkey);

    bswChiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_fromBswChiphertextPolicyAttributeBasedEncryptionSecretKey(secretkeyAsBinaryNew, secretkeyNew);

    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey_destroy(secretkeyNew);

    return CRYPTID_SUCCESS;
}

// Subfunction of decrypt, calculating A value (result) of encrypted and accessTree (node)
CryptidStatus BSWCiphertextPolicyAttributeBasedEncryptionDecryptNode(const BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted, const BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey, const BSWCiphertextPolicyAttributeBasedEncryptionAccessTree* node, Complex* result, int* statusCode)
{
    if(BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_isLeaf(node))
    {
        int found = -1;
        for(int i = 0; i < secretkey->numAttributes; i++)
        {
            if(strcmp(secretkey->attributes[i], node->attribute) == 0)
            {
                found = i;
                break;
            }
        }
        if(found >= 0)
        {
            Complex pairValue;
            CryptidStatus status = tate_performPairing(&pairValue, secretkey->dJ[found], node->cY, 2, secretkey->publickey->q, secretkey->publickey->ellipticCurve);
            if(status)
            {
                return status;
            }

            Complex pairValueA;
            status = tate_performPairing(&pairValueA, secretkey->dJa[found], node->cYa, 2, secretkey->publickey->q, secretkey->publickey->ellipticCurve);
            if(status)
            {
                return status;
            }

            Complex pairValueAinverse;
            status = complex_multiplicativeInverse(&pairValueAinverse, pairValueA, secretkey->publickey->ellipticCurve.fieldOrder);
            if(status)
            {
                return status;
            }

            complex_modMul(result, pairValue, pairValueAinverse, secretkey->publickey->ellipticCurve.fieldOrder);

            complex_destroy(pairValue);
            complex_destroy(pairValueA);
            complex_destroy(pairValueAinverse);

            *statusCode = 1;
        }
    }
    else
    {
        Complex Sx[node->numChildren];
        int Codes[node->numChildren];
        int num = 0;
        for(int i = 0; i < node->numChildren; i++)
        {
            if(node->children[i] && node->children[i] != NULL)
            {
                Complex F;
                int code = 0;
                CryptidStatus status = BSWCiphertextPolicyAttributeBasedEncryptionDecryptNode(encrypted, secretkey, node->children[i], &F, &code);
                if(status)
                {
                    return status;
                }
                if(code)
                {
                    Sx[i] = F;
                    Codes[i] = 1;
                    num++;
                }
                else
                {
                    Codes[i] = 0;
                }
            }
            else
            {
                Codes[i] = 0;
            }
        }

        if(num > 0)
        {
            int indexes[num];
            int c = 0;
            for(int i = 0; i < node->numChildren; i++)
            {
                if(Codes[i] == 1)
                {
                    indexes[c] = i+1;
                    c++;
                }
            }

            Complex fX;
            complex_initLong(&fX, 1, 0);
            for(int i = 0; i < num; i++)
            {
                int result = Lagrange_coefficient(indexes[i], indexes, num, 0);
                mpz_t resultMpz;
                mpz_init_set_ui(resultMpz, abs(result));
                Complex res;
                complex_modPow(&res, Sx[indexes[i]-1], resultMpz, secretkey->publickey->ellipticCurve.fieldOrder); // Sx[indexes[c]] ^ result
                complex_destroy(Sx[indexes[i]-1]);
                Complex tmp;
                if(result < 0) {
                    // Workaround for a^(-b) = 1/(a^b)
                    CryptidStatus status = complex_multiplicativeInverse(&tmp, res, secretkey->publickey->ellipticCurve.fieldOrder);
                    if(status)
                    {
                        return status;
                    }
                    complex_destroy(res);
                }
                else
                {
                    tmp = res;
                }

                Complex oldfX = fX;
                // fX = fX * (Sx[indexes[c]] ^ result)
                complex_modMul(&fX, oldfX, tmp, secretkey->publickey->ellipticCurve.fieldOrder);

                complex_destroy(oldfX);
                complex_destroy(tmp);
                mpz_clear(resultMpz);
            }
            *result = fX;
            *statusCode = 1;
        }
    }
    
    return CRYPTID_SUCCESS;
}

CryptidStatus cryptid_abe_bsw_decrypt(char **result, const BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary* encryptedAsBinary, const BSWCiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary* secretkeyAsBinary)
{
    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey* secretkey = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionSecretKey));
    bswChiphertextPolicyAttributeBasedEncryptionSecretKeyAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionSecretKey(secretkey, secretkeyAsBinary);
    BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage* encrypted = malloc(sizeof (BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage));
    bswChiphertextPolicyAttributeBasedEncryptionEncryptedMessageAsBinary_toBswChiphertextPolicyAttributeBasedEncryptionEncryptedMessage(encrypted, encryptedAsBinary);
    if (!result)
    {
        return CRYPTID_RESULT_POINTER_NULL_ERROR;
    }
    // Check whether the attributes satisfy the accessTree
    int satisfy = BSWCiphertextPolicyAttributeBasedEncryptionAccessTree_satisfyValue(encrypted->tree, secretkey->attributes, secretkey->numAttributes);
    if(satisfy == 0)
    {
        return CRYPTID_ILLEGAL_PRIVATE_KEY_ERROR;
    }
    Complex A;
    int code = 0;
    CryptidStatus status = BSWCiphertextPolicyAttributeBasedEncryptionDecryptNode(encrypted, secretkey, encrypted->tree, &A, &code);
    if(status)
    {
        return status;
    }
    if(code == 0)
    {
        return CRYPTID_ILLEGAL_PRIVATE_KEY_ERROR;
    }

    Complex eCD;
    status = tate_performPairing(&eCD, encrypted->c, secretkey->d, 2, secretkey->publickey->q, secretkey->publickey->ellipticCurve);
    if(status)
    {
        return status;
    }

    Complex eCDinverse;
    status = complex_multiplicativeInverse(&eCDinverse, eCD, secretkey->publickey->ellipticCurve.fieldOrder);
    if(status)
    {
        return status;
    }

    complex_destroy(eCD);

    BSWCiphertextPolicyAttributeBasedEncryptionCtildeSet* lastSet = encrypted->cTildeSet;
    char* fullString = malloc(1);
    fullString[0] = '\0';
    // Iterating over sets of encrypted (splitted) messages
    while(lastSet->last == ABE_CTILDE_SET_NOT_LAST) {
        Complex cTildeA;
        complex_modMul(&cTildeA, lastSet->cTilde, A, secretkey->publickey->ellipticCurve.fieldOrder);

        // Finally equivalent to cTilde/(e(C, D)/A) = M
        Complex decrypted;
        complex_modMul(&decrypted, cTildeA, eCDinverse, secretkey->publickey->ellipticCurve.fieldOrder);
        complex_destroy(cTildeA);

        size_t resultLength;
        char *tmpResult = mpz_export(NULL, &resultLength, 1, 1, 0, 0, decrypted.real);

        char* prevFullString = malloc(strlen(fullString) + 1);
        strcpy(prevFullString, fullString);
        free(fullString);
        
        // Concat splitted parts of message
        fullString = concat(prevFullString, tmpResult);
        free(prevFullString);
        free(tmpResult);

        complex_destroy(decrypted);

        lastSet = lastSet->cTildeSet;
    }

    complex_destroy(A);
    complex_destroy(eCDinverse);

    *result = fullString;

    BSWCiphertextPolicyAttributeBasedEncryptionSecretKey_destroy(secretkey);

    BSWCiphertextPolicyAttributeBasedEncryptionEncryptedMessage_destroy(encrypted);

    return CRYPTID_SUCCESS;
}