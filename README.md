# 6502 SystemC CPU Simulation

This project is for educational purposes (primarily my own education) and
implements a cycle-accurate simulation of the MOS 6502 CPU using C++ and
SystemC. It models instruction execution and memory transactions with timing
that reflects the original hardware.

## Features

- Complete official instruction set
- Cycle-accurate execution
- 10× real-time performance on an M1 Pro with full timing fidelity
- Up to 230× real-time when cycle timing is disabled (logic-only mode)
- Only ~1k loc
- Clean SystemC separation: timing driver is fully abstracted so it can run without SystemC
- Modular opcode and addressing mode handling
- Per-instruction cycle logging
- Complete test suite: execute with `make test`
- Passes [Klaus Dormann's 6502 test suite](https://github.com/Klaus2m5/6502_65C02_functional_tests): execute with `make kd_test`

## Architecture

The core logic is completely independent of SystemC. The entire timing
modelling is delegated to a pluggable SimulationDriver interface. This allows
the simulator to operate in two modes:
* A SystemC-driven mode using timed calls for accurate cycle-level timing
* A logic-only mode where simulation proceeds at maximum logical speed without
accounting for timing.

This design makes it easy to swap out simulation backends, or embed the CPU in
other environments.

## Next Steps

* Add support for memory-mapped peripherals
* Improve documentation and usage examples
* Integrate a basic monitor such as Wozmon
