CPP = g++ -c
CFLAGS = -g -Wall
LINK = g++
OBJECTS = classifier.o decision_rule_classifier.o main.o\
		  matrix.o parzen_window.o util.o 

all: machine_learn

machine_learn: $(OBJECTS)
	$(LINK) -o $@ $(OBJECTS)

%.o : %.cpp
	$(CPP) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o
	rm -rf *.d
	rm -rf machine_learn
	rm -rf tags

tags: $(wildcard   *.cpp) $(wildcard   *.h)
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q

include $(wildcard   *.d)
