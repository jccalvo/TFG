CC=gcc

all: ascii2bin bin2ascii

bin: ascii2bin
	for i in *.txt ; do ./ascii2bin < $$i >> $$i.bin ; done

clean:
	$(RM) ascii2bin *.o *~

test: bin2ascii
	for i in *.txt ; do echo $$i ; cat $$i.bin | ./bin2ascii | diff -u $$i - ; done

distclean: clean
	$(RM) *.bin
