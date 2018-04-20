
aio:aio.o
	g++ -o aio aio.cc -lpthread
main:main.o thread_pool.o
	g++ -o main main.cc thread_pool.cc -lpthread
test_cond:test_cond.o
	g++ -o test_cond test_cond.cc -lpthread

clean:
	rm -rf *.o aio main test_cond
