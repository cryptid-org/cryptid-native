#ifndef __CRYPTID_CRYPTID_ENCRYPTEDMESSAGE_ABE_H
#define __CRYPTID_CRYPTID_ENCRYPTEDMESSAGE_ABE_H

#include "gmp.h"

typedef struct EncryptedMessage_ABE
{
	AccessTree* tree;
	Complex Ctilde;
	AffinePoint C;
} EncryptedMessage_ABE;

#endif
