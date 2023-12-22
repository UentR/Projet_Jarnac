.SUFFIXES:
.SUFFIXES: .o .cpp

CXX = g++
CXXFLAGS = -O2 -Iinclude -g -w
OBJDIR = obj
INCDIR = include
vpath %.cpp src
vpath %.hpp include

PROGS=Server Client

objClient = $(addprefix $(OBJDIR)/, Client.o)
dependClient = $(addprefix $(INCDIR)/, Client.hpp)

objServ = $(addprefix $(OBJDIR)/, Server.o)
dependServ = $(addprefix $(INCDIR)/, Server.hpp)

all: $(PROGS)

clean:
	rm -rf $(OBJDIR)/*.o $(PROGS)

Client: $(objClient)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation Client terminée"

Server: $(objServ)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation Server terminée"

$(OBJDIR)/%.o : %.cpp
	@$(CXX) -c $(CXXFLAGS) $< -o $@