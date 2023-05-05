#!/bin/bash
docker run --privileged -it --rm \
        -v  /dev/serial/by-id:/dev/serial/by-id/ \
        -v  ~/Desktop/CatoptricSurfaceTest/:/fileReader/nottest --network=host catoptric_array