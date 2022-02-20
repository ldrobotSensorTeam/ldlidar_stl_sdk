/**
 * @file main.cpp
 * @author LDRobot (marketing1@ldrobot.com)
 * @brief  main process App
 *         This code is only applicable to LDROBOT LiDAR LD06 products 
 * sold by Shenzhen LDROBOT Co., LTD    
 * @version 0.1
 * @date 2021-10-28
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

#include <stdio.h>

#include <iostream>
#include <string>

#include "cmd_interface_linux.h"
#include "lipkg.h"

int main(int argc, char **argv) {
  std::cout << "[ldrobot] SDK Pack Version is " << "v2.2.1" << std::endl;
  
  if (argc != 2) {
    std::cout << "[ldrobot] please input: ./ldlidar <serial_number>" << std::endl;
    std::cout << "[ldrobot] example:" << std::endl;
    std::cout << "./ldlidar /dev/ttyUSB*" << std::endl;
    std::cout << "or" << std::endl;
    std::cout << "./ldlidar /dev/ttyS*" << std::endl;
    exit(EXIT_FAILURE);
  }


  LiPkg *lidar = new LiPkg;
  CmdInterfaceLinux cmd_port;
  std::string port_name(argv[1]);

  if (port_name.empty()) {
    std::cout << "[ldrobot] Can't find LDLiDAR device" << std::endl;
    exit(EXIT_FAILURE);
  }else {
    std::cout << "[ldrobot] FOUND LDLiDAR" << std::endl;
  }

  cmd_port.SetReadCallback([&lidar](const char *byte, size_t len) {
    if (lidar->Parse((uint8_t *)byte, len)) {
      lidar->AssemblePacket();
    }
  });

  if (cmd_port.Open(port_name)) {
    std::cout << "[ldrobot] open LDLiDAR device  " << port_name  << " success!"<< std::endl;
  }else {
    std::cout << "[ldrobot] open LDLiDAR device  " << port_name << " fail!"<< std::endl;
    exit(EXIT_FAILURE);
  }

  Points2D laser_scan;
    
  while (1) {
    if (lidar->IsFrameReady()) {
      std::cout << "[ldrobot] speed(Hz)： " << lidar->GetSpeed() << std::endl;
      std::cout << "[ldrobot] pack errcount: " << lidar->GetErrorTimes() << std::endl;
      laser_scan = lidar->GetData();
      std::cout << "[ldrobot] laser_scan.size() " << laser_scan.size() << std::endl;
      for (auto ele : laser_scan) {
        std::cout << "[ldrobot] angle: " << ele.angle << " "
                  << "distance(mm): " << ele.distance << " "
                  << "confidence: " << (int)ele.confidence << " "
                  << std::endl;
      }
      lidar->ResetFrameReady();
    }
    // usleep(1000);  // sleep 1ms
  }
  return 0;
}

/********************* (C) COPYRIGHT SHENZHEN LDROBOT CO., LTD *******END OF
 * FILE ********/