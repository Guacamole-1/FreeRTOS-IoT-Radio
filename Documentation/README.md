# LPC1769 -- Driver Library for the Embedded Systems for the IoT Course

**Author:** Pedro Sampaio<br>
**Institution:** DEETC -- Instituto Superior de Engenharia de Lisboa<br>
**Course:** Embedded Systems for the Internet of Things<br>
**Copyright:** © 2017--Present Pedro Sampaio. All rights reserved.

------------------------------------------------------------------------

This project provides a complete **Board Support Package (BSP)** and a modular **driver framework** for the **NXP LPC1769 microcontroller**, designed specifically for teaching and developing **Embedded Systems and IoT applications**.

All documentation is automatically generated from the source code using **Doxygen**.

------------------------------------------------------------------------

## 🎓 Academic Context

This library was developed **exclusively for academic use** within the course:

> **Embedded Systems for the Internet of Things**

Its purpose is to support **teaching, learning, and experimentation** in:

-   Embedded system design
-   Hardware abstraction techniques
-   Device driver development
-   Peripheral interfacing
-   IoT-oriented embedded applications

This framework is intended for **educational environments** and laboratory work using **real LPC1769 hardware**, specifically on the LPCXpresso1769 prototyping boards (Revision C and Revision D).

------------------------------------------------------------------------

## 🎯 Educational Goals

Students using this project will learn how to:

-   Design and implement **hardware abstraction layers**
-   Write **reusable device drivers**
-   Interface with **real peripherals**
-   Build **IoT-style applications**
-   Document embedded software professionally

The focus is on understanding **how hardware works**, not just using high-level libraries.

------------------------------------------------------------------------

## 🧱 Software Architecture

The software is structured in layers:

    +-------------------------------+
    |        Application Layer      |   IoT logic, UI, control
    +-------------------------------+
    |            DRIVERS            |   UART, RTC, LCD, Buttons, Flash, Radio
    +-------------------------------+
    |             CMSIS             |   Core access, startup, system layer
    +-------------------------------+
    |         LPC1769 Hardware      |   Registers, pins, peripherals
    +-------------------------------+

This library is **CMSIS-compliant** and follows the ARM Cortex-M software architecture model.
Applications **must not access registers directly**.
All hardware interaction is performed through the provided **drivers**,
which rely on the CMSIS layer for low-level processor access.

------------------------------------------------------------------------

## 📦 Available Drivers

| Driver      | Purpose               |
|------------|-----------------------|
| PIN        | GPIO control          |
| PINCFG     | Pin multiplexing      |
| CLKPWR     | Peripheral clocks     |
| UART       | Serial communication  |
| RTC        | Real-Time Clock       |
| FLASH      | Internal Flash        |
| LCDText    | LCD display           |
| NAVBTN     | Navigation buttons    |
| RDA5807M   | FM radio receiver     |

------------------------------------------------------------------------

## 🧪 Example

``` c
NAVBTN_Init();
RDA5807M_Init();
UART_Initialize(115200);
```

------------------------------------------------------------------------

## 🛠 Generating the Documentation

To generate the documentation:

``` bash
doxygen Doxyfile
```

Then open:

    html/index.html

------------------------------------------------------------------------

## ⚠️ Usage Restrictions & Disclaimer

This software is distributed:

> **AS IS**, without any warranty of any kind.

### Important:

-   This project is **for academic and educational use only**
-   It is **not intended for commercial, industrial, or safety-critical
    applications**
-   It may contain **simplifications** and **didactic design choices**
-   Performance and robustness were not prioritized over **clarity and
    learning value**

❗ **Commercial use of this library is not permitted.**

The authors assume **no responsibility or liability** for any damage,
malfunction, or loss resulting from its use outside its intended
academic context.

------------------------------------------------------------------------

## ⚖ Copyright Notice

This software and its documentation are the intellectual property of
Pedro Sampaio and are provided strictly for **academic use** within the
context of the course *Embedded Systems for the Internet of Things* at
DEETC -- ISEL.

Unauthorized commercial use, redistribution for profit, or incorporation
into commercial products is prohibited without explicit written
permission from the author.

------------------------------------------------------------------------

## 🧠 Design Philosophy

This framework prioritizes:

-   **Clarity over optimization**
-   **Learning value over performance**
-   **Readable driver structure**
-   **Direct understanding of hardware operation**

Some implementations may intentionally trade efficiency for
**pedagogical transparency**.

------------------------------------------------------------------------

## 🔗 References

-   NXP LPC1769 User Manual
-   RDA5807M Datasheet
-   Doxygen
-   CMSIS
-   Pedro Sampaio, *Sistemas Embebidos — Course Materials*, DEETC – ISEL  
-   Pedro Sampaio, *Sistemas Embebidos para a Internet das Coisas — Course Materials*, DEETC – ISEL  