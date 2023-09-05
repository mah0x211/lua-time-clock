TARGET=src/clock.$(LIB_EXTENSION)
SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.$(LIB_EXTENSION))
GCDAS=$(OBJS:.so=.gcda)
INSTALL?=install

ifdef TIME_CLOCK_COVERAGE
COVFLAGS=--coverage
endif

.PHONY: all install

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(WARNINGS) $(COVFLAGS) $(CPPFLAGS) -o $@ -c $<

%.$(LIB_EXTENSION): %.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS) $(PLATFORM_LDFLAGS) $(COVFLAGS)

install: $(OBJS)
	rm -f $(GCDAS)
	$(INSTALL) -d $(INST_LIBDIR)/time/clock/
	$(INSTALL) $(TARGET) $(INST_LIBDIR)/time/
	$(INSTALL) $(filter-out $(TARGET), $(OBJS)) $(INST_LIBDIR)/time/clock/
	rm -f ./src/*.o $(OBJS)
