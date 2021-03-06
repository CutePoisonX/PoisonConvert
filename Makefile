CXX=g++ -I/opt/include -L/lib/ -I./tclap-1.2.1/include/
CXXFLAGS=-c -Wall -g
LDFLAGS=
SOURCES = $(wildcard *.cpp)
OBJECTS = ${SOURCES:.cpp=.o}

CONFDESTDIR = /usr/syno/etc
DESTDIR = /usr/syno/bin
CONFDIRNAME = PoisonConvert_Config-files
SETTINGS_FILE = /usr/syno/etc/PoisonConvert_Settings

TITLE = poisonconvert
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

install:
	install --mode=755 $(TITLE) $(DESTDIR)/
	cp -r $(CONFDIRNAME)/ $(CONFDESTDIR)/$(CONFDIRNAME)/

uninstall:
	rm $(DESTDIR)/$(TITLE)
	rm $(SETTINGS_FILE)
	rm -r $(CONFDESTDIR)/$(CONFDIRNAME)

-include $(wildcard ./*.d)
 
