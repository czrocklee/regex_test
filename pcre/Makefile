include ../MakeGlobals

CFLAGS += -DHAVE_CONFIG_H

TARGET = libpcre.a

OBJS = pcre_chartables.o pcre_compile.o pcre_config.o pcre_dfa_exec.o \
	pcre_exec.o pcre_fullinfo.o pcre_get.o pcre_globals.o \
	pcre_jit_compile.o pcre_maketables.o pcre_newline.o pcre_ord2utf8.o \
	pcre_refcount.o pcre_study.o pcre_tables.o pcre_ucd.o \
	pcre_valid_utf8.o pcre_version.o pcre_xclass.o

.PHONY: all
all: $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) $^ -c -o $@

$(TARGET) : $(OBJS)
	$(AR) $@ $^

clean:
	$(RM) $(TARGET) $(OBJS)
