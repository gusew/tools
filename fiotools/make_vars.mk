# |
# | *** Compiler and Flag configuration ***
# |
CC 			= g++

# -std=c++11  => rely on language features from c++11
CVERSION	= -std=c++11
# -Wall -Wextra -Werror -pedantic  => be strict, force clean and robust code
CWARNME		= -Wall -Wextra -Werror -pedantic -fmax-errors=3
# -fPIC  => produce position-independent code, enable use of shared objects
# -fmax-error=3  => stop after 3 errors were detected
# -Wl,--export-dynamic  => add all symbols to dynamic symbol table, needed to access from dlopened-algorithm-library to global variables (memRegistryPtr)
CSHLIB		= -fPIC -Wl,--export-dynamic
# -shared => produce a shared object, which can be linked to other objects
CSOFLAG		= -shared
# enable pthread functionality
CTHREAD		= -pthread
# compiler optimization level
COPTIM		= -O3
# link static against libstdc++.so (in order to run on machines where g++ is not installed)
CSTATIC		= -static-libstdc++

# final variable for flag definition
CFLAGS 		= $(CVERSION) $(CWARNME) $(COPTIM) $(CSTATIC)

# |
# | *** Program name and build directory ***
# |
APPNAME		= fio
BUILD		= ./build/
SRC 		= ./src/

# |
# | *** Configuration of other tools ***
# |

# flags for running valgrind (memcheck)
VALFLAGS	= --tool=memcheck --leak-check=yes

# general command for creating a directory for files
MKDIR		= @mkdir -p $(@D)

