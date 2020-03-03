#include "attribute-based/EncryptedMessage_ABE.h"
#include <stdlib.h>

void destroyEncryptedMessage_ABE(EncryptedMessage_ABE* encrypted) {
	Ctilde_set* lastSet = encrypted->Ctilde_set;
	while(lastSet->last == 0) {
		Ctilde_set* prev = lastSet;
		complex_destroy(lastSet->Ctilde);
		lastSet = lastSet->Ctilde_set;
		free(prev);
	}
	free(lastSet);
	affine_destroy(encrypted->C);
	free(encrypted);
}