# 6502 SystemC CPU Simulation

This project is for educational purposes (primarily my own education) and
implements a cycle-accurate simulation of the MOS 6502 CPU using C++ and
SystemC. It models instruction execution and memory transactions with timing
that reflects the original hardware.

## Features

- Complete official instruction set
- Cycle-accurate execution
- 4x faster than real-time (on an M1 Pro Chip)
- Only ~1k loc
- Modular opcode and addressing mode handling
- Per-instruction cycle logging
- Complete test suite: execute with `make test`
- Passes [Klaus Dormann's 6502 test suite](https://github.com/Klaus2m5/6502_65C02_functional_tests): execute with `make kd_test`

## Next Steps

- Allow connecting peripherals
- Improve documentation
