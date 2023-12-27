.SUFFIXES:
.SUFFIXES: .o .cpp

CXX = g++
# CXXFLAGS = -W -Wall -std=c++11 -O2 -g -Wno-sign-compare -Wno-narrowing
CXXFLAGS = -O2 -w -Wall -Iinclude -g -Wno-sign-compare -Wno-narrowing
OBJDIR = object
INCDIR = include
vpath %.cpp src
vpath %.hpp include

PROGS=Jarnac Demo
debug = $(addprefix $(OBJDIR)/, Debug.o)
dependDebug = $(addprefix $(INCDIR)/, Debug.hpp)
objects = $(addprefix $(OBJDIR)/, mCalculPoints.o mDictionnaire.o vueEnModeTexte.o )
depend = $(addprefix $(INCDIR)/, mCalculPoints.hpp mDictionnaire.hpp vueEnModeTexte.hpp)

jeu = $(addprefix $(OBJDIR)/, Jeu.o controleurChoixAction.o )
jeudepend = $(addprefix $(INCDIR)/, controleurChoixAction.hpp)
demo = $(addprefix $(OBJDIR)/, Temp.o loadIA.o)
demodepend = $(addprefix $(INCDIR)/, loadIA.hpp)

all: $(PROGS)

clean:
	rm -rf $(OBJDIR)/*.o $(PROGS)

Jarnac: $(objects) $(debug) $(jeu)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation jeu terminée"

Demo:  $(debug) $(demo)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation demo terminée"

$(OBJDIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
