#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/empty.hpp"
#include "training_interfaces/msg/sensor_reading.hpp"

using namespace std::chrono_literals;

class MonitorNode : public rclcpp::Node {
public:
    MonitorNode() : Node("monitor") {
        // TODO(task-04): 声明参数 topic（默认 /sensor_reading）与 report_period_s（默认 5.0）。

        // TODO(task-04): 订阅目标话题；回调里只更新统计，不打印、不做重活。

        // TODO(task-04): 提供 ~/reset_stats（std_srvs/srv/Empty），清空统计。

        // TODO(task-04): 按 report_period_s 创建报告定时器。
    }

private:
    void on_reading(const training_interfaces::msg::SensorReading& msg) {
        // TODO(task-04): 更新计数与 min/max/avg；message 里的字段按 Task 03 的定义使用。
        (void)msg;
    }

    void on_report() {
        // TODO(task-04): 输出本周期统计；没有数据时输出「等待数据」，不要刷屏。
    }

    void on_reset(const std::shared_ptr<std_srvs::srv::Empty::Request> request,
                  std::shared_ptr<std_srvs::srv::Empty::Response> response) {
        // TODO(task-04): 清空统计并输出一条日志。
        (void)request;
        (void)response;
    }

    // TODO(task-04): 成员：subscription_、service_、timer_、topic_、report_period_s_、
    // count_、min_、max_、sum_。
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MonitorNode>());
    rclcpp::shutdown();
    return 0;
}
