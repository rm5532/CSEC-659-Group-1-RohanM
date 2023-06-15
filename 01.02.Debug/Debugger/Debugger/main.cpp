#include <windows.h>
#include <stdio.h>
void test() { // Step over to walk thru the func.
	int i = 0; // Step over and Watch the value in Autos
	printf("Hello World! test func\n"); // Step Over
	i++; // Step over and watch the value change in Autos
}
int main(void) {
	printf("Hello World!\n"); // set breakpoint and Step Over
	test(); // set breakpoint on this function and Step Into
	return 0;
}
