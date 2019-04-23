
all: maxprotein test

test: maxprotein_test 
	./maxprotein_test

maxprotein_test: maxprotein.hh rubrictest.hh maxprotein_test.cc
	g++ -std=c++11 maxprotein_test.cc -o maxprotein_test

maxprotein: maxprotein.hh timer.hh maxprotein_main.cc
	g++ -std=c++11 maxprotein_main.cc -o experiment

clean:
	rm -f maxprotein maxprotein_test
