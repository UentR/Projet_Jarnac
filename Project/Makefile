.SUFFIXES:
.SUFFIXES: .o .cpp

CXX = g++
# CXXFLAGS = -W -Wall -std=c++11 -O2 -g -Wno-sign-compare -Wno-narrowing
CXXFLAGS = -O2 -Iinclude -g -Wno-sign-compare -Wno-narrowing
OBJDIR = object
INCDIR = include
vpath %.cpp src
vpath %.hpp include

PROGS=Jarnac Demo
debug = $(addprefix $(OBJDIR)/, Debug.o)
dependDebug = $(addprefix $(INCDIR)/, Debug.hpp)
objects = $(addprefix $(OBJDIR)/, mCalculPoints.o mDictionnaire.o loadIA.o vueEnModeTexte.o controleurChoixAction.o )
depend = $(addprefix $(INCDIR)/, mCalculPoints.hpp mDictionnaire.hpp loadIA.hpp vueEnModeTexte.hpp controleurChoixAction.hpp)

jeu = $(addprefix $(OBJDIR)/, Jeu.o)
jeudepend = $(addprefix $(INCDIR)/, controleurChoixAction.hpp)
demo = $(addprefix $(OBJDIR)/, Demo.o)
demodepend = $(addprefix $(INCDIR)/, controleurChoixAction.hpp)

stats = $(addprefix $(OBJDIR)/, testDico.o, mDictionnaire.o)
dependStats = $(addprefix $(INCDIR)/, testDico.hpp, mDictionnaire.hpp)


all: $(PROGS)

clean:
	rm -rf $(OBJDIR)/*.o $(PROGS)

Jarnac: $(objects) $(debug) $(jeu)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation jeu terminée"

Demo:  $(objects) $(debug) $(demo)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation demo terminée"

Stats: $(debug) $(stats)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation stats terminée"

$(OBJDIR)/%.o : %.cpp
	@$(CXX) -c $(CXXFLAGS) $< -o $@
