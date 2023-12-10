.SUFFIXES:
.SUFFIXES: .o .cpp

CXX = g++
# CXXFLAGS = -W -Wall -std=c++11 -O2 -g -Wno-sign-compare -Wno-narrowing
CXXFLAGS = -O2 -Iinclude -g -Wno-sign-compare -Wno-narrowing
OBJDIR = object
INCDIR = include
vpath %.cpp src
vpath %.hpp include

PROGS=Jarnac
TEST = $(addprefix $(OBJDIR)/, test-dico.o, mDictionnaire.o)
dependTest = $(addprefix $(INCDIR)/, test-dico.hpp mDictionnaire.hpp)
debug = $(addprefix $(OBJDIR)/, Debug.o)
dependDebug = $(addprefix $(INCDIR)/, Debug.hpp)
objects = $(addprefix $(OBJDIR)/, mCalculPoints.o mDictionnaire.o loadIA.o vueEnModeTexte.o controleurChoixAction.o)
depend = $(addprefix $(INCDIR)/, mCalculPoints.hpp mDictionnaire.hpp loadIA.hpp vueEnModeTexte.hpp controleurChoixAction.hpp)

all: $(PROGS)

clean:
	rm -rf $(OBJDIR)/*.o $(PROGS) Error.txt 

Jarnac: $(objects) $(debug)
	@echo "Start Compilation"
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation terminée"

Test: $(TEST) $(debug)
	echo "Start Compilation test"
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation test terminée"

$(OBJDIR)/%.o : %.cpp
	@$(CXX) -c $(CXXFLAGS) $< -o $@
