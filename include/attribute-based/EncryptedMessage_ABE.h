#include "gmp.h"

typedef struct EncryptedMessage_ABE
{
	AccessTree* tree;
	Complex Ctilde;
	AffinePoint C;
} EncryptedMessage_ABE;