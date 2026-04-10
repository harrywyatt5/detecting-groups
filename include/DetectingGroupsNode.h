#pragma once

#include <rclcpp/rclcpp.hpp>
#include <isaac_ros_managed_nitros/managed_nitros_publisher.hpp>
#include <isaac_ros_managed_nitros/managed_nitros_subscriber.hpp>
#include <isaac_ros_nitros_image_type/nitros_image_view.hpp>
#include <detecting_groups_custom_msg/msg/present_groups.hpp>
#include <memory>

namespace nitros = nvidia::isaac_ros::nitros;

class DetectingGroupsNode : public rclcpp::Node {
private:
    std::shared_ptr<nitros::ManagedNitrosSubscriber<nitros::NitrosImageView>> maskedImageSub;
    rclcpp::Publisher<detecting_groups_custom_msg::msg::PresentGroups>::SharedPtr groupsPub;
public:
    DetectingGroupsNode();
    void maskedImageCallback(const nitros::NitrosImageView& msg);
};
