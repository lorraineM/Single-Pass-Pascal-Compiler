#include <iostream>
#include "symboltable.h"

symboltable st;

int main() {
	int a = st.name_unit("name");
	std::cout<<a;
	return 0;
}