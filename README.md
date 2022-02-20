- [cn](#操作指南)
- [en](#Instructions)
# 操作指南

>此SDK仅适用于深圳乐动机器人有限公司销售的激光雷达产品，产品型号为:
> - LDROBOT LiDAR LD06
> - LDROBOT LiDAR LD19
## 0. 获取雷达的ROS功能包
```bash
$ cd ~

$ mkdir  ldlidar_ws

$ cd ldlidar_ws

$ git clone  https://github.com/ldrobotSensorTeam/ldlidar_stl_sdk.git
# 或者
$ git clone  https://gitee.com/ldrobotSensorTeam/ldlidar_stl_sdk.git
```

## 1. 系统设置
- 第一步，通过板载串口或者USB转串口模块(例如,cp2102模块)的方式使雷达连接到你的系统主板.
- 第二步，设置雷达在系统中挂载的串口设备-x权限(以/dev/ttyUSB0为例)
	- 实际使用时，根据雷达在你的系统中的实际挂载情况来设置，可以使用`ls -l /dev`命令查看.

``` bash
$ cd ~/ldlidar_ws/ldlidar_stl_sdk

$ sudo chmod 777 /dev/ttyUSB0
```

## 2. 编译

```bash
$ mkdir build # 若工程目录下不存在build文件夹则需创建
$ cd build
$ cmake ../
$ make
```

## 3. 运行
  ``` bash
  $ ./ldlidar_stl <serial_number>
  # 例如 ./ldlidar_stl /dev/ttyS0
  ```

# Instructions
> This SDK is only applicable to the LiDAR products sold by Shenzhen LDROBOT Co., LTD. The product models are :
> - LDROBOT LiDAR LD06
> - LDROBOT LiDAR LD19
## 0. get LiDAR ROS Package
```bash
$ cd ~

$ mkdir  ldlidar_ws

$ cd ldlidar_ws

$ git clone  https://github.com/ldrobotSensorTeam/ldlidar_stl_sdk.git
# or
$ git clone  https://gitee.com/ldrobotSensorTeam/ldlidar_stl_sdk.git
```
## step 1: system setup
- Connect the LiDAR to your system motherboard via an onboard serial port or usB-to-serial module (for example, CP2102 module).

- Set the -x permission for the serial port device mounted by the radar in the system (for example, /dev/ttyUSB0)

  - In actual use, the LiDAR can be set according to the actual mounted status of your system, you can use 'ls -l /dev' command to view.

``` bash
$ cd ~/ldlidar_ws/ldlidar_stl_sdk

$ sudo chmod 777 /dev/ttyUSB0
```

## step 2: build

``` bash
$ mkdir build # If the build folder does not exist in the project directory, create one
$ cd build
$ cmake ../
$ make
```

## step 3: run

  ``` bash
  $ ./ldlidar_stl <serial_number>
  # eg ./ldlidar_stl /dev/ttyS0
  ```