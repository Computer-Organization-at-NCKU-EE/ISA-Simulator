#include "iss.h"
#include "common.h"

int main(int argc, char **argv) {
    // check argc
    Assert(argc == 2, "The number of arguments should be 2");

    // main body
    ISS iss;
    ISS_ctor(&iss, argv[1]);
    ISS_step(&iss, -1);

    // end of main
    ISS_dtor(&iss);
}
