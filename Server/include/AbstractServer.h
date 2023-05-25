#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;

class AbstractServer {
public:
    AbstractServer() { }
    virtual ~AbstractServer() { }
    virtual void Run() {}
    virtual void ProcessStuff(string* message) {}

protected:
    static const int BUFFER_LENGTH = 10;
    const int MICROSECONDS_IN_SECOND = 1000000;
    const int NANOSECONDS_IN_SECOND = 1000;
    const int PORT = 8080;
};