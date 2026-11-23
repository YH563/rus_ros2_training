# Linux 基础：非常粗略版

> 只求「看得懂、不害怕」。命令不用背，用到再查。真正要记住的只有两件事：
> **每个终端都要 `source`**，以及**节点就是进程**。

## 1. 终端与当前目录

打开终端就是进入一个 shell。它执行你输入的命令，并显示输出。

```bash
pwd            # 我在哪
ls             # 这里有什么
cd ~/RUS_Sim   # 去某个目录（~ 是家目录）
cd ..          # 上一级
```

- `Tab` 补全，`↑` 翻历史，`Ctrl+C` 停止当前程序
- 路径分绝对（`/opt/ros/humble`）和相对（`../src`）

## 2. 记住四个目录

| 目录 | 是什么 |
|---|---|
| `~` | 你的家目录，代码一般放这里 |
| `/opt/ros/humble` | ROS 2 安装位置 |
| `src/` | 你的源码 |
| `install/` | 编译后的东西，`ros2 run` 找的是它 |

工作区还有 `build/`（中间产物）和 `log/`（构建日志），都可以删了重建。

## 3. 够用的常用命令

```bash
mkdir -p a/b      # 建目录
cp -r src dst     # 复制
mv old new        # 移动/改名
rm file           # 删除（没有回收站）
find . -name '*.cpp'
grep -rn "TODO" src
cat file          # 看文件
less file         # 分页看，q 退出
tail -f log.txt   # 持续看日志，Ctrl+C 退出
du -sh build      # 看目录大小
df -h             # 看磁盘
```

`rm -rf` 很危险，执行前先 `ls` 确认路径。

## 4. 权限与 sudo

```bash
ls -l          # 看谁拥有、谁能读写执行
chmod +x x.sh  # 加可执行权限
```

- `Permission denied`：先看文件属于谁、有没有权限
- **不要用 `sudo colcon build`**，否则 `build/install` 变成 root 所有，后面很难处理
- `sudo` 只在装系统软件时用

## 5. 环境变量：最常见的坑

```bash
echo $PATH              # 命令从哪些目录找
printenv ROS_DISTRO     # 当前 ROS 版本
env | grep ROS          # 所有 ROS 变量
```

每个新终端都要：

```bash
source /opt/ros/humble/setup.bash
source install/setup.bash
```

`source` 只影响当前终端。另一个终端「找不到包」，通常就是没 source。
`install/setup.bash` 必须在 `colcon build` 之后 source。

## 6. 进程：节点就是进程

```bash
ros2 run pkg node    # 前台运行，Ctrl+C 结束
ps aux | grep ros2   # 找进程
kill <PID>           # 结束进程
```

- `Ctrl+C` 结束；`Ctrl+Z` 只是暂停并放到后台
- 一个 ROS 节点通常是一个进程；话题没数据时，先确认发布者进程还在

```bash
ros2 node list
ros2 topic info /temperature --verbose
```

## 7. 管道、重定向、退出码

```bash
命令 | 命令2            # 把输出交给下一个命令
命令 > out.log          # 覆盖写文件
命令 >> out.log         # 追加
命令 2>&1 | tee all.log # 输出和错误都保存并显示
echo $?                 # 上一条命令的退出码，0 表示成功
```

脚本和命令链靠退出码判断成败。

## 8. 网络与装软件（了解）

```bash
ss -ltnp              # 本机监听端口
ssh user@host         # 远程登录
scp file user@host:path
sudo apt update && sudo apt install <包名>
```

`127.0.0.1` 就是本机；前端连的 `ws://127.0.0.1:8765` 在本机。

## 9. 和本仓库的对应

| 你做的事 | 对应概念 |
|---|---|
| 每个终端先 source | 环境变量按终端隔离 |
| `colcon build` | 启动编译进程，产物进 `build/install` |
| 两个终端一发布一订阅 | 多进程 |
| `Ctrl+C` 停节点 | 前台进程与信号 |
| `Permission denied` | 文件权限/所有权 |

## 10. 自检

1. 为什么另一个终端 `ros2 run` 找不到包？
2. `Ctrl+C` 和 `Ctrl+Z` 有什么区别？
3. 为什么不能用 `sudo colcon build`？

答不上来就回看对应小节；其余命令用到再查。
