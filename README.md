# QEMU FIES injections

This project shows simple QEMU based injection applications for FIES framework: [github.com/ahoeller/fies](https://github.com/ahoeller/fies).

There are implemented two kinds of semihosting applications:
* CPU injection - inject NOP instruction and change control flow of target application
* RAM injection - change bits of selected memory cells (bit flipping)

Semihosting applications use ARM Angel interface to communicate with QEMU host (like printing messages and exit): [infocenter.arm.com](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0471c/Bgbjhiea.html).

Some papers about FIES:
* FIES: A Fault Injection Framework for the Evaluation of Self-Tests for COTS-Based Safety-Critical Systems
* QEMU-Based Fault Injection for a System-Level Analysis of Software Countermeasures Against Fault Attacks
* A Fault Injection System Based on QEMU Simulator and Designed for BIT Software Testing

More info about protecting SW againts injections/HW faults can be found in these papers:
* Comprehensive Analysis of Software Countermeasures Against Fault Attacks
* Software Detection Mechanisms Providing Full Coverage Against Single Bit-Flip Faults
* RAM error detection & correction using HVD implementation
* Enforcing Forward-Edge Control-Flow Integrity in GCC & LLVM
* Flipping Bits in Memory Without Accessing Them: An Experimental Study of DRAM Disturbance Errors
* Exploiting the DRAM rowhammer bug to gain kernel privileges ([video](https://www.youtube.com/watch?v=0U7511Fb4to), [source code](https://github.com/google/rowhammer-test))
* Bitsquatting: Exploiting Bit-flips for Fun, or Profit?
* Plundervolt: Flipping Bits from Software without Rowhammer ([video](https://www.youtube.com/watch?v=zKIliM-pHFs), [source code](https://github.com/KitMurdock/plundervolt))

