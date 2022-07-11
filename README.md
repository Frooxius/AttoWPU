# AttoWPU
This is an experimental processor architecture I developed back in 2010, with an extremelly minimalist approach to programming - flipping bits on its bus.

I am publishing this repo as-is, with the code in its original state from back in 2010, mistakes, unfinished parts, code quality I'm no longer happy with and such, but if you want to poke around, hope you'll have fun with it!

The repo contains a compiler/assembler and a GUI simulator (this one needs Qt).

https://youtu.be/wJXb590IKhM

More info in my Patreon post: https://www.patreon.com/posts/attowpu-cpu-6-68802933

# Overview (from original specification)

AttoWPU is experimental processor architecture from the WPU (Weird Processing Unit) series with accompanying programming languages, which tries different unusual approach the assembly programming and programming in general for various purposes, including education, curiosity, fun and even an artistic intent.

This experimental big endian processing unit allows programmer to design processor's function himself, using special attoassembly language. Processor itself doesn’t have any function at all or direct way to control all its parts using conventional instructions, because there aren’t any built in opcodes for a program. There are however three opcodes for the attocode, allowing to change status of one 64 wires, that is, set it to logical zero, logical one or invert the current status.

64 wires connected directly to the processor’s core are divided into four buses: address bus, control bus, data bus and Quick aJump bus. These buses allow information exchange between various logical blocks/units connected to these buses and control these units by changing values on the buses using three attoinstructions.

The base of the processor is the attocore. During each attocycle, this logical part will read current instruction from attocode memory, decode instruction and bit number from it and change value of specified bit accordingly. Then it will instruct atto Program Counter (aPC) to increment by one, thus, moving to the next attoinstruction. This simple basic process repeats until the processor is stopped.

Programmer can create processor’s function by designing the program in the attocode program memory: by specifically adjusting values of individual wires of the buses, he can control other units in the processor to his needs, usually by creating attocode that will process actual program created in normal assembly language, stored in program+data memory: the attocode will decode instruction opcode (designed by programmer) from the program+data memory and then control processor’s units to execute appropriate function.
