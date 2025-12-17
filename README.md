# RISC-V Microkernel

A fully functional microkernel implementation for RISC-V architecture built on xv6 foundation, offering core OS components.

## ⭐ Features

### Memory Management
- Custom memory allocator optimized for kernel-level operations
- Efficient heap management for dynamic memory allocation

### Process Management
- Complete process lifecycle management
- Process creation, termination, and state tracking
- Support for multiple concurrent processes

### Context Switching
- Hardware-accelerated context switches using RISC-V capabilities
- Efficient CPU time-sharing between processes
- Minimal overhead during process transitions

### Concurrency Control
- Robust synchronization primitives
- Multithreading support with thread-safe operations

### Scheduler
- Fair CPU time allocation
- Standard process blocking and wake-up
- Timer-based process suspension and resumption

### Asynchronous I/O
- Non-blocking console input operations
- Buffered console output for improved performance
- Interrupt-driven I/O handling

## 🏗️ Architecture and APIs

This project follows the microkernel design philosophy, keeping the kernel minimal while providing essential services. It is composed of different abstraction layers, each relying on the one beneath it, while offering an interface to the one above it:

```
┌─────────────────────────────────────┐
│          User Applications          │
├─────────────────────────────────────┤
│              C++ API                │
├─────────────────────────────────────┤
│               C API                 │ 
├─────────────────────────────────────┤
│                ABI                  │ 
├─────────────────────────────────────┤
│           RISC-V Hardware           │
└─────────────────────────────────────┘
```
- **C++ API**: Object-oriented interface for modern C++ applications
- **C API**: Low-level C interface for maximum compatibility
- **ABI**: Direct system call interface for performance-critical operations


## 🛠️ Getting Started
This project requires a RISC-V cross-compiler and the QEMU emulator.

### 1. Installing Toolchain
- **Ubuntu:**  
```bash
# Update package list
sudo apt update

# Option 1: Bare-metal toolchain (Ubuntu 24.04+)
sudo apt install gcc-riscv64-unknown-elf binutils-riscv64-unknown-elf qemu-system-misc

# Option 2: If unavailable (older Ubuntu versions)
sudo apt install gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu qemu-system-misc
```
- **macOS:**  
```bash
brew tap riscv/riscv
brew install riscv-tools qemu
```
Verify installation with:  
```bash
riscv64-unknown-elf-gcc --version
qemu-system-riscv64 --version
# OR
riscv64-linux-gnu-gcc --version
qemu-system-riscv64 --version
```

### 2. Building and Running

```bash
# Clean the project (optional)
make clean
# Build and run in QEMU emulator
make qemu
```

The `make qemu` command will build the kernel, launch it in the QEMU emulator, and start user program containing tests. From console, user can choose a test to execute.

### Running Your Own Program

To run your own program instead of the tests:

1. Open `main.cpp`
2. Locate the `wrapper` function
3. Replace the `userMain` with your own application code
4. Rebuild and run with `make qemu`

```cpp
// In main.cpp
void wrapper(void*) {  
    // Your code here... 
}
```

## ⚙️ Technical Details

- **Target Architecture**: RISC-V
- **Language**: C/C++ & Assembly
- **Emulation Platform**: QEMU
- **Concurrency Model**: Preemptive, time-shared
- **I/O Model**: Asynchronous, interrupt-driven
