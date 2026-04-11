#include "InputImageHandler.h"

#include <isaac_ros_nitros_image_type/nitros_image_view.hpp>
#include <rclcpp/rclcpp.hpp>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/image_encodings.hpp>
#include <llama.h>
#include <mtmd.h>
#include <cstdint>
#include <stdexcept>

void InputImageHandler::uploadMessage(const nitros::NitrosImageView& imageMsg) {
    if (imageMsg.GetEncoding() != sensor_msgs::image_encodings::RGB8) {
        throw std::runtime_error("DetectingGroupsNode only supports RGB8 image inputs");
    }

    // Wrap incoming message in a GpuMat and use this to download it into the cv::Mat
    // The first time we do this it'll probably be expensive, as we will have to allocate bytes to
    // cv::Mat!
    const cv::cuda::GpuMat gpuTemp(
        imageMsg.GetHeight(),
        imageMsg.GetWidth(),
        CV_8UC3,
        const_cast<uint8_t*>(imageMsg.GetGpuData()),
        imageMsg.GetStride()
    );

    gpuTemp.download(inputImage);
    // if somehow the buffer is not continous, then make sure it is
    if (!inputImage.isContinuous()) {
        inputImage = inputImage.clone();
    }
    populatedWithNewImage = true;
}

mtmd::bitmap InputImageHandler::createMtmdBitmap() {
    if (!populatedWithNewImage) {
        throw std::runtime_error("No image has been uploaded yet");
    }

    populatedWithNewImage = false;
    return mtmd::bitmap(inputImage.cols, inputImage.rows, inputImage.ptr<uint8_t>());
}

bool InputImageHandler::hasNextImage() const {
    return populatedWithNewImage;
}
