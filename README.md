<div align="center">
  <a href="https://github.com/cryptid-org">
    <img alt="CryptID" src="docs/img/cryptid-logo.png" width="200">
  </a>
</div>

<div align="center">

[![Build Status](https://dev.azure.com/cryptid-org/cryptid-native/_apis/build/status/cryptid-org.cryptid-native?branchName=master)](https://dev.azure.com/cryptid-org/cryptid-native/_build/latest?definitionId=3&branchName=master)
[![Coverage Status](https://coveralls.io/repos/github/cryptid-org/cryptid-native/badge.svg?branch=master)](https://coveralls.io/github/cryptid-org/cryptid-native?branch=master)
[![License](https://img.shields.io/github/license/cryptid-org/cryptid-native.svg)](LICENSE)

</div>

<div align="center">
Cross-platform Identity-based Encryption solution.
</div>

---

# CryptID.native

Cross-platform C implementation of the Boneh-Franklin Identity-based Encryption system as described in [RFC 5091](https://tools.ietf.org/html/rfc5091), the Hess Identity-based Signature protocol described in [Hess-IBS](https://doi.org/10.1007/3-540-36492-7_20) and the BSW Ciphertext-Policy Attribute-based Encryption described in [BSW CP-ABE](https://doi.org/10.1109/SP.2007.11).

CryptID.native provides the foundation of CryptID.js which is a WebAssembly library mainly targeting browsers.

If you're new to CryptID and Identity-based Encryption, then make sure to check out the [CryptID Getting Started](https://github.com/cryptid-org/getting-started) guide.

## Building CryptID.native

### Dependencies

CryptID requires the following components to be present:

  * gcc 5+,
  * [Node.js](https://nodejs.org/en/) v8+,
  * [GMP](https://gmplib.org/) 6+.
    * Please note, that [GMP-LAWL](https://github.com/cryptid-org/gmp-lawl/) is not capable of integrating with CryptID.native in a stable manner yet. Thus, the use of the vanilla GMP library is advised.

### Creating a Static Library

A static library can be created using the following command:

~~~~bash
./task.sh build
~~~~

The resulting library (`libcryptid.a`) will be placed in the `build` directory.

## Example

There will be a working example added, when the second version is finished. If you want to try out the library, check out the first version [CryptID V1](https://github.com/cryptid-org/cryptid-native/tree/release/v1)

## License

CryptID.native is licensed under the [Apache License 2.0](LICENSE).

Licenses of dependencies:

  * [GMP](https://gmplib.org/): [GNU LGPL v3](https://www.gnu.org/licenses/lgpl.html)
  * [greatest](https://github.com/silentbicycle/greatest): [ISC](https://github.com/silentbicycle/greatest/blob/master/LICENSE)
  * [greatest/entapment](https://github.com/silentbicycle/greatest/blob/master/contrib/entapment): [ISC](https://github.com/silentbicycle/greatest/blob/master/contrib/entapment)
  * [SHA](https://tools.ietf.org/html/rfc6234): [2-clause BSD License](https://tools.ietf.org/html/rfc6234#section-8.1.1)

## Acknowledgements

This work is supported by the construction EFOP-3.6.3-VEKOP-16-2017-00002. The project is supported by the European Union, co-financed by the European Social Fund.

<p align="right">
  <img alt="CryptID" src="docs/img/szechenyi-logo.jpg" width="350">
</p>
