all:main
main:main.cc Tasks.cc
	g++ -o $@ $^ -std=c++14 -g -lpthread

.PHONY:clean
clean:
	rm -rf main