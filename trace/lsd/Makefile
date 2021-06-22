all: test_lsd

#DEBUG=-g -D _DEBUG
DEBUG=-O3

clean:
	rm objects/*.o
	rm src/private_Parserbase.h
	rm src/private_Parser.ih
	rm src/private_parse.cc
	rm lib/liblsd.so
	rm lsdParse

src/private_parse.cc: grammar Makefile
	bisonc++ --parsefun-source=src/private_parse.cc\
		     --baseclass-header=src/private_Parserbase.h\
			 --class-header=src/private_Parser.h\
			 --implementation-header=src/private_Parser.ih\
			 --namespace=lsd_private\
			 grammar

objects/private_parse.o: src/private_parse.cc src/private_Parser.h include/lsd/lsd.h Makefile
	g++ -c $(DEBUG) -fPIC src/private_parse.cc -I./ -I./include -o objects/private_parse.o
objects/Parser.o: src/Parser.cc src/private_Parser.h include/lsd/lsd.h Makefile
	g++ -c $(DEBUG) -fPIC src/Parser.cc -I./ -I./include -o objects/Parser.o
objects/lsdParser.o: src/lsdParser.cc src/private_Parser.h include/lsd/lsdParser.h Makefile
	g++ -c $(DEBUG) -fPIC src/lsdParser.cc -I./ -I./include -o objects/lsdParser.o
lib/liblsd.so: objects/private_parse.o objects/Parser.o objects/lsdParser.o
	g++ -shared objects/private_parse.o objects/Parser.o objects/lsdParser.o -o lib/liblsd.so
lsdParse: lib/liblsd.so src/main.cc
	g++ $(DEBUG) src/main.cc -L./lib -I./ -I./include -Wl,-rpath='$$ORIGIN/lib' -llsd -o lsdParse
test_lsd: lsdParse files/box.lsd
	./lsdParse
