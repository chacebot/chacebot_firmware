######################################################################
#  Makefile for top level rtos subdirectory
######################################################################

PROJS = 

all:	libwwg 
	for proj in $(PROJS) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$proj ; \
	done

clean:	
	for proj in $(PROJS) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$proj clean ; \
	done
	$(MAKE) -$(MAKEFLAGS) -C ./libwwg clean

clobber:
	for proj in $(PROJS) ; do \
		$(MAKE) -$(MAKEFLAGS) -C ./$$proj clobber ; \
	done
	$(MAKE) -$(MAKEFLAGS) -C ./libwwg clean

# End
