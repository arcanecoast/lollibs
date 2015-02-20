Utility library for Lands of Lore: The Throne of Chaos
======================================================

Author: metamaker

This library provides utilities to work with various Lands of Lore's formats.
Among supported formats are:
- .ENG, .DIP, .GER, .FRE (see Translation.h)
- .PAK (see Package.h)

Building the library
--------------------

You need CMake (<http://www.cmake.org/>) to build this library. Example of how to build the library is for
Visual Studio's nmake but you can use pretty same steps sequence for any other generator. Building steps are
next:

1. Open command line and change current directory with `cd' command to the directory where you unpacked the library's source code.
2. Create new empty directory `build' and change your working directory to it by running `mkdir build', `cd build'.
3. Run `cmake -G "NMake Makefiles" ..' to create Makefile.
4. Run `nmake' to build the library.
5. Optionally you can run `nmake install' to install newly built library.

Usage examples
--------------

See the /examples folder.

Library structure
-----------------

Library consists of the next folders:
- /examples - examples which show how to use the library.
- /include  - library headers for usage in your application.
- /src      - library sources. They are needed for building the library.
    
License agreement
-----------------

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License version 3 as published
by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>
 
Release history
---------------

20.02.2015:
- Initial release
