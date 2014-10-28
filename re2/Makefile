include ../MakeGlobals

CFLAGS += -I. -I.. -DNO_THREADS

TARGET = libre2.a

OBJS = bitstate.o compile.o dfa.o filtered_re2.o \
	mimics_pcre.o nfa.o onepass.o parse.o \
	perl_groups.o prefilter.o prefilter_tree.o prog.o \
	re2.o regexp.o set.o simplify.o \
	tostring.o unicode_casefold.o unicode_groups.o \
	util$(DIRSEP)c_helper.o util$(DIRSEP)hash.o \
	util$(DIRSEP)rune.o util$(DIRSEP)stringpiece.o \
	util$(DIRSEP)stringprintf.o util$(DIRSEP)strutil.o \
	util$(DIRSEP)valgrind.o

.PHONY: all
all: $(TARGET)

%.o : %.cc
	$(CC) $(CFLAGS) $^ -c -o $@

$(TARGET) : $(OBJS)
	$(AR) $@ $^

clean:
	$(RM) $(TARGET) $(OBJS)
