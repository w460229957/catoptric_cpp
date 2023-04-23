#!/bin/bash
docker run --privileged -it --rm -v  /dev/serial/by-id:/dev/serial/by-id/ --network=host catoptric_array