from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    namespace = LaunchConfiguration("namespace")

    return LaunchDescription(
        [
            DeclareLaunchArgument("namespace", default_value="sensor"),
            Node(
                package="training_sensor",
                executable="sensor_node",
                name="sensor",
                namespace=namespace,
                output="screen",
                # TODO(task-04): 传入与 monitor 匹配的参数。
            ),
            Node(
                package="training_capstone",
                executable="monitor_node",
                name="monitor",
                namespace="monitor",
                output="screen",
                # TODO(task-04): 传入 topic / report_period_s。
            ),
        ]
    )
