/**
 * @file test.cpp
 * @author LDRobot (support@ldrobot.com)
 * @brief  sdk function test
 *         This code is only applicable to LDROBOT LiDAR LD00 LD03 LD08 LD14
 * products sold by Shenzhen LDROBOT Co., LTD
 * @version 0.1
 * @date 2021-11-08
 *
 * @copyright Copyright (c) 2021  SHENZHEN LDROBOT CO., LTD. All rights
 * reserved.
 * Licensed under the MIT License (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License in the file LICENSE
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ldlidar_driver.h"
#include <wiringPi.h>

uint64_t GetSystemTimeStamp(void) {
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp = 
    std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
  auto tmp = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch());
  return ((uint64_t)tmp.count());
}

bool  ControlPinInit(void) {
  // raspberrypi wiringPi GPIO Init
  if(wiringPiSetup() == -1) {
    return false;
  }
  pinMode(25,OUTPUT); // set wiringPi 25 Pin number is outuput Mode.
  return true;
}

void LidarPowerOn(void) {
  LDS_LOG_DEBUG("Lidar Power On","");
  digitalWrite(25,HIGH);
}

void LidarPowerOff(void) {
  LDS_LOG_DEBUG("Lidar Power Off","");
  digitalWrite(25,LOW);
}

void AbortTesting(void) {
  LDS_LOG_WARN("Testing abort","");
  LidarPowerOff();
  exit(EXIT_FAILURE);
}

int function_main(int argc, char **argv) {

  if (argc < 4) {
    LDS_LOG_WARN("Terminal >>: ./ldlidar_stl_node <product_name> serialcom <serial_number>","");
    LDS_LOG_WARN("or","");
    LDS_LOG_WARN("Terminal >>: ./ldlidar_stl_node <product_name> networkcom_tcpclient <server_ip> <server_port>","");
    LDS_LOG_WARN("For example:","");
    LDS_LOG_WARN("./ldlidar_stl_node LD06 serialcom /dev/ttyUSB0","");
    LDS_LOG_WARN("./ldlidar_stl_node LD19 serialcom /dev/ttyUSB0","");
    LDS_LOG_WARN("-----------------","");
    LDS_LOG_WARN("./ldlidar_stl_node LD06 networkcom_tcpclient 192.168.1.200 2000","");
    LDS_LOG_WARN("./ldlidar_stl_node LD19 networkcom_tcpclient 192.168.1.200 2000","");
    exit(EXIT_FAILURE);
  }

  std::string product_name(argv[1]);
  std::string communication_mode(argv[2]);
  std::string port_name;
  std::string server_ip;
  std::string server_port;
  uint32_t serial_baudrate = 0;
  ldlidar::LDType type_name;

  if ((communication_mode != "serialcom") && 
      (communication_mode != "networkcom_tcpclient")) {
    LD_LOG_ERROR("Terminal:input argc(3) value is error","");
    LDS_LOG_WARN("Terminal >>: ./ldlidar_stl_node <product_name> serialcom <serial_number>","");
    LDS_LOG_WARN("or","");
    LDS_LOG_WARN("Terminal >>: ./ldlidar_stl_node <product_name> networkcom_tcpclient <server_ip> <server_port>","");
    LDS_LOG_WARN("For example:","");
    LDS_LOG_WARN("./ldlidar_stl_node LD06 serialcom /dev/ttyUSB0","");
    LDS_LOG_WARN("./ldlidar_stl_node LD19 serialcom /dev/ttyUSB0","");
    LDS_LOG_WARN("-----------------","");
    LDS_LOG_WARN("./ldlidar_stl_node LD06 networkcom_tcpclient 192.168.1.200 2000","");
    LDS_LOG_WARN("./ldlidar_stl_node LD19 networkcom_tcpclient 192.168.1.200 2000","");
    exit(EXIT_FAILURE);
  } else {
    if (communication_mode == "serialcom") {
      if (argc != 4) {
        LD_LOG_ERROR("Terminal:input argc != 4","");
        exit(EXIT_FAILURE);
      }
      port_name = argv[3];
    } else if (communication_mode == "networkcom_tcpclient") {
      if (argc != 5) {
        LD_LOG_ERROR("Terminal:input argc != 5","");
        exit(EXIT_FAILURE);
      }
      server_ip = argv[3];
      server_port = argv[4];
    }
  }

  ldlidar::LDLidarDriver* node = new ldlidar::LDLidarDriver();

  LDS_LOG_INFO("LDLiDAR SDK Pack Version is %s", node->GetLidarSdkVersionNumber().c_str());

  node->RegisterGetTimestampFunctional(std::bind(&GetSystemTimeStamp)); 

  node->EnableFilterAlgorithnmProcess(true);

  if (product_name == "LD06") {
    serial_baudrate = 230400;
    type_name = ldlidar::LDType::LD_06; 
  } else if (product_name == "LD19") {
    serial_baudrate = 230400;
    type_name = ldlidar::LDType::LD_19;
  } else {
    LDS_LOG_ERROR("input <product_name> is error!","");
    exit(EXIT_FAILURE);
  }
  
  if (communication_mode == "serialcom") {
    if (node->Start(type_name, port_name, serial_baudrate, ldlidar::COMM_SERIAL_MODE)) {
      LDS_LOG_INFO("ldldiar node start is success","");
      LidarPowerOn();
    } else {
      LD_LOG_ERROR("ldlidar node start is fail","");
      exit(EXIT_FAILURE);
    }
  } else if (communication_mode == "networkcom_tcpclient") {
    if (node->Start(type_name, server_ip.c_str(), server_port.c_str(), ldlidar::COMM_TCP_CLIENT_MODE)) {
      LDS_LOG_INFO("ldlidar node start is success","");
      LidarPowerOn();
    } else {
      LD_LOG_ERROR("ldlidar node start is fail","");
      exit(EXIT_FAILURE);
    }
  }

  if (node->WaitLidarCommConnect(3500)) {
    LDS_LOG_INFO("ldlidar communication is normal.","");
  } else {
    LDS_LOG_ERROR("ldlidar communication is abnormal.","");
    AbortTesting();
  }

  
  ldlidar::Points2D laser_scan_points;
  int cnt = 100;
  while (ldlidar::LDLidarDriver::IsOk()) {
    if ((cnt--) <= 0) {
      node->Stop();
    }

    switch (node->GetLaserScanData(laser_scan_points, 1500)){
      case ldlidar::LidarStatus::NORMAL: {
        double lidar_scan_freq = 0;
        node->GetLidarScanFreq(lidar_scan_freq);

#ifdef __LP64__
        LDS_LOG_INFO("speed(Hz):%f,size:%d,stamp_front:%lu, stamp_back:%lu",
            lidar_scan_freq, laser_scan_points.size(), laser_scan_points.front().stamp, laser_scan_points.back().stamp);
#else
        LDS_LOG_INFO("speed(Hz):%f,size:%d,stamp_front:%llu, stamp_back:%llu",
            lidar_scan_freq, laser_scan_points.size(), laser_scan_points.front().stamp, laser_scan_points.back().stamp);
#endif

        if (laser_scan_points.front().stamp >= laser_scan_points.back().stamp) {
          LDS_LOG_ERROR("timestamp error!","");
          node->Stop();
          AbortTesting();
        }
        
        int distance_zero_point_cnt = 0;
        for (auto point : laser_scan_points) {
          if (0 == point.distance) {
            distance_zero_point_cnt++;
          }
        }
        
        if (distance_zero_point_cnt >= (int)laser_scan_points.size()) {
          LDS_LOG_ERROR("a frame distance is zero value","");
          node->Stop();
          AbortTesting();
        }

        break;
      }
      case ldlidar::LidarStatus::DATA_TIME_OUT: {
        LDS_LOG_ERROR("ldlidar point cloud data publish time out, please check your lidar device.","");
        node->Stop();
        AbortTesting();
        break;
      }
      case ldlidar::LidarStatus::DATA_WAIT: {
        break;
      }
      default:
        break;
    }

    usleep(1000*100);  // sleep 100ms , 10hz
  }

  node->Stop();
  LidarPowerOff();
  sleep(3);

  delete node;
  node = nullptr;

  return 0;
}

int main(int argc, char** argv) {

  if (!ControlPinInit()) {
    LDS_LOG_ERROR("Control pin Setup failed.","");
    exit(EXIT_FAILURE);
  }
  
  for (int i = 0; i < 10000; i++) {
    function_main(argc, argv);
  }
  
  LDS_LOG_INFO("test is end.","");
  while (1) {
    sleep(1);
  }

  return 0;
}


/********************* (C) COPYRIGHT SHENZHEN LDROBOT CO., LTD *******END OF
 * FILE ********/
