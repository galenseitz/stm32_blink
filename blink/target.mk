.SUFFIXES:

ifndef _ARCH
  _ARCH := STM32F303x8
  export _ARCH
endif

OBJDIR := $(_ARCH:%=_%)
MAKETARGET = $(MAKE) -r --no-print-directory -C $@ -f $(CURDIR)/Makefile \
		SRCDIR=$(CURDIR) DEVICE=$(@:_%=%) $(MAKECMDGOALS)

.PHONY: all
all: $(OBJDIR)

# Force clean if performing a release build.
.PHONY: release
release : clean all

.PHONY: $(OBJDIR)
$(OBJDIR):
	+@[ -d $@ ] || mkdir -p $@
	+@$(MAKETARGET)

Makefile : ;
%.mk :: ;

% :: $(OBJDIR) ; @:

.PHONY: clean
clean:
	rm -rf $(OBJDIR)
