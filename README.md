# jakOS

A simple hobby operating system built from scratch for educational purposes.  
The project is currently in an early learning and experimental phase.

Supports booting on **x86 32-bit** systems (x86_64 planned for the future).  
The main goal is to build a modular, well-structured OS from the ground up.

---

## ✅ Current Status

| Feature                                    | Status        |
|-------------------------------------------|--------------|
| Complete bootloader and kernel entry      | ✅ Done       |
| Physical Memory Manager (PMM)             | ✅ Done       |
| Virtual Memory Manager (VMM)              | ✅ Done       |
| Dynamic Heap Memory Allocator             | ✅ Done       |
| Basic Keyboard Driver                     | ✅ Done       |
| PCI device scanning (for filesystem prep) | 🛠️ In progress |
| Filesystem architecture                   | ⏳ Planned    |
| Convert to 64-bit (x86_64)                | ⏳ Planned    |

---

## 🛠️ Future Goals

- Finish PCI device scanning and driver detection
- Design and implement filesystem architecture
- Build basic user-mode support and process management
- Create a simple shell / CLI
- Add basic hardware drivers (e.g., storage, display)
- Port to 64-bit architecture

---

## 📦 Build & Run

> ⚠️ **Note:** The OS is in an experimental stage and primarily tested under emulators like QEMU or VirtualBox.

```bash
git clone https://github.com/TumanyanH/jakOS.git
cd jakOS
make
```

> ⚠️ Since project is widly using hardware emulation and virtualization, please make sure to check the most up-to-date **make run** and QEMU run commands before running.

# 📚 About
jakOS is being built as part of a journey to deeply understand operating system fundamentals:
bootloaders, memory management, hardware abstraction, drivers, and filesystem design.
