FROM dgerblick/arc-dev:latest

ARG USER=arc
ARG CATKIN_WS=/home/$USER/catkin_ws
ARG CATKIN_SRC=$CATKIN_WS/src/

USER root

# Add packages
COPY . $CATKIN_SRC/camera_tracking/
# RUN git clone https://github.com/foo/bar.git $CATKIN_SRC/bar/

# Install dependencies and Build workspace
WORKDIR $CATKIN_WS
RUN rosdep install --as-root apt:no --from-paths src --ignore-src -r -y &&  \
    zsh -c "source ~/.zshrc && \
    catkin config -i /opt/ros/$ROS_DISTRO --install --cmake-args -DCMAKE_BUILD_TYPE=Release && \
    catkin clean -b -y && \
    catkin build"

# Start
CMD ["roslaunch", "camera_tracking", "blob_detect.launch", "blob_overlay:=overlay"]