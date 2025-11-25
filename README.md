# custom_shell

This repository contains a small custom Unix-like shell written in C. It provides a starting point for learning about parsing, execution, and basic shell features.

**Quick summary**
- **Language:** C
- **Structure:** Source files in `src/`, headers in `include/`, built binaries in `build/` (if used by the Makefile)

**Features**
- Basic command parsing and execution
- Simple built-ins and executor logic
- Clean, minimal code to study and extend

**Project Layout**
- `Makefile`: build rules
- `src/`: implementation files (`executor.c`, `main.c`, `parser.c`, `shell.c`)
- `include/`: public headers (`executor.h`, `parser.h`, `shell.h`)
- `build/`: (optional) build outputs

Getting started

1. Build

Run the default build target. If your `Makefile` places the binary in `build/`, it will appear there; otherwise a `main` or similar binary may be created at the project root.

```bash
make
```

2. Run the shell

Try the most common locations for the built binary:

```bash
# if binary at project root
./main

# or if Makefile places binary into build/
./build/main
```

If those don't work, inspect the `Makefile` to find the output target name.

Usage examples

- Run a simple command:

```text
$ ./main
> ls -la
<ls output>
> exit
```

- Use pipes and redirections (depending on implemented features):

```text
> grep main src/*.c | wc -l
```

Notes & testing

- The repository is intentionally minimal to be a learning project. If you change the build rules or binary name, update this README accordingly.
- To validate the build instructions, run `make` and then the produced binary. If something fails, open an issue or share the `Makefile` and build output.

Contributing

- Fork the repository and submit pull requests for bug fixes or feature additions.
- Keep functions small and add tests (or example sessions) demonstrating behavior.

License

No license file is included. Add a `LICENSE` if you want to make the project open-source with a specific license.

Contact

Open an issue or reach out in the repo for questions, improvements, or help running the project.
