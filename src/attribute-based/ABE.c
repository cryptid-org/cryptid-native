#include <string.h>
#include <stdlib.h>

#include "attribute-based/ABE.h"
#include "elliptic/TatePairing.h"
#include "util/RandBytes.h"
#include "util/Utils.h"
#include "util/PrimalityTest.h"

static const unsigned int SOLINAS_GENERATION_ATTEMPT_LIMIT = 100;
static const unsigned int POINT_GENERATION_ATTEMPT_LIMIT = 100;

static const unsigned int Q_LENGTH_MAPPING[] = { 160, 224, 256, 384, 512 };
static const unsigned int P_LENGTH_MAPPING[] = { 512, 1024, 1536, 3840, 7680 };

CryptidStatus cryptid_setup_ABE(const SecurityLevel securityLevel, PublicKey_ABE* publickey, MasterKey_ABE* masterkey)
{
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

    publickey->ellipticCurve = ec;
    publickey->g = pointP;

    status = affine_wNAFMultiply(&publickey->h, publickey->g, beta, publickey->ellipticCurve);
    if(status) {
        mpz_clears(p, q, r, pMinusOne, alpha, beta, NULL);
        ellipticCurve_destroy(ec);
        return status;
    }

    mpz_t beta_inverse;
    mpz_init(beta_inverse);
    mpz_invert(beta_inverse, beta, q);

    status = affine_wNAFMultiply(&publickey->f, publickey->g, beta_inverse, publickey->ellipticCurve);
    if(status) {
        mpz_clears(p, q, r, pMinusOne, alpha, beta, beta_inverse, NULL);
        return status;
    }

    mpz_init(masterkey->beta);
    mpz_set(masterkey->beta, beta);

    status = affine_wNAFMultiply(&masterkey->g_alpha, publickey->g, alpha, publickey->ellipticCurve);
    if(status) {
        mpz_clears(p, q, r, pMinusOne, alpha, beta, beta_inverse, NULL);
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

    masterkey->pubkey = publickey;

    mpz_clears(zero, one, NULL);
    mpz_clears(p, q, r, pMinusOne, alpha, beta, beta_inverse, NULL);

    return CRYPTID_SUCCESS;
}

CryptidStatus compute_tree(AccessTree* accessTree, mpz_t s, PublicKey_ABE* publickey)
{
    if(!isLeaf(accessTree))
    {
        int d = accessTree->value - 1; // dx = kx-1, degree = threshold-1
        Polynom* q = createPolynom(d, s, publickey);

        int i;
        for(i = 0; i < MAX_CHILDREN; i++)
        {
            if(accessTree->children[i] && accessTree->children[i] != NULL)
            {
                mpz_t sum;
                mpz_init(sum);
                polynomSum(q, i+1, sum);
                compute_tree(accessTree->children[i], sum, publickey);
                mpz_clear(sum);
            }
        }

        destroyPolynom(q);
    }
    else
    {
        AffinePoint Cy;
        CryptidStatus status = affine_wNAFMultiply(&Cy, publickey->g, s, publickey->ellipticCurve);
        if(status) {
            affine_destroy(Cy);
            return status;
        }

        AffinePoint hashedPoint;

        status = hashToPoint(&hashedPoint, accessTree->attribute, accessTree->attributeLength, publickey->q, publickey->ellipticCurve, publickey->hashFunction);

        if (status) 
        {
            return status;
        }

        AffinePoint CyA;
        status = affine_wNAFMultiply(&CyA, hashedPoint, s, publickey->ellipticCurve);
        if(status) {
            return status;
        }

        affine_destroy(hashedPoint);

        accessTree->Cy = Cy;
        accessTree->CyA = CyA;
        accessTree->computed = 1;
    }

    return CRYPTID_SUCCESS;
}

CryptidStatus cryptid_encrypt_ABE(EncryptedMessage_ABE* encrypted,
                                const char *const message, const size_t messageLength,
                                    PublicKey_ABE* publickey, AccessTree* accessTree)
{
    if(!message)
    {
        return CRYPTID_MESSAGE_NULL_ERROR;
    }

    if(messageLength == 0)
    {
        return CRYPTID_MESSAGE_LENGTH_ERROR;
    }

    if(!publickey) {
        return CRYPTID_MESSAGE_NULL_ERROR;
    }

    if(!accessTree) {
        return CRYPTID_MESSAGE_NULL_ERROR;
    }

    mpz_t pMinusOne;
    mpz_init(pMinusOne);
    mpz_sub_ui(pMinusOne, publickey->ellipticCurve.fieldOrder, 1);

    mpz_t s;
    mpz_init(s);
    random_mpzInRange(s, pMinusOne);
    compute_tree(accessTree, s, publickey);

    encrypted->tree = accessTree;
    Complex eggalphas;
    complex_modPow(&eggalphas, publickey->eggalpha, s, publickey->ellipticCurve.fieldOrder);

    mpz_t M;
    mpz_init(M);
    size_t n = messageLength;
    size_t startFrom = 0;
    char* msg = malloc(sizeof(char)*messageLength);
    strncpy(msg, message, messageLength);
    Ctilde_set* prevSet = (Ctilde_set*) malloc(sizeof(Ctilde_set));
    encrypted->Ctilde_set = prevSet;
    prevSet->last = 2;
    while(n > 0)
    {
        while(prevSet->last == 2 || mpz_cmp(M, publickey->ellipticCurve.fieldOrder) >= 0)
        {
            if(prevSet->last == 0) {
                n--;
            }
            prevSet->last = 0;
            char* msgPart = malloc(sizeof(char)*(n+1));
            strncpy(msgPart, msg + 0, n);
            msgPart[n] = '\0';
            mpz_import (M, n+1, 1, 1, 0, 0, msgPart);
            free(msgPart);
        }
        Complex Ctilde;
        complex_modMulScalar(&Ctilde, eggalphas, M, publickey->ellipticCurve.fieldOrder);
        prevSet->Ctilde = Ctilde;
        startFrom += n;
        n = messageLength-startFrom;
        prevSet->Ctilde_set = (Ctilde_set*) malloc(sizeof(Ctilde_set));
        prevSet = prevSet->Ctilde_set;
        prevSet->last = 2;
        strncpy(msg, message + startFrom, n);
    }
    free(msg);
    complex_destroy(eggalphas);
    prevSet->Ctilde_set = NULL;
    prevSet->last = 1;

    CryptidStatus status = affine_wNAFMultiply(&encrypted->C, publickey->h, s, publickey->ellipticCurve);
    if(status) {
        mpz_clear(M);
        mpz_clears(pMinusOne, s, NULL);
        affine_destroy(encrypted->C);
        return status;
    }

    mpz_clear(M);
    mpz_clears(pMinusOne, s, NULL);

    return CRYPTID_SUCCESS;
}

CryptidStatus cryptid_keygen_ABE(MasterKey_ABE* masterkey, char** attributes, SecretKey_ABE* secretkey)
{
    PublicKey_ABE* publickey = masterkey->pubkey;

    AffinePoint Gr;

    mpz_t r;
    mpz_init(r);
    ABE_randomNumber(r, publickey);

    CryptidStatus status = affine_wNAFMultiply(&Gr, publickey->g, r, publickey->ellipticCurve);
    if(status) {
        return status;
    }

    AffinePoint Gar;
    affine_add(&Gar, masterkey->g_alpha, Gr, publickey->ellipticCurve);

    AffinePoint GarBi;

    mpz_t beta_inverse;
    mpz_init(beta_inverse);
    mpz_invert(beta_inverse, masterkey->beta, publickey->q);

    status = affine_wNAFMultiply(&GarBi, Gar, beta_inverse, publickey->ellipticCurve);
    if(status) {
        affine_destroy(GarBi);
        mpz_clear(r);
        mpz_clear(beta_inverse);

        affine_destroy(Gr);
        affine_destroy(Gar);
        return status;
    }

    secretkey->D = GarBi;

    for(int i = 0; i < MAX_ATTRIBUTES; i++)
    {
        if(attributes[i][0] != '\0')
        {
            int attributeLength = 0;
            for(int c = 0; c < ATTRIBUTE_LENGTH; c++) {
                if(attributes[i][c] == '\0') {
                    break;
                }
                attributeLength++;
            }

            mpz_t rj;
            mpz_init(rj);
            ABE_randomNumber(rj, publickey);

            AffinePoint Hj;

            status = hashToPoint(&Hj, attributes[i], attributeLength, publickey->q, publickey->ellipticCurve, publickey->hashFunction);

            if(status) {
                return status;
            }


            AffinePoint HjRj;

            status = affine_wNAFMultiply(&HjRj, Hj, rj, publickey->ellipticCurve);
            if(status) {
                return status;
            }

            AffinePoint Dj;
            affine_add(&Dj, HjRj, Gr, publickey->ellipticCurve);
            secretkey->Dj[i] = Dj;

            AffinePoint DjA;
            status = affine_wNAFMultiply(&DjA, publickey->g, rj, publickey->ellipticCurve);
            if(status) {
                return status;
            }
            secretkey->DjA[i] = DjA;

            secretkey->attributes[i] = malloc(strlen(attributes[i]) + 1);
            strcpy(secretkey->attributes[i], attributes[i]);

            secretkey->pubkey = masterkey->pubkey;

            affine_destroy(Hj);
            affine_destroy(HjRj);

            mpz_clear(rj);
        }
        else
        {
            secretkey->attributes[i] = malloc(sizeof(char));
            secretkey->attributes[i][0] = '\0';
        }
    }

    mpz_clear(r);
    mpz_clear(beta_inverse);

    affine_destroy(Gr);
    affine_destroy(Gar);

    return CRYPTID_SUCCESS;
}

CryptidStatus cryptid_delegate_ABE(SecretKey_ABE* secretkey, char** attributes, SecretKey_ABE* secretkey_new)
{
    PublicKey_ABE* publickey = secretkey->pubkey;

    AffinePoint Fr;
    AffinePoint Gr;

    mpz_t r;
    mpz_init(r);
    ABE_randomNumber(r, publickey);

    CryptidStatus status = affine_wNAFMultiply(&Fr, publickey->f, r, publickey->ellipticCurve);
    if(status) {
        return status;
    }

    status = affine_wNAFMultiply(&Gr, publickey->g, r, publickey->ellipticCurve);
    if(status) {
        return status;
    }

    affine_add(&secretkey_new->D, secretkey->D, Fr, publickey->ellipticCurve);

    for(int i = 0; i < MAX_ATTRIBUTES; i++)
    {
        if(attributes[i][0] != '\0')
        {
            int attributeLength = 0;
            for(int c = 0; c < ATTRIBUTE_LENGTH; c++) {
                if(attributes[i][c] == '\0') {
                    break;
                }
                attributeLength++;
            }

            int otherID = -1;
            for(int o = 0; o < MAX_ATTRIBUTES; o++)
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
            ABE_randomNumber(rj, publickey);

            AffinePoint Hj;

            status = hashToPoint(&Hj, attributes[i], attributeLength, publickey->q, publickey->ellipticCurve, publickey->hashFunction);

            if(status) {
                return status;
            }


            AffinePoint HjRj;

            status = affine_wNAFMultiply(&HjRj, Hj, rj, publickey->ellipticCurve);
            if(status) {
                return status;
            }

            AffinePoint Dj;
            affine_add(&Dj, HjRj, Gr, publickey->ellipticCurve);
            AffinePoint DjDk;
            affine_add(&DjDk, Dj, secretkey->Dj[otherID], publickey->ellipticCurve);
            secretkey_new->Dj[i] = DjDk;

            AffinePoint DjA;
            status = affine_wNAFMultiply(&DjA, publickey->g, rj, publickey->ellipticCurve);
            if(status) {
                return status;
            }

            AffinePoint DjADjK;
            affine_add(&DjADjK, DjA, secretkey->DjA[otherID], publickey->ellipticCurve);
            secretkey_new->DjA[i] = DjADjK;

            secretkey_new->attributes[i] = malloc(strlen(attributes[i]) + 1);
            strcpy(secretkey_new->attributes[i], attributes[i]);

            secretkey_new->pubkey = secretkey->pubkey;

            affine_destroy(Hj);
            affine_destroy(HjRj);
            affine_destroy(Dj);
            affine_destroy(DjA);

            mpz_clear(rj);
        }
        else
        {
            secretkey_new->attributes[i] = malloc(sizeof(char));
            secretkey_new->attributes[i][0] = '\0';
        }
    }

    mpz_clear(r);

    affine_destroy(Fr);
    affine_destroy(Gr);

    return CRYPTID_SUCCESS;
}

int Lagrange_coefficient(int xi, int* S, int sLength, int x)
{
    double result = 1;
    for(int i = 0; i < sLength; i++)
    {
        if(&S[i] != NULL)
        {
            if(S[i] != xi)
            {
                double xD = (double) x;
                double j = (double) S[i];
                double xiD = (double) xi;
                result = result*((xD-j)/(xiD-j));
            }
        }
    }
    return (int) result;
}

CryptidStatus DecryptNode_ABE(EncryptedMessage_ABE* encrypted, SecretKey_ABE* secretkey, AccessTree* node, Complex* result, int* statusCode)
{
    if(isLeaf(node))
    {
        int found = -1;
        for(int i = 0; i < MAX_ATTRIBUTES; i++)
        {
            if(secretkey->attributes[i] && secretkey->attributes[i][0] && secretkey->attributes[i][0] != '\0')
            {
                if(strcmp(secretkey->attributes[i], node->attribute) == 0)
                {
                    found = i;
                    break;
                }
            }
        }
        if(found >= 0)
        {
            Complex pairValue;
            CryptidStatus status = tate_performPairing(&pairValue, secretkey->Dj[found], node->Cy, 2, secretkey->pubkey->q, secretkey->pubkey->ellipticCurve);
            if(status)
            {
                return status;
            }

            Complex pairValueA;
            status = tate_performPairing(&pairValueA, secretkey->DjA[found], node->CyA, 2, secretkey->pubkey->q, secretkey->pubkey->ellipticCurve);
            if(status)
            {
                return status;
            }

            Complex pairValueA_inverse;
            status = complex_multiplicativeInverse(&pairValueA_inverse, pairValueA, secretkey->pubkey->ellipticCurve.fieldOrder);
            if(status)
            {
                return status;
            }

            complex_modMul(result, pairValue, pairValueA_inverse, secretkey->pubkey->ellipticCurve.fieldOrder);

            complex_destroy(pairValue);
            complex_destroy(pairValueA);
            complex_destroy(pairValueA_inverse);

            *statusCode = 1;
        }
    }
    else
    {
        Complex Sx[MAX_CHILDREN];
        int Codes[MAX_CHILDREN];
        int num = 0;
        for(int i = 0; i < MAX_CHILDREN; i++)
        {
            if(node->children[i] && node->children[i] != NULL)
            {
                Complex F;
                int code = 0;
                DecryptNode_ABE(encrypted, secretkey, node->children[i], &F, &code);
                if(code == 1)
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
            for(int i = 0; i < MAX_CHILDREN; i++)
            {
                if(Codes[i] == 1)
                {
                    indexes[c] = i+1;
                    c++;
                }
            }

            Complex Fx;
            complex_initLong(&Fx, 1, 0);
            for(int i = 0; i < num; i++)
            {
                int result = Lagrange_coefficient(indexes[i], indexes, num, 0);
                mpz_t resultMpz;
                mpz_init_set_ui(resultMpz, abs(result));
                Complex res;
                complex_modPow(&res, Sx[indexes[i]-1], resultMpz, secretkey->pubkey->ellipticCurve.fieldOrder); // Sx[indexes[c]] ^ result
                complex_destroy(Sx[indexes[i]-1]);
                Complex tmp;
                if(result < 0) {
                    CryptidStatus status = complex_multiplicativeInverse(&tmp, res, secretkey->pubkey->ellipticCurve.fieldOrder);
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
                Complex oldFx = Fx;
                complex_modMul(&Fx, oldFx, tmp, secretkey->pubkey->ellipticCurve.fieldOrder);
                complex_destroy(oldFx);
                complex_destroy(tmp);
                mpz_clear(resultMpz);

                //Fx = Fx * Sx[indexes[c]] ^ result
            }
            *result = Fx;
            *statusCode = 1;
        }
    }
    
    return CRYPTID_SUCCESS;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

CryptidStatus cryptid_decrypt_ABE(char **result, EncryptedMessage_ABE* encrypted, SecretKey_ABE* secretkey)
{
    if (!result)
    {
        return CRYPTID_RESULT_POINTER_NULL_ERROR;
    }
    int satisfy = satisfyValue(encrypted->tree, secretkey->attributes);
    if(satisfy == 0)
    {
        return CRYPTID_ILLEGAL_PRIVATE_KEY_ERROR;
    }
    Complex A;
    int code = 0;
    DecryptNode_ABE(encrypted, secretkey, encrypted->tree, &A, &code);
    if(code == 0)
    {
        return CRYPTID_ILLEGAL_PRIVATE_KEY_ERROR;
    }

    Complex eCD;
    CryptidStatus status = tate_performPairing(&eCD, encrypted->C, secretkey->D, 2, secretkey->pubkey->q, secretkey->pubkey->ellipticCurve);
    if(status)
    {
        return status;
    }

    Complex eCD_inverse;
    status = complex_multiplicativeInverse(&eCD_inverse, eCD, secretkey->pubkey->ellipticCurve.fieldOrder);
    if(status)
    {
        return status;
    }

    complex_destroy(eCD);

    Ctilde_set* lastSet = encrypted->Ctilde_set;
    //char* fullNumber = "";
    char* fullString = malloc(strlen("")+1);
    strcpy(fullString, "");
    while(lastSet->last == 0) {
        Complex Ctilde_A;
        complex_modMul(&Ctilde_A, lastSet->Ctilde, A, secretkey->pubkey->ellipticCurve.fieldOrder);

        Complex decrypted;
        complex_modMul(&decrypted, Ctilde_A, eCD_inverse, secretkey->pubkey->ellipticCurve.fieldOrder);
        complex_destroy(Ctilde_A);

        size_t resultLength;
        char *tmpResult = mpz_export(NULL, &resultLength, 1, 1, 0, 0, decrypted.real);

        char* prevFullString = malloc(strlen(fullString)+1);
        strcpy(prevFullString, fullString);
        free(fullString);
        fullString = concat(prevFullString, tmpResult);
        free(prevFullString);
        free(tmpResult);

        complex_destroy(decrypted);

        lastSet = lastSet->Ctilde_set;
    }

    complex_destroy(A);
    complex_destroy(eCD_inverse);

    *result = fullString;

    return CRYPTID_SUCCESS;
}