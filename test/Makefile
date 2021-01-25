BASEDIR:= $(abspath ../../src)
TESTDIR:= $(abspath ../)

suite:
	${MAKE} -C suite

test clean distclean coverage:
	${MAKE} -C suite $@

coverage: suite
	mkdir -p $(BASEDIR)/build/logs
	lcov -o $(BASEDIR)/build/rawcoverage.info -c -d . ;
	lcov -e $(BASEDIR)/build/rawcoverage.info $(BASEDIR)/src/* --path $(BASEDIR) -o $(BASEDIR)/build/report.info;
	genhtml -o $(BASEDIR)/build/coverage --legend --title 'Code Coverage Report' $(BASEDIR)/build/report.info;
	gcovr -f '${BASEDIR}/src/' -r $(BASEDIR) -x -o $(BASEDIR)/build/logs/cobertura.xml
	cp -R */*-Results.xml $(BASEDIR)/build/logs
