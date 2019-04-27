#ifndef __CRYPTID_SECURITY_LEVEL_H
#define __CRYPTID_SECURITY_LEVEL_H


// References
//  * [RFC-5091] Xavier Boyen, Luther Martin. 2007. RFC 5091. Identity-Based Cryptography Standard (IBCS) #1: Supersingular Curve Implementations of the BF and BB1 Cryptosystems


/**
 * The security level of the encryption. Please refer to Section 9. in [RFC-5091] regarding the meaning
 * of the security levels.
 */
typedef enum
{
    LOWEST,
    LOW,
    MEDIUM,
    HIGH,
    HIGHEST
} SecurityLevel;

#endif
