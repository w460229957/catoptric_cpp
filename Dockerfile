FROM alpine:latest
LABEL author='ZhengYuan Zhang'
LABEL email='zhang.zhengyuan@wustl.edu'
LABEL version='1.0'
LABEL description='Dockerfile for building the catoptric_array project runtime.'

#Copy the necessary java runtime to the container(Java dependency)
ENV JAVA_HOME=/opt/java/openjdk
COPY --from=eclipse-temurin:11 $JAVA_HOME $JAVA_HOME
ENV PATH="${JAVA_HOME}/bin:${PATH}"

# Install build tools needed for compiling the shared library using g++(C++ dependency)
RUN  apk update && apk add --update g++ make \
    && rm -rf /var/cache/apk/*

# Copy the source code to the build container
COPY library_source/header/ /header/
COPY library_source/source/ /source/
COPY library_source/Makefile /Makefile

# Compile the shared library
RUN make


COPY server/source server/source
WORKDIR /server/source
RUN javac JNI.java
CMD ["java","-Djava.library.path=/server/source","JNI"]

