TARGET=clock.$(LIB_EXTENSION)
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS=$(SRCS:.c=.o)
GCDAS=$(OBJS:.o=.gcda)
INSTALL?=install

ifdef TIME_CLOCK_COVERAGE
COVFLAGS=--coverage
endif

.PHONY: all install

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(WARNINGS) $(COVFLAGS) $(CPPFLAGS) -o $@ -c $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS) $(PLATFORM_LDFLAGS) $(COVFLAGS)

install:
	$(INSTALL) -d $(INST_LIBDIR)/time/
	$(INSTALL) $(TARGET) $(INST_LIBDIR)/time/
	rm -f $(OBJS) $(TARGET) $(GCDAS)
