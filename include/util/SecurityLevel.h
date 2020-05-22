#ifndef __CRYPTID_SECURITY_LEVEL_H
#define __CRYPTID_SECURITY_LEVEL_H

/**
 * ## Description
 *
 * The security level of the protocol. Please refer to Section 9. in [RFC-5091]
 * regarding the meaning of the security levels.
 */
typedef enum { LOWEST, LOW, MEDIUM, HIGH, HIGHEST } SecurityLevel;

#endif
