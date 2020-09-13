#include <camera_tracking/blob_detect.h>
#include <camera_tracking/BlobArray.h>
#include <camera_tracking/Blob.h>

using namespace camera_tracking;

BlobDetect::BlobDetect(ros::NodeHandle& nh) : _nh(nh), _it(nh) {
  _imageSub =
      _it.subscribe("/webcam/image_raw", 1, &BlobDetect::imageCallback, this);
  _blobPub = _nh.advertise<camera_tracking::BlobArray>("blobs", 16);
  _overlayPub = _it.advertise("overlay", 16);
  _detector = cv::SimpleBlobDetector::create();
}

void BlobDetect::imageCallback(const sensor_msgs::ImageConstPtr& msg) {
  cv::Mat mat = cv_bridge::toCvShare(msg, "bgr8")->image;
  std::vector<cv::KeyPoint> keyPoints;
  _detector->detect(mat, keyPoints);

  std_msgs::Header header = std_msgs::Header();

  BlobArray blobs;
  blobs.header = header;
  blobs.blobs = std::vector<Blob>(keyPoints.size());
  for (size_t i = 0; i < keyPoints.size(); i++) {
    blobs.blobs[i].angle = keyPoints[i].angle;
    blobs.blobs[i].class_id = keyPoints[i].class_id;
    blobs.blobs[i].octave = keyPoints[i].octave;
    blobs.blobs[i].xPos = keyPoints[i].pt.x;
    blobs.blobs[i].yPos = keyPoints[i].pt.y;
    blobs.blobs[i].response = keyPoints[i].response;
    blobs.blobs[i].size = keyPoints[i].size;
  }

  cv::Mat overlay;
  cv::drawKeypoints(mat, keyPoints, overlay, cv::Scalar(0, 0, 255),
                    cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  sensor_msgs::ImagePtr overlayMsg =
      cv_bridge::CvImage(header, "bgr8", overlay).toImageMsg();

  _blobPub.publish(blobs);
  _overlayPub.publish(overlayMsg);
}