#include "util/PrimalityTest.h"
#include "util/Random.h"

static const int MIGHT_BE_PRIME = 1;
static const int NOT_PRIME = 0;

static int primalityTest_millerrabin(const mpz_srcptr p,
                                     const mpz_srcptr pMinus1,
                                     const mpz_ptr base, const mpz_ptr basePow,
                                     const mpz_srcptr d,
                                     const unsigned long int s) {
  mpz_powm(basePow, base, d, p);

  if (mpz_cmp_ui(basePow, 1L) == 0 || mpz_cmp(basePow, pMinus1) == 0)
    return MIGHT_BE_PRIME;

  for (unsigned long int i = 1; i < s; i++) {
    mpz_powm_ui(basePow, basePow, 2L, p);
    if (mpz_cmp(basePow, pMinus1) == 0)
      return MIGHT_BE_PRIME;

    if (mpz_cmp_ui(basePow, 1L) <= 0)
      return NOT_PRIME;
  }
  return NOT_PRIME;
}

static int primalityTest_millerrabin_mpz(const mpz_srcptr p,
                                         const int repetitions) {
  if (mpz_cmp_ui(p, 3L) <= 0) {
    return MIGHT_BE_PRIME;
  }

  mpz_t pMinus1, pMinus3, base, basePow, d;
  mpz_inits(pMinus1, pMinus3, base, basePow, d, NULL);
  unsigned long int s;
  int isPrime;

  mpz_sub_ui(pMinus1, p, 1L);

  mpz_set_ui(base, 210L);
  mpz_powm(basePow, base, pMinus1, p);
  if (mpz_cmp_ui(basePow, 1L) != 0) {
    mpz_clears(pMinus1, pMinus3, base, basePow, d, NULL);
    return NOT_PRIME;
  }

  s = mpz_scan1(pMinus1, 0L);
  mpz_tdiv_q_2exp(d, pMinus1, s);

  mpz_sub_ui(pMinus3, p, 3L);

  isPrime = 1;
  for (int r = 0; r < repetitions && isPrime; r++) {
    random_mpzInRange(base, pMinus3);
    mpz_add_ui(base, base, 2L);

    isPrime = primalityTest_millerrabin(p, pMinus1, base, basePow, d, s);
  }

  mpz_clears(pMinus1, pMinus3, base, basePow, d, NULL);
  return isPrime;
}

#if defined(__CRYPTID_EXTERN_PRIMALITY_TEST)

extern int __primalityTest_isProbablePrime(const mpz_t p);

CryptidValidationResult primalityTest_isProbablePrime(const mpz_t p) {
  return __primalityTest_isProbablePrime(p) >= MIGHT_BE_PRIME
             ? CRYPTID_VALIDATION_SUCCESS
             : CRYPTID_VALIDATION_FAILURE;
}

#else

CryptidValidationResult primalityTest_isProbablePrime(const mpz_t p) {
  return primalityTest_millerrabin_mpz(p, 50) >= MIGHT_BE_PRIME
             ? CRYPTID_VALIDATION_SUCCESS
             : CRYPTID_VALIDATION_FAILURE;
}

#endif