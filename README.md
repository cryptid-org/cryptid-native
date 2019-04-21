<p align="center">
  <a href="https://github.com/cryptid-org">
    <img alt="CryptID" src="docs/img/cryptid-logo.png" width="200">
  </a>
</p>

<p align="center">
Identity-based Encryption solution you can trust.
</p>

---

# CryptID

High-performance, cross-platform C and WebAssembly implementation of the aforementioned RFC, aimed for usage in both browsers and Node.

## Building CryptID

Although it's possible to build a C library from CryptID, it's aimed for WebAssembly compilation through emscripten. The following build process assumes that you're using the [CryptID Dev Machine](dev-machine).

First `vagrant ssh` into the dev virtual machine. Then switch to the root account using `sudo su`. Now setup the emscripten dev environment:

~~~~bash
cd /home/vagrant/emsdk
./emsdk activate sdk-tag-1.38.8-32bit
source ./emsdk_env.sh
~~~~

Now you can build the library using `task.js` from the repository root:

~~~~bash
./task.js cryptid wasm build
~~~~

The resulting JS and WASM files are placed in the `out` directory. By default, the script creates a release binary. Debug symbols and settings can be enabled by running `./task.js cryptid wasm build --debug`.

## Testing CryptID

Assuming, that you are using the `CryptID Dev Machine`, testing can be done as follows. First setup the environment, so that emscripten is on the PATH:

~~~~bash
sudo su
cd /home/vagrant/emsdk
./emsdk activate sdk-tag-1.38.8-32bit
source ./emsdk_env.sh
~~~~

Now use `task.js` from the repository root:

  * run all tests in WASM format: `./task.js cryptid wasm test-all`
  * run individual tests in WASM format: `./task.js cryptid wasm test <component>`, for example `./task.js cryptid wasm test Complex`
  * run all tests in native format: `./task.js cryptid native test-all`
  * run individual tests in native format: `./task.js cryptid native test <component>`
  
## Developing Tests

  * Tests must be written in C using the [greatest](https://github.com/silentbicycle/greatest) unit testing library.
  * A separate test file should be created for each component of CryptID. The test file should be named `component.test.c`.
