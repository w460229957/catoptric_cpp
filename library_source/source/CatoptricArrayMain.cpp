#include <fstream> // ifstream
#include <iostream> // cin
#include <sstream> // stringstream
#include <algorithm> // transform
#include "../header/CatoptricController.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <cstdio>

int main(int argc, char* argv[]) {
    CatoptricController catCon;
    catCon.run(argc,argv);
    return RET_SUCCESS;
}