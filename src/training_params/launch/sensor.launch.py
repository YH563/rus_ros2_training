from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    namespace = LaunchConfiguration("namespace")

    return LaunchDescription(
        [
            DeclareLaunchArgument("namespace", default_value="sensor_a"),
            Node(
                package="training_params",
                executable="sensor_node",
                name="sensor",
                namespace=namespace,
                output="screen",
                # TODO(task-02): 从这里传入参数（sensor_id / publish_rate_hz / alarm_threshold）。
            ),
            # TODO(task-02): 再加一份实例，namespace=sensor_b，参数与 sensor_a 不同。
        ]
    )
