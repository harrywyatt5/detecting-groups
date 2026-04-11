#pragma once

#include <isaac_ros_nitros_image_type/nitros_image_view.hpp>
#include <rclcpp/rclcpp.hpp>
#include <opencv2/opencv.hpp>
#include <llama.h>
#include <mtmd.h>
#include <cstdint>

namespace nitros = nvidia::isaac_ros::nitros;

class InputImageHandler {
private:
    cv::Mat inputImage;
    bool populatedWithNewImage;
public:
    InputImageHandler() : populatedWithNewImage(false) {};
    void uploadMessage(const nitros::NitrosImageView& imageMsg);
    bool hasNextImage() const;
    mtmd::bitmap createMtmdBitmap();
};
