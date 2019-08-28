fail:
	@echo "Please specify either \"win32\" or \"linux\""

win32:
	make -f makefile.win32

linux:
	make -f makefile.linux