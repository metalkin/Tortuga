#for use on Windows:

#MKDIR=mkdir
#RM=del /y

#CXXFLAGS+=-static-libgcc -static-libstdc++ -g -fno-inline-functions -Wall
#CXXFLAGS+=-static-libgcc -static-libstdc++

#export

OUTDIR=out

all: $(OUTDIR)
	$(MAKE) -C common
	$(MAKE) -C server
	$(MAKE) -C client

debug: export CXXFLAGS+=-g
debug: clean all

release: export CXXFLAGS+=-static-libgcc -static-libstdc++
release: clean all

$(OUTDIR):
	mkdir $(OUTDIR)

clean:
	$(RM) *.o *.a *.exe

rebuild: clean all
