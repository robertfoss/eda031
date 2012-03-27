DIRS	= client server

.PHONY: all server client clean


all: 
	-for d in $(DIRS); do (cd $$d; $(MAKE) all ); done

clean:
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done
	$(RM) -rf bin/
