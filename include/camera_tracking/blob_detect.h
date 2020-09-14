#ifndef CAMERA_TRACKING_BLOB_DETECT_H
#define CAMERA_TRACKING_BLOB_DETECT_H

#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <dynamic_reconfigure/server.h>
#include <camera_tracking/BlobDetectConfig.h>

namespace camera_tracking {
class BlobDetect {
public:
  BlobDetect(ros::NodeHandle& nh);
  BlobDetect(BlobDetect&&) = default;
  BlobDetect(const BlobDetect&) = default;
  BlobDetect& operator=(BlobDetect&&) = default;
  BlobDetect& operator=(const BlobDetect&) = default;
  ~BlobDetect() = default;

private:
  void imageCallback(const sensor_msgs::ImageConstPtr& msg);
  void reconfigureCallback(BlobDetectConfig& config, uint32_t level);

  bool _publishOverlay;
  std::string _imageFormat;
  ros::Publisher _blobPub;
  ros::NodeHandle _nh;
  image_transport::ImageTransport _it;
  image_transport::Subscriber _imageSub;
  image_transport::Publisher _overlayPub;
  dynamic_reconfigure::Server<BlobDetectConfig> _detectorParams;
  cv::Ptr<cv::SimpleBlobDetector> _detector;
};

}  // namespace camera_tracking

#endif  // !CAMERA_TRACKING_BLOB_DETECT_H