#pragma once

#include <termios.h>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <errno.h>

int prep_serial(const char *port_str_in);
