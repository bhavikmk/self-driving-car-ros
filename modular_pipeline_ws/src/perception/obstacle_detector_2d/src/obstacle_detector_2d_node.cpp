#include "object_detector.h"

class ObstacleDetector2D
{
    ros::NodeHandle nh;
    car_msgs::Detections detections;
    image_transport::ImageTransport it;
    image_transport::Subscriber image_sub_;

public:
    ObstacleDetector2D() : it(nh)
    {
        image_sub_ = it.subscribe("/front_cam/image_raw", 1, &ObstacleDetector2D::imageCb, this);
        ros::Publisher pub = nh.advertise<car_msgs::Detections>("/2d_objects_detected", 1);
    }

    ~ObstacleDetector2D(){}

    void imageCb(const sensor_msgs::ImageConstPtr &msg)
    {
        cv_bridge::CvImagePtr cv_ptr;
        namespace enc = sensor_msgs::image_encodings;

        try
        {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception &e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }
        detectClassifySign(cv_ptr->image);
        
    }
    void detectClassifySign(cv::Mat img)
    {
        cv::Mat src, src_gray;
        cv::Mat dst, detected_edges;

        int edgeThresh = 1;
        int lowThreshold = 200;
        int highThreshold = 300;
        int kernel_size = 5;

        img.copyTo(src);

        cv::cvtColor(img, src_gray, CV_BGR2GRAY);
        cv::blur(src_gray, detected_edges, cv::Size(5, 5));
        cv::Canny(detected_edges, detected_edges, lowThreshold, highThreshold, kernel_size);

        dst = cv::Scalar::all(0);
        img.copyTo(dst, detected_edges);
        dst.copyTo(img);

        // Find 

        // pub.publish(Detections);
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "obstacle_detector_2d");
    ObstacleDetector2D ic;
    ros::spin();
    return 0;
}