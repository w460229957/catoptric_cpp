FROM eclipse-temurin:11
LABEL author='ZhengYuan Zhang'
LABEL email='zhang.zhengyuan@wustl.edu'
LABEL version='1.0'
LABEL description='Dockerfile for building the catoptric array project runtime.'

# Install the build dependencies for the shared library(c++ dependencies)
COPY library_source/dependency.sh /dependency.sh
RUN chmod +x /dependency.sh && /dependency.sh

# Copy the source code to the container
COPY library_source/header/ /header/
COPY library_source/source/ /source/
COPY library_source/Makefile /Makefile

# Build the shared library
RUN make

# Copy java source code to the container
COPY southbridge/ /
COPY resource/ /
RUN mv source/libcatoptric_cpp.so /usr/lib && \
    javac fileReader/*.java controller/*.java  && \
    rm -rf source/ header/ Makefile 
ENTRYPOINT [ "java","fileReader/CatoptricFileReader","-t ","CatoprticProjectTest.txt" ]

