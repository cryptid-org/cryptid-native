#include "gmp.h"

#include "elliptic/AffinePoint.h"
#include "attribute-based/AccessTree.h"
#include "attribute-based/MasterKey_ABE.h"
#include "util/Status.h"

CryptidStatus cryptid_setup_ABE(const SecurityLevel securityLevel, PublicKey_ABE* publickey, MasterKey_ABE* masterkey);