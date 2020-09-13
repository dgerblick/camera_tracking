#ifndef CAMERA_TRACKING_FILTER_H
#define CAMERA_TRACKING_FILTER_H

#include <ros/ros.h>
#include <image_transport/image_transport.h>

namespace arc {
class Filter {
public:
  Filter(ros::NodeHandle& nh);
  ~Filter();

private:
  ros::NodeHandle _nh;
  image_transport::ImageTransport _it;
  ros::Subscriber _imageSub;
  ros::Subscriber _settingsSub;
  ros::Publisher _imagePub;
};

}  // namespace arc

#endif  // !CAMERA_TRACKING_FILTER_H