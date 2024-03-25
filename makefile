IDIR = .
CC=gcc
CFLAGS= -Wall -ansi -pedantic  -I$(IDIR)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~