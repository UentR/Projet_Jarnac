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
# TESTS=mCalculPoints-test mAnagrammes-test mEquivalence-test vueEnModeTexte-test


objects = $(addprefix $(OBJDIR)/, mCalculPoints.o mDictionnaire.o loadIA.o vueEnModeTexte.o controleurChoixAction.o Debug.o)
depend = $(addprefix $(INCDIR)/, mCalculPoints.hpp mDictionnaire.hpp loadIA.hpp vueEnModeTexte.hpp controleurChoixAction.hpp Debug.hpp)

# all: $(TESTS) $(PROGS)
all: $(PROGS)

# test:  $(TESTS)
# 	for test in $(TESTS); do \
# 	    echo Lancement des tests dans $$test; \
# 	    ./$$test; \
# 	done

clean:
	rm -rf $(OBJDIR)/*.o $(PROGS) Error.txt 
# $(TESTS)

Jarnac: $(objects)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation terminée"

# ExempleInterface: ExempleInterface.cpp vueEnModeTexte.cpp mCalculPoints.o mDictionnaire.cpp
# 	$(CXX) $^ -o $@ $(CXXFLAGS)

# IllustrationInterface: IllustrationInterface.o vueEnModeTexte.o mCalculPoints.o mDictionnaire.o
# 	$(CXX) $^ -o $@ $(CXXFLAGS)

# controleurChoixAction: controleurChoixAction.o mCalculPoints.o mEquivalence.o mAnagrammes.o mDictionnaire.o mPoseMotJarnac-correction.o mCreeJeuJarnac-correction.o vueEnModeTexte.o
# 	$(CXX) $^ -o $@ $(CXXFLAGS)

# mCalculPoints-test: mCalculPoints-test.o mCalculPoints.o
# 	$(CXX) $^ -o $@ $(CXXFLAGS)

# mEquivalence-test: mEquivalence-test.o mEquivalence.o mAnagrammes.o mDictionnaire.o
# 	$(CXX) $^ -o $@ $(CXXFLAGS)

# mAnagrammes-test: mAnagrammes-test.o mAnagrammes.o mDictionnaire.o
# 	$(CXX) $^ -o $@ $(CXXFLAGS)

# vueEnModeTexte-test: vueEnModeTexte-test.o vueEnModeTexte.o mCalculPoints.o mAnagrammes.o mDictionnaire.o
# 	$(CXX) $^ -o $@ $(CXXFLAGS)


$(OBJDIR)/%.o : %.cpp
	@$(CXX) -c $(CXXFLAGS) $< -o $@

Move:
	mv *.o ./object/
