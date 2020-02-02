#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Polynom {
	int degree;
    struct PolynomExpression* children[0];
} Polynom;

typedef struct PolynomExpression {
	int degree;
	mpz_t coeff;
} PolynomExpression;

CryptidStatus ABE_randomNumber(mpz_t randElement, PublicKey_ABE* publickey)
{
	mpz_t pMinusOne;
    mpz_init(pMinusOne);
    mpz_sub_ui(pMinusOne, publickey->ellipticCurve.fieldOrder, 1);

    random_mpzInRange(randElement, pMinusOne);

    mpz_clear(pMinusOne);

    return CRYPTID_SUCCESS;
}

Polynom* createPolynom(int degree, mpz_t zeroValue, PublicKey_ABE* publickey) {
	Polynom* polynom = malloc(sizeof(Polynom)+degree*sizeof(PolynomExpression));
	polynom->degree = degree;
	mpz_init_set(polynom->children[0]->coeff, zeroValue);
	int i;
	for(i = 1; i <= degree; i++) {
		polynom->children[i]->degree = i;
		mpz_init(polynom->children[i]->coeff);
		ABE_randomNumber(polynom->children[i]->coeff, publickey);
	}
	return polynom;
}

CryptidStatus polynomSum(Polynom* polynom, int x, mpz_t sum)
{
	mpz_t one;
    mpz_init_set_ui(one, 1);

	int i;
	mpz_set_ui(sum, 0);
	for(i = 0; i <= polynom->degree; i++)
	{
		mpz_t tmp;
		mpz_init(tmp);
		mpz_set_ui(tmp, x);
		mpz_powm_ui(tmp, tmp, polynom->children[i]->degree, one);
		mpz_mul(tmp, tmp, polynom->children[i]->coeff); // coeff*x^degree

		mpz_add(sum, sum, tmp);
		mpz_clear(tmp);
	}

	mpz_clear(one);

	return CRYPTID_SUCCESS;
}