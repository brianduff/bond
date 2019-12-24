load(
    "@bazel_tools//tools/jdk:default_java_toolchain.bzl",
    "java_runtime_files",
)

java_runtime_files(
    name = "java_jvmti_header",
    srcs = [
        "include/darwin/jni_md.h",
        "include/jni.h",
        "include/jvmti.h",
    ],
)

cc_library(
    name = "agent",
    srcs = ["agent.cpp"],
    hdrs = [":java_jvmti_header"],
    includes = [
        "include",
        "include/darwin",
    ],
)
