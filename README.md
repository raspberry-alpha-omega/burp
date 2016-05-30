BURP
====

BURP (Basic Using Reverse Polish) is an odd niche programing language from the late 1970s which was used as the system language of the little-known "Powertran Psi-Comp 80" kit-built home computer. On the surface it appears a bit like a fairly limited early BASIC, but it uses RPN instead of the traditional infix notation for expressions.

BURP was designed for a specific machine architecture which used both a general-purpose miccroprocessor (Zilog Z80) with a specialist "Number-Oriented Processor" (National Semiconductor MM57109) to handle all the maths. The constraints of the MM57109, which was essentially a dedicated calculator chip with a 4-bit microcontroller interface bus, strongly affected the design of the language.

Significant features of the language include a fixed set of 26 numeric variables (A-Z), a maximum of 256 lines of code, a three-deep hardware stack for expressions, a limit of one for-loop at a time (so no nesting, not even in GOSUBs) and no string processing (except implicit concatenation of static text and numbers during output.)

This Project
------------

The intention of this project is to build enough of a simulator to run the few remaining published programs for this language, and to allow experimentation with new ones, even on very limited (by modern standards) hardware. To achieve this, the project consists of several phases.

Phase 1 (in progress)
-------

Build a simulator for the MM57109 Number-Oriented Processor in C, together with a suite of unit tests. The key here is to prove that it works and that I understand all the ins and outs of how it should work. No particular emphasis on code size or speed.

Phase 2
-------

Build a simple language interpreter for BURP, using the MM57109 code from phase 1. This phase is also a proof of concept rather than tight production code.

Phase 3
-------

Work on size, speed and portability, to see just how small a system can run the MM57109 and BURP C code.

Phase 4 (optional)
------------------

Convert some or all of the code to assembly for a modern embedded microcontroller (ARM, AVR, etc.) if the C code won't go quite small enough.

Phase 5 (very optional)
-----------------------

Build a minimal microcontroller board to actually implement this wonky architecture, and run the code from phase 4 on it!

References
----------

* [Articles about BURP and the "Psi Comp 80" computer from Wireless World (1979-)](http://vintagecomputers.site90.net/comp80/)
* [MM57109 data sheet](http://www.datasheetarchive.com/dl/Scans-063/DSA2IH00122414.pdf)
* [The blog post which introduced me to the concept of BURP](https://sustburbia.blogspot.co.uk/2016/05/and-i-have-never-seen-star-wars.html) 

