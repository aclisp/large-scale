all:
	@cd radutil; make
	@cd test; make
	@cd test; ./runtest.exe
	@cd radsql; make
	@cd radsock; make
	@cd radfwk; make

clean:
	@cd test; make clean
	@cd radutil; make clean
	@cd radsql; make clean
	@cd radsock; make clean
	@cd radfwk; make clean
