#!/bin/bash

docker run --rm -it \
    -e DISPLAY \
    -e LIBGL_ALWAYS_INDIRECT \
    -v $XAUTH:/home/$USER/.Xauthority \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    --device=/dev/video0 \
    --hostname camera_tracking_test \
    --name camera-tracking_test \
    dgerblick/camera_tracking