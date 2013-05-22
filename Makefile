CXX=g++
CXXFLAGS=-c -Wall -g
LDFLAGS=
SOURCES = $(wildcard *.cpp)
OBJECTS = ${SOURCES:.cpp=.o}
TITLE= poisonconvert
ARCHIVE=$(TITLE).tar.gz

.PHONY : all clean debug valgrind archive

all: $(TITLE)

$(TITLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< -MMD -MF ./$@.d

clean :
	rm -f ~* *.o *.gch *.d

cleanw :
	del ~*
	del *.o
	del *.gch
	del /q *.d

debug : $(TITLE)
	cgdb ./$<

valgrind : $(TITLE)
	valgrind --tool=memcheck --leak-check=yes ./$<

archive :
	tar cfz $(ARCHIVE) --ignore-failed-read *.cpp *.h *.pdf Makefile

-include $(wildcard ./*.d)
 