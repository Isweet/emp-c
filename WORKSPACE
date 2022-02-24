# External Dependencies from HTTPS / Git
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# External Dependencies from Nix
http_archive(
  name = "io_tweag_rules_nixpkgs",
  strip_prefix = "rules_nixpkgs-bf6da6adf97c2dae6b477a0f8266c56025fc8cec",
  urls = [ "https://github.com/tweag/rules_nixpkgs/archive/bf6da6adf97c2dae6b477a0f8266c56025fc8cec.tar.gz" ],
)

load("@io_tweag_rules_nixpkgs//nixpkgs:repositories.bzl", "rules_nixpkgs_dependencies")
rules_nixpkgs_dependencies()
load("@io_tweag_rules_nixpkgs//nixpkgs:nixpkgs.bzl", "nixpkgs_git_repository", "nixpkgs_package", "nixpkgs_cc_configure")

# Fix Nixpkgs version
nixpkgs_git_repository(
  name = "nixpkgs",
  revision = "21.11", # Any tag or commit hash
)

# Nix: Boost
nixpkgs_package(
  name = "boost.out",
  repository = "@nixpkgs",
)

nixpkgs_package(
  name = "boost.dev",
  repository = "@nixpkgs",
  build_file_content = """\
load("@rules_cc//cc:defs.bzl", "cc_library")
filegroup(
  name = "include",
  srcs = glob(["include/**/*.h", "include/**/*.hpp", "include/**/*.ipp"]),
  visibility = ["//visibility:public"],
)
""",
)

# Nix: MPIR
nixpkgs_package(
  name = "mpir",
  repository = "@nixpkgs",
)

# Nix: OpenSSL
nixpkgs_package(
  name = "openssl_3_0.out",
  build_file_content = """\
load("@rules_cc//cc:defs.bzl", "cc_library")
filegroup(
  name = "lib",
  srcs = [ "lib/libcrypto.dylib" ],
  visibility = ["//visibility:public"],
)
""",
  repository = "@nixpkgs",
)

nixpkgs_package(
  name = "openssl_3_0.dev",
  repository = "@nixpkgs",
)

# Nix: Sodium
nixpkgs_package(
  name = "libsodium.out",
  repository = "@nixpkgs",
)

nixpkgs_package(
  name = "libsodium.dev",
  repository = "@nixpkgs",
)

# SimpleOT
git_repository(
  name = "symphony-SimpleOT",
  remote = "https://github.com/Isweet/symphony-SimpleOT.git",
  commit = "19e4a863e2b81df231591aa3f489a649ca689779",
)

# EMP

## EMP-TOOL
git_repository(
  name   = "emp-tool",
  remote = "https://github.com/Isweet/symphony-emp-tool.git",
  commit = "7e244f762895591bfcaf953cf64ccc4cfa3d0d93",
)

## EMP-OT
git_repository(
  name   = "emp-ot",
  remote = "https://github.com/Isweet/symphony-emp-ot.git",
  commit = "59be60808ac09f0daea2dbfe8c41999e28c3cdda",
)

## EMP-SH2PC
git_repository(
  name   = "emp-sh2pc",
  remote = "https://github.com/Isweet/symphony-emp-sh2pc.git",
  commit = "31d6440d7ccb315c7d0a8dafe38fe3a1ae7b8d6a",
)

# SPDZ
git_repository(
  name = "spdz",
  remote = "https://github.com/Isweet/symphony-spdz.git",
  commit = "1cc7072f2e4585acbf3da7b2e4c044fd8e2b0697",
)
