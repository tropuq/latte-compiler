all:
	$(MAKE) -C lib
	$(MAKE) -C src
	mv src/latc_x86_64 .
	ln -sf latc_x86_64 latc

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C src clean
	rm -f latc latc_x86_64
