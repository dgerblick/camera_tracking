#include <ros/ros.h>

#include <camera_tracking/blob_detect.h>

int main(int argc, char** argv) {
  ros::init(argc, argv, "blob_detect");
  ros::NodeHandle nh;

  camera_tracking::BlobDetect blobDetect(nh);

  ros::spin();
}
