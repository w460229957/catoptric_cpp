#!/bin/bash
source_file="."
destination_dir="../../library_source/header"
javac -h . ControllerNative.java
mv controller_ControllerNative.h $destination_dir
