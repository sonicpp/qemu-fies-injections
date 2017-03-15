# QEMU FIES injections

This project shows simple QEMU based injection applications for FIES framework: [github.com/ahoeller/fies](https://github.com/ahoeller/fies).

There are implemented two kinds of semihosting applications:
* CPU injection - inject NOP instruction and change control flow of target application
* RAM injection - change bits of selected memory cells (bit flipping)

Semihosting applications use ARM Angel interface to communicate with QEMU host (like printing messages and exit): [infocenter.arm.com](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0471c/Bgbjhiea.html).

