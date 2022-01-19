#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include <spdlog/spdlog.h>

auto main(int argc, char **argv) -> int {
    spdlog::set_level(spdlog::level::debug);

    doctest::Context context;
    context.applyCommandLine(argc, argv);

    int res = context.run(); // run

    if (context.shouldExit()) // important - query flags (and --exit) rely on
                              // the user doing this
        return res;           // propagate the result of the tests

    int client_stuff_return_code = 0;

    return res + client_stuff_return_code; // the result from doctest
}
