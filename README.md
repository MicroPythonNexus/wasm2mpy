# wasm2mpy

A bare-bones example demonstrating how to convert a `WASM` file into a `MPY` module and run it dynamically on a Raspberry Pi Pico.

> [!IMPORTANT]
> **This is purely a Proof-of-Concept, not optimized or ready for actual use.**

## Compile

Modify `Makefile` to specify the path to [WABT](https://github.com/WebAssembly/wabt/releases/latest) and the [MicroPython](https://github.com/micropython/micropython) source code.

```sh
$ make
```

Output:

```log
W2C test.wasm
GEN build/test.config.h
CC runtime.c
CC wasm.c
CC wasm2c/wasm-rt-mem-impl.c
CC wasm2c/wasm-rt-impl.c
AS thumb_case.S
LINK build/runtime.o
arch:         EM_ARM
text size:    1388
bss size:     108
GOT entries:  5
GEN test.mpy
```

## Upload to the board

```sh
mpremote cp test.mpy :lib/
```

## Run

```log
MicroPython v1.24.0-preview.224.g6c3dc0c0b on 2024-08-22; Raspberry Pi Pico W with RP2040
Type "help()" for more information.
>>> import test
>>> test.add(3, 4)
7
>>> test.add(10, 6)
16
```

## Further reading

- https://github.com/wasm3/embedded-wasm-apps
- https://docs.micropython.org/en/latest/develop/natmod.html
- https://github.com/micropython/micropython/issues/15270#issuecomment-2280942885
