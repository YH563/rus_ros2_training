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
                # TODO(task-03): 传入参数（sensor_id / publish_rate_hz / alarm_threshold）。
            ),
        ]
    )
