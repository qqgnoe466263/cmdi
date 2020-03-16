#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "console.h"

static void sigsegvhandler(int sig)
{
    exit(0);
}

static void init_cmdi()
{
    signal(SIGSEGV, sigsegvhandler);
}


int main() 
{
    init_cmdi();
    init_cmd();
    run_console();
}
