load(
    "@bazel_tools//tools/jdk:default_java_toolchain.bzl",
    "java_runtime_files",
)

java_runtime_files(
    name = "java_jvmti_common_headers",
    srcs = [
        "include/jni.h",
        "include/jvmti.h",
    ],
)

java_runtime_files(
    name = "java_jvmti_linux_header",
    srcs = [
        "include/linux/jni_md.h",
    ],
)

java_runtime_files(
    name = "java_jvmti_darwin_header",
    srcs = [
        "include/darwin/jni_md.h",
    ],
)

cc_library(
    name = "agent",
    srcs = ["agent.cpp"],
    hdrs = [":java_jvmti_common_headers"] + select ({
        ":linux_jni": [":java_jvmti_linux_header"],
        ":darwin_jni": [":java_jvmti_darwin_header"],
        "//conditions:default": [":java_jvmti_darwin_header"],
    }),
    includes = [
        "include"
    ] + select({
        ":linux_jni": ["include/linux"],
        ":darwin_jni": ["include/darwin"],
        "//conditions:default": ["include/darwin"],
    })
)

config_setting(
    name = "linux_jni",
    values = {"define": "jni_platform=linux"},
)

config_setting(
    name = "darwin_jni",
    values = {"define": "jni_platform=darwin"},
)