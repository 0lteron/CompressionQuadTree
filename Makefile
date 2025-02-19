ARCHIVE := L3.2024.ProgC-Maloum-Benesby
LIB 	  := ./libqtc
APP 	  := ./app

arch:
	tar -czf $(ARCHIVE).tar.gz $(LIB) $(APP) Readme.txt