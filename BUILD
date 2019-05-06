VISITOR_INCLS = [
    "include/visitor.h",
    "include/objects.h",
]

cc_library(
    name = "bool",
    srcs = VISITOR_INCLS + ["src/bool.cc"],
    hdrs = [
        "include/bool.h",
        "include/object.h",
    ],
    includes = ["include"]
)

cc_library(
    name = "byte",
    srcs = VISITOR_INCLS + ["src/byte.cc"],
    hdrs = [
        "include/byte.h",
        "include/object.h",
        "include/number.h",
    ],
    includes = ["include"],
)

cc_library(
    name = "int",
    srcs = VISITOR_INCLS + ["src/int.cc"],
    hdrs = [
        "include/int.h",
        "include/object.h",
        "include/number.h",
    ],
    includes = ["include"],
)

cc_library(
    name = "symbol",
    srcs = VISITOR_INCLS + ["src/symbol.cc"],
    hdrs =[
        "include/symbol.h",
        "include/object.h",
    ],
    includes = ["include"],
)

cc_library(
    name = "void",
    srcs = VISITOR_INCLS + ["src/void.cc"],
    hdrs = [
        "include/void.h",
        "include/object.h",
    ],
    includes = ["include"],
)

cc_library(
    name = "error",
    srcs = ["src/error.cc"],
    hdrs = VISITOR_INCLS + [
        "include/error.h",
        "include/object.h",
    ],
    includes = ["include"],
)

cc_library(
    name = "list",
    srcs = VISITOR_INCLS + [
        "src/list.cc",
    ],
    hdrs = [
        "include/list.h",
        "include/object.h",
    ],
    includes = ["include"],
    deps = [
        ":error",
        ":utils",
        ":void"
    ],
)

cc_library(
    name = "array",
    srcs = VISITOR_INCLS + ["src/array.cc"],
    hdrs = [
        "include/array.h",
        "include/object.h",
    ],
    includes = ["include"],
    deps = [":list"],
)

cc_library(
    name = "function",
    srcs = VISITOR_INCLS + [
        "src/function.cc",
    ],
    hdrs = [
        "include/function.h",
        "include/object.h",
        "include/frame.h",
        "include/environment.h",
        "include/interpreter.h",
    ],
    includes = ["include"],
    deps = [":list", ":symbol"],
)
    
cc_library(
    name = "builtins",
    srcs = ["src/builtins.cc"],
    hdrs = [
        "include/builtins.h",
    ],
    includes = ["include"],
    deps = [":function", ":int", ":array", ":bool"],
)

cc_library(
    name = "specialforms",
    srcs = [
        "src/specialforms.cc",
    ],
    hdrs = [
        "include/specialforms.h",
        "include/environment.h",
    ],
    includes = ["include"],
    deps = [
        ":symbol",
    ]
)

cc_library(
    name = "utils",
    srcs = [
        "src/utils.cc",
    ],
    hdrs = [
        "include/utils.h",
    ],
    includes = ["include"],
    deps = [":error"],
)

cc_library(
    name = "reader",
    srcs = [
        "src/reader.cc",
    ],
    hdrs = [
        "include/reader.h",
    ],
    includes = ["include"],
    deps = [
        ":list",
        ":int",
        ":symbol",
    ],
)

cc_library(
    name = "treewalk_interp",
    srcs = [
        "src/treewalk_interp.cc",
        "include/visitor.h",
    ],
    hdrs = [
        "include/treewalk_interp.h",
    ],
    includes = ["include"],
    deps = [
        ":array",
        ":bool",
        ":builtins",
        ":byte",
        ":int",
        ":list",
        ":specialforms",
        ":symbol",
    ]
)

cc_library(
    name = "repl",
    srcs = [
        "src/repl.cc",
    ],
    hdrs = [
        "include/repl.h",
    ],
    includes = ["include"],
    deps = [
        ":error",
        ":reader",
        ":treewalk_interp",
    ]
)

cc_binary(
    name = "main",
    srcs = [
        "main.cc",
    ],
    includes = ["include"],
    deps = [
        ":error",
        ":reader",
        ":repl",
        ":treewalk_interp",
    ],
)
