all : x64 x86
	
x64 : hook.cpp test_x64
	g++ -shared -fPIC hook.cpp logger.cpp variable.cpp util.cpp -o hook_x64.so -ldl -m64 -D _X64 -std=c++11

x86 : hook.cpp test_x86
	g++ -shared -fPIC hook.cpp logger.cpp variable.cpp util.cpp -o hook_x86.so -ldl -m32 -D _X86 -std=c++11

test_x64 : test.c
	gcc -o test_x64 test.c

test_x86 : test.c
	gcc -o test_x86 test.c -m32

clean :
	rm *.so test_x64 test_x86
