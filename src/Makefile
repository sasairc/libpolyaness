#
#    Makefile for libpolyaness
#

VERSION		= 0
PATCHLEVEL	= 1
SUBLEVEL	= 1
LINKERNAME	= libpolyaness.so
SONAME		= $(LINKERNAME).$(VERSION)
REALNAME	= $(LINKERNAME).$(VERSION).$(PATCHLEVEL).$(SUBLEVEL)

SRCS		= $(wildcard *.c)
OBJS		= $(SRCS:.c=.o)

DEFCFLAGS	= -fPIC
DEFLDFLAGS	= -shared		\
		  -Wl,-soname=$(SONAME)

all: $(REALNAME)

$(REALNAME): $(OBJS)
ifeq ($(CMDLINE), 0)
	@echo "  BUILD   $@"
	@$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
else
	$(CC) $(LDFLAGS) $^ -o $@ $(DEFLDFLAGS)
endif

%.o: %.c
ifeq ($(CMDLINE), 0)
	@echo "  CC      $@"
	@$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
else
	$(CC) $(CFLAGS) -c $< -o $@ $(DEFCFLAGS)
endif

install-lib: $(REALNAME)
	install -pd $(LIBDIR)
	install -pm 755 $(REALNAME) $(LIBDIR)/
	ln -sf $(LIBDIR)/$(REALNAME) $(LIBDIR)/$(SONAME)
	ln -sf $(LIBDIR)/$(REALNAME) $(LIBDIR)/$(LINKERNAME)

install-header: polyaness.h
	install -pd $(INCLUDEDIR)
	install -pm 644 polyaness.h $(INCLUDEDIR)/

clean:
	-$(RM) -f $(REALNAME)
	-$(RM) -f $(OBJS)

.PHONY: all		\
	install-lib	\
	install-header	\
	clean
