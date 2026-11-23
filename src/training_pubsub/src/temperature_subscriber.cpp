#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

class TemperatureSubscriber : public rclcpp::Node {
public:
    TemperatureSubscriber() : Node("temperature_subscriber") {
        // TODO(task-01): 创建 subscription，订阅 /temperature。
        // 提示：回调参数用 const std_msgs::msg::Float64&。
    }

private:
    void on_temperature(const std_msgs::msg::Float64& msg) {
        // TODO(task-01): 打印收到的数值；大于 30.0 时用 RCLCPP_WARN 告警。
        (void)msg;
    }

    // TODO(task-01): 声明 subscription_ 成员。
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TemperatureSubscriber>());
    rclcpp::shutdown();
    return 0;
}
