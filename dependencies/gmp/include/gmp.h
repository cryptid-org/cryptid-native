#ifndef __CRYPTID_GMP_H
#define __CRYPTID_GMP_H

//TODO remove if cmake is ready
#define __CRYPTID_GMP

#ifdef __CRYPTID_GMP

#include "full-gmp.h"

#elif defined(__CRYPTID_GMP_MINI)

#include "mini-gmp.h"

#endif

#endif