#ifndef __CRYPTID_IO_H
#define __CRYPTID_IO_H

#include "gmp.h"

#include "CryptID.h"
#include "util/Validation.h"
#include "complex/Complex.h"

/**
 * Writes the public parameters to the following files: PP.ellipticCurve.a  PP.ellipticCurve.b  PP.ellipticCurve.fieldOrder  PP.hashf  PP.pointPpublic.x  PP.pointPpublic.y  PP.pointP.x  PP.pointP.y  PP.q in the folder ./PP.
 * @param publicParameters The parameter storing the public parameters.
 */
void writePublicParToFiles(PublicParameters* publicParameters);

/**
 * Writes the ciphertext tuple to the following files: cipher  cipherUx  cipherUy
 in the folder ./CT.
 * @param publicParameters The parameter storing the public parameters.
 */
void writeCipherTextToFiles(CipherTextTuple* ciphertext);

/**
 * Writes the private key to the following files: privateX  privateY
 in the folder ./PK.
 * @param publicParameters The parameter storing the public parameters.
 */
void writePrivateKeyToFiles(AffinePoint privateKey);

/**
 * Reads public parameters from the files written by writePublicParToFile.
 * @return the public parameters.
 */
PublicParameters readPublicParFromFile();

/**
 * Reads private key from the files written by writePublicParToFiles.
 * @return the private key.
 */
AffinePoint readPrivateKeyFromFiles();

/**
 * Reads ciphertext from the files written by writePublicParToFiles.
 * @return the ciphertext.
 */
CipherTextTuple readCipherTextFromFile();

#endif
