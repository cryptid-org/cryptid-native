<p align="center">
  <a href="https://github.com/cryptid-org">
    <img alt="CryptID" src="docs/img/cryptid-logo.png" width="200">
  </a>
</p>

<p align="center">
Cross-platform Identity-based Encryption solution.
</p>

---

# CryptID.native

Cross-platform C implementation of the Boneh-Franklin Identity-based Encryption system as described in [RFC 5091](https://tools.ietf.org/html/rfc5091).

CryptID.native provides the foundation of CryptID.js which is a WebAssembly library mainly targeting browsers.

## Building CryptID.native

### Dependencies

CryptID requires the following components to be present:

  * gcc 5+,
  * [Node.js](https://nodejs.org/en/) v10+,
  * [GMP](https://gmplib.org/) 6+.

### Static Library

A static library can be created using the following command:

~~~~bash
./task.sh build-static
~~~~

The resulting library will be placed in the `build` directory.

## Testing CryptID.native

### Running Tests

Tests can be executed using the task command:

~~~~bash
./task.sh test
~~~~

If no additional arguments are present, then all components will be tested. Running only specific tests can be enforced by providing a list of component names:

~~~~bash
./task.sh test Complex TatePairing
~~~~

### Testing with Coverage

Coverage data is generated using the following tools:

  * gcov,
  * lcov,
  * genhtml.

Line and branch coverage data can be generated using the following command:

~~~~bash
./task.sh html-coverage
~~~~

This command will place the HTML-formatted coverage data in the `coverage` directory. Gathering coverage for specific components can be done by appending a list of components names after the command.

### Leak Checking

Memory leak checking requires the presence `valgrind`.

The leak checking process can be initiated by executing the following command:

~~~~bash
./task.sh memory-check
~~~~

Again, specific components can be memchecked by appending their names after the command.

## License

CryptID.native is licensed under the [Apache License 2.0](LICENSE).

## Acknowledgements

This work is supported by the construction EFOP-3.6.3-VEKOP-16-2017-00002. The project is supported by the European Union, co-financed by the European Social Fund.

<p align="right">
  <img alt="CryptID" src="docs/img/szechenyi-logo.jpg" width="350">
</p>
