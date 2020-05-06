#ifndef __CRYPTID_IDENTITY_BASED_H
#define __CRYPTID_IDENTITY_BASED_H

// Define every Identity-based crypto algorithms
#ifdef __CRYPTID_IDENTITY_BASED

#define __CRYPTID_IDENTITY_BASED_ENCRYPTION
#define __CRYPTID_IDENTITY_BASED_SIGNATURE

#endif

#include "identity-based/encryption/IdentityBasedEncryption.h"

#include "identity-based/signature/IdentityBasedSignature.h"

#endif