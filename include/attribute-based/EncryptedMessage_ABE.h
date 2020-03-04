#ifndef __CRYPTID_CRYPTID_ENCRYPTEDMESSAGE_ABE_H
#define __CRYPTID_CRYPTID_ENCRYPTEDMESSAGE_ABE_H

#include "gmp.h"
#include "elliptic/AffinePoint.h"
#include "attribute-based/AccessTree.h"
#include "complex/Complex.h"

typedef struct Ctilde_set
{
	Complex Ctilde;
	struct Ctilde_set* Ctilde_set;
	int last;
} Ctilde_set;

typedef struct EncryptedMessage_ABE
{
	AccessTree* tree;
	Ctilde_set* Ctilde_set;
	AffinePoint C;
} EncryptedMessage_ABE;

void destroyEncryptedMessage_ABE(EncryptedMessage_ABE* encrypted);

#endif
