#include <camera_tracking/blob_detect.h>
#include <camera_tracking/BlobArray.h>
#include <camera_tracking/Blob.h>

using namespace camera_tracking;

BlobDetect::BlobDetect(ros::NodeHandle& nh) : _nh(nh), _it(nh) {
  std::string imageIn;
  std::string blobOut;
  std::string overlayOut;

  if (!_nh.param<std::string>("image_in", imageIn, "/camera/image_raw"))
    _nh.setParam("image_in", "HERE");
  _nh.param<std::string>("blob_out", blobOut, "blobs");
  _nh.param<std::string>("overlay_out", overlayOut, "");

  _imageSub = _it.subscribe(imageIn, 1, &BlobDetect::imageCallback, this);
  _blobPub = _nh.advertise<camera_tracking::BlobArray>(blobOut, 16);

  _publishOverlay = !overlayOut.empty();
  if (_publishOverlay)
    _overlayPub = _it.advertise(overlayOut, 16);

  _nh.param<std::string>("image_format", _imageFormat, "bgr8");

  dynamic_reconfigure::Server<BlobDetectConfig>::CallbackType callback =
      boost::bind(&BlobDetect::reconfigureCallback, this, _1, _2);
  _detectorParams.setCallback(callback);
  _detector = cv::SimpleBlobDetector::create();
}

void BlobDetect::imageCallback(const sensor_msgs::ImageConstPtr& msg) {
  cv::Mat mat = cv_bridge::toCvShare(msg, _imageFormat)->image;
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
      cv_bridge::CvImage(header, _imageFormat, overlay).toImageMsg();

  _blobPub.publish(blobs);
  if (_publishOverlay)
    _overlayPub.publish(overlayMsg);
}

void BlobDetect::reconfigureCallback(BlobDetectConfig& config, uint32_t level) {
  cv::SimpleBlobDetector::Params params;
  params.thresholdStep = config.thresholdStep;
  params.minThreshold = config.minThreshold;
  params.maxThreshold = config.maxThreshold;
  params.minDistBetweenBlobs = config.minDistBetweenBlobs;
  params.minRepeatability = config.minRepeatability;
  params.filterByColor = config.filterByColor;
  params.blobColor = config.blobColor;
  params.filterByArea = config.filterByArea;
  params.minArea = config.minArea;
  params.maxArea = config.maxArea;
  params.filterByCircularity = config.filterByCircularity;
  params.minCircularity = config.minCircularity;
  params.maxCircularity = config.maxCircularity;
  params.filterByInertia = config.filterByInertia;
  params.minInertiaRatio = config.minInertiaRatio;
  params.maxInertiaRatio = config.maxInertiaRatio;
  params.filterByConvexity = config.filterByConvexity;
  params.minConvexity = config.minConvexity;
  params.maxConvexity = config.maxConvexity;
  _detector = cv::SimpleBlobDetector::create(params);
}