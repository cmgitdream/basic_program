all:write direct

write:write.o io.o
	g++ -o write write.cc io.cc
direct:direct.o io.o
	g++ -o direct direct.cc io.cc


clean:
	rm -rf *.o write direct
