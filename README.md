# Introduction

The Example Module is an illustrative module for BringAuto's [Fleet Protocol](https://github.com/bringauto/fleet-protocol). It serves as a template for module maintainers to implement their own modules outside the reserved range. **Note**: This module is not intended for production use.

## Identification and Purpose

Module number/ID: `1000`

### Device list

| **Device Name** | **Device Type** | **Device Roles** | Comment                                     |
| --------------- | --------------- | ---------------- | ------------------------------------------- |
| button1         | 0               | test             | Button driving a state of a dedicated LED   |
| button1         | 10              | test             | Button driving a state of a dedicated LED   |
| button2         | 0               | test2            | Button driving a state of a dedicated LED   |

**For better understanding of the module functionality and how to implement your own modules, see [module documentation](./docs/example_module.md).**

## Dependencies

- [CMakeLib](https://github.com/cmakelib/cmakelib)

## Build

```bash
mkdir _build && cd _build
cmake .. -DCMLIB_DIR=<CMAKELIB_DIR>
make
```

> cmakelib directory is absolute path to [this](https://github.com/cmakelib/cmakelib) repo root directory.

Now shared libraries `example_module_module_manager.so` and `example_module_external_server.so` should be compiled in current directory. Use `example_module_module_manager.so` with module gateway and `example_module_external_server.so` with external server.

## Configuration

This module does not require any configuration.

## Example module client

See [client readme](./python_client/README.md)
