#ifdef __linux__ 
#include "../include/LinuxServer.h"
#elif _WIN32
#include "../include/WindowsServer.h"
#endif 

using namespace std;

int main()
{
	auto server = new Server();
	server->Run();
	return 0;
}
