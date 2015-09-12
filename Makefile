#Makefile for the Perishable Inventory System

COMPILER = g++
LINKER = g++
WARNINGS = -Wchar-subscripts -Wparentheses -Wreturn-type -Wmissing-braces -Wundef -Wshadow
COMPILER_OPTS = -c -g -Wfatal-errors -Werror $(WARNINGS) -msse2
LINKER_OPTS = -o
EXE = pissim
OBJS = main.o pis.o eventlist.o

$(EXE) : $(OBJS)
	$(LINKER) $(OBJS) $(LINKER_OPTS) $(EXE)

main.o : main.cpp pis.h eventlist.h
	$(COMPILER) $(COMPILER_OPTS) main.cpp

pis.o : pis.cpp pis.h eventlist.h
	$(COMPILER) $(COMPILER_OPTS) pis.cpp

eventlist.o : eventlist.cpp eventlist.h
	$(COMPILER) $(COMPILER_OPTS) eventlist.cpp

clean :
	-rm -rf *.o $(EXE)

tidy :
	-rm -rf doc