# RPU Simulator
------------------------------------------------------------------------

RPU is a coarse-grain multithreading core, which is time-triggered to be
time-safe. It supports RV32I module and RISC-V TTI module which is a
custom module for real-time applications. Refer to `doc/RISC-V TTI.docx`
to get details about it.

This project is a graphic simulator for RPU. It can give behavior of RPU
for every instruction cycle. The simulator is written by C++ with Qt.

This is a project for research. I design this for better understanding of
real-time cores. After that, we design RPU using SystemVerilog based on
CV32E40P. The RPU rtk code is not open-sourced yet.

After all, the simulator is function-complete. But since it is not designed
for using, it is a little stupid now. I think I will make it better when
the research is good enough.

In order to use it, you need to install Qt. The default RPU has 4 threads
and the TT table has 20 entries. All the threads starts at 0x0000_0000,
and the data memory starts at 0x1000_0000. You need to prepare your test
refer to the existing ones.

## Project Organization
* Folder `doc` contains all the documents you may want to refer.
* Folder `src` contains the source code of the simulator.
* Folder `tests` contains the simple tests of the simulator.