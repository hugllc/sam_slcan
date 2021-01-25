

test coverage:
	${MAKE} -C test $@

clean:
	rm -Rf build/
	${MAKE} -C test $@

distclean:
	rm -Rf build/
	${MAKE} -C test $@


.PHONY: test
