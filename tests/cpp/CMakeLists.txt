add_executable(tests pyconcord_t.cpp)
target_link_libraries(tests PUBLIC pyconcord
                                   Catch2::Catch2WithMain)

# allow user to run tests with `make test` or `ctest`
catch_discover_tests(tests)
