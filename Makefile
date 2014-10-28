include MakeGlobals

TARGET = runtest$(EXEC)

COBJS = main.o onig.o pcre.o re2.o tre.o 
CPPOBJS = std_regex.o boost_regex.o
CSOURCE = $(patsubst %.o, %.c, ${COBJS})
LIBS = pcre$(DIRSEP)libonig.a pcre$(DIRSEP)libpcre.a re2$(DIRSEP)libre2.a tre$(DIRSEP)libtre.a

.PHONY: all
all: $(TARGET)

$(TARGET) : $(COBJS) $(CPPOBJS) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(COBJS) $(CPPOBJS) -Lonig -lonig -Lpcre -lpcre -Lre2 -lre2 -Ltre -ltre -lboost_regex

$(COBJS) : %.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(CPPOBJS) : %.o: %.cpp
	$(CPP) -c $< -o $@ $(CPPFLAGS)

.PHONY: pcre$(DIRSEP)libonig.a
pcre$(DIRSEP)libonig.a :
	$(MAKE) -C onig

.PHONY: pcre$(DIRSEP)libpcre.a
pcre$(DIRSEP)libpcre.a :
	$(MAKE) -C pcre

.PHONY: re2$(DIRSEP)libre2.a
re2$(DIRSEP)libre2.a :
	$(MAKE) -C re2

.PHONY: tre$(DIRSEP)libtre.a
tre$(DIRSEP)libtre.a :
	$(MAKE) -C tre

clean:
	$(RM) -f $(TARGET) $(COBJS) $(CPPOBJS)
	
