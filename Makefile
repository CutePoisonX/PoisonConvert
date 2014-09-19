CXX=g++
CXXFLAGS=-c -Wall -g
LDFLAGS=
SOURCES = $(wildcard *.cpp)
OBJECTS = ${SOURCES:.cpp=.o}

CONFDESTDIR = /opt/etc
DESTDIR = /opt/bin
CONFDIRNAME = PoisonConvert_Config-files
SETTINGS_FILE = /opt/etc/PoisonConvert_Settings

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
	rm $(DESTDIR)/$(SETTINGS_FILE)
	rm -r $(CONFDESTDIR)/$(CONFDIRNAME)

-include $(wildcard ./*.d)
 