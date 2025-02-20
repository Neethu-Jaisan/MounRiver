# MounRiver 
###This Repository contains documenation related to ch3v003 microcontroller, WCH LINKE and example programs.
## RISC V
RISC-V is an open-source, royalty-free instruction set architecture (ISA) based on the principles of reduced instruction set computing (RISC). It was developed at UC Berkeley and has gained widespread adoption due to its flexibility, modularity, and openness.
### Key Features of RISC-V
Open-Source & Free: No licensing fees, allowing anyone to develop and modify processors.
Modular ISA: Comes in base integer variants (e.g., RV32I, RV64I) with optional extensions (e.g., M for multiplication, A for atomic operations, F for floating-point, etc.).
Scalability: Can be used in embedded systems, IoT devices, AI accelerators, and high-performance computing.
Growing Ecosystem: Supported by organizations like RISC-V International, with hardware implementations from companies like SiFive, Andes, and Ventana.
### Common RISC-V Implementations
SiFive Freedom Processors (Used in embedded and industrial applications)
ESP32-C3 (Espressif’s RISC-V-based Wi-Fi + BLE microcontroller)
HiFive Unmatched & Unleashed (Development boards for high-performance applications)
StarFive VisionFive 2 (RISC-V-based single-board computer)
Applications of RISC-V
IoT and embedded devices
AI accelerators
Cloud computing and data centers
Edge computing
Custom silicon development
## nanoCH35V003
The nanoCH32V003 is an ultra-low-cost RISC-V microcontroller from QingKe (WCH), part of the CH32V003 series. It's gaining attention due to its affordability and small form factor, making it a great choice for budget-conscious projects.
### Key Features of nanoCH32V003:
Core: QingKe V2A (RISC-V 32-bit, based on RV32EC)
Clock Speed: Up to 48 MHz
Memory:
16 KB Flash
2 KB SRAM
Peripherals:
GPIOs
USART, I²C, SPI
PWM, ADC
Timers & watchdog
Power Supply: 3.3V
Package: TSSOP-20, QFN20, and SOP-16 (small footprint)
Low Cost: Available for less than $0.10 per chip in bulk!
### Why Consider nanoCH32V003 for Your LoRaWAN Mesh Project?
✅ Ultra-low cost – Ideal for mass deployment
 ✅ Low power – Good for battery-powered LoRa nodes
 ✅ RISC-V architecture – Open-source ecosystem
 ✅ Basic peripherals – Enough for simple control and communication tasks
### Potential Limitations:
❌ Limited Flash & RAM – Might struggle with complex LoRaWAN stacks
 ❌ No built-in LoRa – Needs external SX1276/SX1262 transceiver
 ❌ Limited development tools – SDK is still evolving


## WCH LINK E
The WCH-LinkE is a low-cost debugger and programmer designed for WCH’s RISC-V and ARM microcontrollers, including the CH32V003 series. It is widely used for flashing firmware and debugging RISC-V MCUs from WCH.

### 🔹 Key Features of WCH-LinkE
✅ Supports RISC-V (CH32V) and ARM (CH32F) MCUs
 ✅ SWD and WCH-Link debugging protocols
 ✅ USB interface – Plugs directly into your PC
 ✅ Supports OpenOCD – Can be used in open-source toolchains
 ✅ Compatible with MounRiver IDE – WCH’s official development environment

### 🔹 How It Works
Programming & Debugging: Used to flash firmware onto CH32V003 and other WCH MCUs.
SWD Interface: Allows debugging using breakpoints and stepping through code.
Works with MounRiver Studio: The official WCH IDE for RISC-V development.

### 🔹 Why Use WCH-LinkE for Your LoRaWAN Project?
Since you're working on a LoRaWAN mesh network, if you're using nanoCH32V003 for your nodes or any CH32V series MCU, the WCH-LinkE is an essential tool to program and debug them. It's cheap and simple to use but might have limitations with mainstream debugging tools.
### ⚡ Alternative Debuggers:
WCH-Link (More features, supports CH32F/RISC-V MCUs)
J-Link EDU Mini (For broader MCU support)
CMSIS-DAP Debuggers (Open-source alternative)

https://github.com/openwch/ch32v003
