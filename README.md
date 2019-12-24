# Experiments with JVMTI agents

This project contains some random experiments with JVMTI agents. You need bazel to build them:

https://docs.bazel.build/versions/master/install.html

More information about JVMTI is here:

https://docs.oracle.com/javase/8/docs/platform/jvmti/jvmti.html

## Compiling the agent

This needs different header files depending on platform, for now you have to tell Bazel which platform you care about. Example:

Build for linux:

```
bazel build --define jni_platform=linux agent:agent
```

Build for Mac OS (darwin), the default:

```
bazel build --define jni_platform=darwin agent:agent
```

It might also require you to install the JDK if you don't already have it.

## Running with the sample

Build the sample, then invoke it with the agent:

```
bazel build java/org/dubh/bond/example:Simple_deploy.jar
java -agentpath:bazel-bin/agent/libagent.so -jar bazel-bin/java/org/dubh/bond/example/Simple_deploy.jar 
```

This will run the simple app, and dump all invoked methods, along with their declaring class and signature.

## Caveat

I haven't necessarily handled errors or memory well in this sample, still just playing around...
