#include <chrono>
#include <cmath>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

using namespace std::chrono_literals;

class SensorNode : public rclcpp::Node {
public:
    SensorNode() : Node("sensor") {
        // TODO(task-02): 声明并读取参数 sensor_id / publish_rate_hz / alarm_threshold。
        // 提示：declare_parameter<T>("名字", 默认值)。

        // TODO(task-02): 校验 publish_rate_hz（> 0 且 <= 100）和 alarm_threshold。
        // 非法值要记录日志并回退到安全默认值，不能让节点崩溃。

        // TODO(task-02): 创建 publisher（/temperature，std_msgs::msg::Float64）和定时器。

        // TODO(task-02): 注册参数回调（add_on_set_parameters_callback），
        // 频率变化时先 cancel 旧定时器，再按新周期创建。
    }

private:
    void on_timer() {
        // TODO(task-02): 发布数值；超过 alarm_threshold_ 时用 RCLCPP_WARN，并带上 sensor_id_。
    }

    // TODO(task-02): 成员：publisher_、timer_、sensor_id_、alarm_threshold_、publish_rate_hz_，
    // 以及参数回调句柄。
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SensorNode>());
    rclcpp::shutdown();
    return 0;
}
