# c-dynamic-input-learning

A small C learning project that reads user input and detects whether it is:

- an integer
- a floating-point number
- a string

The program then stores the value in a C `union`-based variant type and prints it.

## Project structure

- `variante_data.c` - main source file containing type detection, parsing, and output logic.

## Build

From the repository root:

```bash
gcc -Wall -Wextra -std=c11 variante_data.c -o variant_reader
```

## Run

```bash
./variant_reader
```

Example interaction:

```text
write or give any type of data you want..(must be 19 char under or number)
123
123
```

## Notes

- Input is read with `scanf("%19s", buffer)`, so the maximum accepted length is 19 characters.
- The current parser supports unsigned integer-like and simple decimal float formats, plus plain strings.