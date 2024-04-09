This is the implementation of Example module. Repo contains example_module libraries and a python client.

This module serves as an example for module maintainers to implement their own modules outside reserved range of modules. It is not intended to be used in production.
Module number of this module is 1000.

# Requirements

- fleet protocol library - [v2.0.0](https://github.com/bringauto/fleet-protocol/releases/tag/v2.0.0)

# Build

```bash
mkdir _build && cd _build
cmake .. -DCMLIB_DIR=<path-to-cmakelib-dir>
make
```

> cmakelib directory is absolute path to [this](https://github.com/cmakelib/cmakelib) repo root directory.

Now shared libraries `example_module_module_manager.so` and `example_module_external_server.so` should be compiled in current directory. Use `example_module_module_manager.so` with module gateway and `example_module_external_server.so` with external server.

# Example module client
See [client readme](./python_client/README.md)
