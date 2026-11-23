#include <chrono>
#include <cmath>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

using namespace std::chrono_literals;

class TemperaturePublisher : public rclcpp::Node {
public:
    TemperaturePublisher() : Node("temperature_publisher") {
        // TODO(task-01): 创建 publisher：话题 /temperature，类型 std_msgs::msg::Float64。
        // 提示：成员变量要保存下来，否则构造结束后就被销毁。

        // TODO(task-01): 创建 1 Hz 定时器，回调绑定到 on_timer。
    }

private:
    void on_timer() {
        // TODO(task-01): 计算 value = 20.0 + 10.0 * sin(运行秒数)。
        // 提示：用 rclcpp::Clock 或 std::chrono::steady_clock 记录起点。

        // TODO(task-01): 填入 Float64 消息并发布，再用 RCLCPP_INFO 打印本次的值。
    }

    // TODO(task-01): 声明 publisher_（std_msgs::msg::Float64 的 publisher）。
    // TODO(task-01): 声明 timer_（rclcpp::TimerBase::SharedPtr）。
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TemperaturePublisher>());
    rclcpp::shutdown();
    return 0;
}
