#include "DetectingGroupsNode.h"

#include <isaac_ros_nitros_image_type/nitros_image_view.hpp>
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <string>

DetectingGroupsNode::DetectingGroupsNode() : Node("detecting_groups_node") {
    auto shareDir = ament_index_cpp::get_package_share_directory("detecting_groups");

    this->declare_parameter<std::string>("gemma_model_location", shareDir + "/gemma-model/gemma-4-E4B-it-Q8_0.gguf");
    this->declare_parameter<std::string>("image_encoder_location", shareDir + "/gemma-model/mmproj-F16.gguf");
    this->declare_parameter<std::string>("prompt_location", shareDir + "/prompt.txt");

    rclcpp::QoS maskedImageQoS(1);
    maskedImageQoS.best_effort();
    maskedImageQoS.keep_last(1);
    maskedImageQoS.durability_volatile();
    
}

void DetectingGroupsNode::maskedImageCallback(const nitros::NitrosImageView& imageView) {
    
}
