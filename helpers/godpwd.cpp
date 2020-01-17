#include <iostream>
#include <string>
#ifdef _WIN32
#include "../src/windows-encrypt.h"
#endif

int main(int argc, char *argv[])
{

	if(argc==2)
	{
		char *out=crypt(argv[1],"k7");
		std::cout << std::string(out);
	}
	else
	{
		std::cout << "You must specify password" << std::endl;
	}

	return 0;
}
