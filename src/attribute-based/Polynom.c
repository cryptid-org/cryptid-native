#include "attribute-based/Polynom.h"

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
	Polynom* polynom = malloc(sizeof(Polynom));
	polynom->children = malloc(sizeof(PolynomExpression)*degree+1);
	polynom->children[0] = malloc(sizeof(PolynomExpression));
	polynom->children[0]->degree = 0;
	polynom->degree = degree;
	mpz_init_set(polynom->children[0]->coeff, zeroValue);
	int i;
	for(i = 1; i <= degree; i++) {
		polynom->children[i] = malloc(sizeof(PolynomExpression));
		polynom->children[i]->degree = i;
		mpz_init(polynom->children[i]->coeff);
		ABE_randomNumber(polynom->children[i]->coeff, publickey);
	}
	return polynom;
}

CryptidStatus polynomSum(Polynom* polynom, int x, mpz_t sum)
{
	int i;
	mpz_set_ui(sum, 0);
	mpz_t t;
	mpz_init_set_ui(t, 1);
	for(i = 0; i <= polynom->degree; i++)
	{
		mpz_t tmp;
		mpz_init(tmp);
		mpz_set_ui(tmp, 1);
		mpz_mul(tmp, t, polynom->children[i]->coeff); // coeff*x^degree

		mpz_mul_ui(t, t, x);

		mpz_add(sum, sum, tmp);
		mpz_clear(tmp);
	}
	mpz_clear(t);

	return CRYPTID_SUCCESS;
}