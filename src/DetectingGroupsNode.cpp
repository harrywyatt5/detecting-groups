#include "DetectingGroupsNode.h"

#include "SystemPrompt.h"
#include "MixedUserPrompt.h"
#include "PromptFromFileFactory.h"
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/header.hpp>
#include <isaac_ros_nitros_image_type/nitros_image_view.hpp>
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <detecting_groups_custom_msg/msg/present_groups.hpp>
#include <string>
#include <chrono>

DetectingGroupsNode::DetectingGroupsNode() : lastProcessedFrame(rclcpp::Time(0, 0)), Node("detecting_groups_node") {
    auto shareDir = ament_index_cpp::get_package_share_directory("detecting_groups");

    this->declare_parameter<std::string>("masked_image_topic", "/masked_image");
    this->declare_parameter<std::string>("group_info_topic", "group_info");
    this->declare_parameter<std::string>("gemma_model_location", shareDir + "/gemma-model/gemma-4-E4B-it-Q8_0.gguf");
    this->declare_parameter<std::string>("image_encoder_location", shareDir + "/gemma-model/mmproj-F16.gguf");
    this->declare_parameter<std::string>("prompt_location", shareDir + "/gemma-model/prompt.txt");
    this->declare_parameter<double>("processing_period", 5.0f);

    processingPeriod = this->get_parameter("processing_period").as_double();

    rclcpp::QoS maskedImageQoS(1);
    maskedImageQoS.best_effort();
    maskedImageQoS.keep_last(1);
    maskedImageQoS.durability_volatile();
    maskedImageSub = std::make_shared<nitros::ManagedNitrosSubscriber<nitros::NitrosImageView>>(
        this,
        this->get_parameter("masked_image_topic").as_string(),
        nitros::nitros_image_rgb8_t::supported_type_name,
        std::bind(&DetectingGroupsNode::maskedImageCallback, this, std::placeholders::_1),
        nitros::NitrosDiagnosticsConfig{},
        maskedImageQoS
    );

    groupsPub = this->create_publisher<detecting_groups_custom_msg::msg::PresentGroups>(
        this->get_parameter("group_info_topic").as_string(),
        10
    );

    // Get system prompt and set up model
    auto systemPrompt = PromptFromFileFactory().createSystemPromptFromFile(this->get_parameter("prompt_location").as_string());
    modelProvider = std::make_unique<GemmaModelProvider>(
        this->get_parameter("gemma_model_location").as_string(),
        this->get_parameter("image_encoder_location").as_string(),
        systemPrompt.get()
    );
}

void DetectingGroupsNode::maskedImageCallback(const nitros::NitrosImageView& imageView) {
    auto imageTime = rclcpp::Time(imageView.GetTimestampSeconds(), imageView.GetTimestampNanoseconds());
    double delta = (imageTime - lastProcessedFrame).seconds();

    if (delta < processingPeriod) {
        // We haven't waited long enough, just return now
        return;
    }

    auto start_time = std::chrono::steady_clock::now();

    lastProcessedFrame = imageTime;

    // Copy across image to input and then get a bitmap of it which we can use for the next input
    // Unfortunately, we have to create MixedUserPrompt on the heap due to polymorphism in C++
    inputHandler.uploadMessage(imageView);
    auto nextPrompt = std::make_unique<MixedUserPrompt>(std::move(inputHandler.createMtmdBitmap()));
    auto response = modelProvider->processPrompt(nextPrompt.get());

    auto responseMsg = response.generateStructuredResponse();
    std_msgs::msg::Header header;
    header.stamp = this->get_clock()->now();
    responseMsg.header = header;
    responseMsg.original_frame_time = lastProcessedFrame;
    groupsPub->publish(responseMsg);

    auto end_time = std::chrono::steady_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    RCLCPP_INFO(this->get_logger(), "Frame time: %ldms", static_cast<long>(duration_ms));
}
