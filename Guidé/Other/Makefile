CXX = g++
CXXFLAGS = -W -Wall -std=c++11 -O2 -g -Wno-sign-compare -Wno-narrowing

PROGS=ExempleInterface IllustrationInterface controleurChoixAction
TESTS=mCalculPoints-test mAnagrammes-test mEquivalence-test vueEnModeTexte-test



all: $(TESTS) $(PROGS)

test:  $(TESTS)
	for test in $(TESTS); do \
	    echo Lancement des tests dans $$test; \
	    ./$$test; \
	done

clean:
	rm -rf *.o $(TESTS) $(PROGS)

ExempleInterface: ExempleInterface.cpp vueEnModeTexte.cpp mCalculPoints.o mDictionnaire.cpp
	$(CXX) $^ -o $@ $(CXXFLAGS)

IllustrationInterface: IllustrationInterface.o vueEnModeTexte.o mCalculPoints.o mDictionnaire.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

controleurChoixAction: controleurChoixAction.o mCalculPoints.o mEquivalence.o mAnagrammes.o mDictionnaire.o mPoseMotJarnac-correction.o mCreeJeuJarnac-correction.o vueEnModeTexte.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

mCalculPoints-test: mCalculPoints-test.o mCalculPoints.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

mEquivalence-test: mEquivalence-test.o mEquivalence.o mAnagrammes.o mDictionnaire.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

mAnagrammes-test: mAnagrammes-test.o mAnagrammes.o mDictionnaire.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

vueEnModeTexte-test: vueEnModeTexte-test.o vueEnModeTexte.o mCalculPoints.o mAnagrammes.o mDictionnaire.o
	$(CXX) $^ -o $@ $(CXXFLAGS)




%.o : %.cpp
	$(CXX) -c $< $(CXXFLAGS)

