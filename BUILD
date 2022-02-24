cc_library(
  name = "symphony-mpc",
  deps = [ "@emp-sh2pc//:emp-sh2pc", "@spdz//:SPDZ" ],
  srcs = glob([ "src/**/*.cpp" ]),
  hdrs = glob([ "src/**/*.h" ]),
  copts = [ "-maes" ],
  strip_include_prefix = "src",
)

cc_binary(
  name = "add",
  deps = [ ":symphony-mpc" ],
  srcs = [ "examples/add.c" ],
)