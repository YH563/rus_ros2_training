#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "training_interfaces/msg/sensor_reading.hpp"
#include "training_interfaces/srv/reset_counter.hpp"

using namespace std::chrono_literals;

class SensorNode : public rclcpp::Node {
public:
    SensorNode() : Node("sensor_node") {
        // TODO(task-03): 声明并读取参数 sensor_id / publish_rate_hz / alarm_threshold。

        // TODO(task-03): 创建 /sensor_reading 的 publisher。
        // 消息类型：training_interfaces::msg::SensorReading。

        // TODO(task-03): 创建服务 /reset_counter（training_interfaces::srv::ResetCounter）。
        // 回调里比较请求的 sensor_id，返回 success 和重置前的计数。

        // TODO(task-03): 创建定时器，周期由 publish_rate_hz 决定。
    }

private:
    void on_timer() {
        // TODO(task-03): 填 stamp / sensor_id / value / status 并发布。
        // value 超过阈值时 status=STATUS_WARN，否则 STATUS_OK；计数加一。
    }

    void on_reset(const std::shared_ptr<training_interfaces::srv::ResetCounter::Request> request,
                  std::shared_ptr<training_interfaces::srv::ResetCounter::Response> response) {
        // TODO(task-03): 校验 request->sensor_id；成功时把计数清零并写入 response->reset_count。
        (void)request;
        (void)response;
    }

    // TODO(task-03): 成员：publisher_、service_、timer_、sensor_id_、
    // publish_rate_hz_、alarm_threshold_、counter_。
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SensorNode>());
    rclcpp::shutdown();
    return 0;
}
