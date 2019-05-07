# Test to verify gtest works
cc_test(
    name = "meta_test",
    srcs = ["meta_test.cc"],
    includes = ["external/gtest/include"],
    deps = ["@gtest//:gtest"],
)

cc_test(
    name = "bool_test",
    srcs = ["bool_test.cc"],
    includes = ["external/gtest/include"],
    deps = [
        "@gtest//:gtest",
        "//:bool",
    ],
)

cc_test(
    name = "byte_test",
    srcs = ["byte_test.cc"],
    includes = ["external/gtest/include"],
    deps = [
        "@gtest//:gtest",
        "//:byte",
    ],
)

cc_test(
    name = "int_test",
    srcs = ["int_test.cc"],
    includes = ["external/gtest/include"],
    deps = [
        "@gtest//:gtest",
        "//:int",
    ],
)


cc_test(
    name = "symbol_test",
    srcs = ["symbol_test.cc"],
    includes = ["external/gtest/include"],
    deps = [
        "@gtest//:gtest",
        "//:symbol",
    ],
)

# cc_test(
#     name = "void_test",
#     srcs = ["void_test.cc"],
#     includes = ["external/gtest/include"],
#     deps = [
#         "@gtest//:gtest",
#         "//:void",
#     ],
# )

# cc_test(
#     name = "error_test",
#     srcs = ["error_test.cc"],
#     includes = ["external/gtest/include"],
#     deps = [
#         "@gtest//:gtest",
#         "//:error",
#     ],
# )

cc_test(
    name = "list_test",
    srcs = ["list_test.cc"],
    includes = ["external/gtest/include"],
    deps = [
        "@gtest//:gtest",
        "//:int",
        "//:list",
    ],
)

cc_test(
    name = "array_test",
    srcs = ["array_test.cc"],
    includes = ["external/gtest/include"],
    deps = [
        "@gtest//:gtest",
        "//:array",
        "//:int",
        "//:list",
    ],
)

# MISSING TEST
# cc_test(
#     name = "function_test",
#     srcs = ["function_test.cc"],
#     includes = ["external/gtest/include"],
#     deps = [
#         "@gtest//:gtest",
#         "//:function",
#     ],
# )

cc_test(
    name = "builtins_test",
    srcs = ["builtins_test.cc"],
    includes = ["external/gtest/include"],
    deps = [
        "@gtest//:gtest",
        "//:bool",
        "//:builtins",
        "//:int",
        "//:list",
    ],
)

# MISSING TEST
# cc_test(
#     name = "specialforms_test",
#     srcs = ["specialforms_test.cc"],
#     includes = ["external/gtest/include"],
#     deps = [
#         "@gtest//:gtest",
#         "//:specialforms",
#     ],
# )

# MISSING TEST
# cc_test(
#     name = "utils_test",
#     srcs = ["utils_test.cc"],
#     includes = ["external/gtest/include"],
#     deps = [
#         "@gtest//:gtest",
#         "//:utils",
#     ],
# )

cc_test(
    name = "reader_test",
    srcs = ["reader_test.cc"],
    includes = ["external/gtest/include"],
    deps = [
        "@gtest//:gtest",
        "//:int",
        "//:list",
        "//:reader",
        "//:symbol",
    ],
)

cc_test(
    name = "treewalk_interp_test",
    srcs = ["treewalk_interp_test.cc"],
    includes = ["external/gtest/include"],
    deps = [
        "@gtest//:gtest",
        "//:bool",
        "//:error",
        "//:function",
        "//:int",
        "//:list",
        "//:treewalk_interp",
        "//:symbol",
    ],
)

# MISSING TEST
# cc_test(
#     name = "repl_test",
#     srcs = ["repl_test.cc"],
#     includes = ["external/gtest/include"],
#     deps = [
#         "@gtest//:gtest",
#         "//:repl",
#     ],
# )
