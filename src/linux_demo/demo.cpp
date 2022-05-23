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

#include "lipkg.h"

int main(int argc, char **argv) {
 
  if (argc != 2) {
    std::cerr << "[ldrobot] please input: ./ldlidar_stl <serial_number>" << std::endl;
    std::cerr << "[ldrobot] example:" << std::endl;
    std::cerr << "./ldlidar_stl /dev/ttyUSB*" << std::endl;
    std::cerr << "or" << std::endl;
    std::cerr << "./ldlidar_stl /dev/ttyS*" << std::endl;
    exit(EXIT_FAILURE);
  }

  ldlidar::LiPkg *lidar = new ldlidar::LiPkg();
  std::cout << "[ldrobot] SDK Pack Version is " << lidar->GetSdkVersionNumber() << std::endl;

  ldlidar::CmdInterfaceLinux *cmd_port = new ldlidar::CmdInterfaceLinux();
  std::string port_name(argv[1]);
  cmd_port->SetReadCallback(std::bind(&ldlidar::LiPkg::CommReadCallback, lidar, std::placeholders::_1, std::placeholders::_2));

  if (cmd_port->Open(port_name)) {
    std::cout << "[ldrobot] open LDLiDAR device  " << port_name  << " success!"<< std::endl;
  }else {
    std::cerr << "[ldrobot] open LDLiDAR device  " << port_name << " fail!"<< std::endl;
    exit(EXIT_FAILURE);
  }

  auto last_time = std::chrono::steady_clock::now();

  while (1) {
    if (lidar->IsFrameReady()) {
      lidar->ResetFrameReady();
      last_time = std::chrono::steady_clock::now();
      std::cout << "[ldrobot] speed(Hz): " << lidar->GetSpeed() << std::endl;
      ldlidar::Points2D laser_scan = lidar->GetLaserScanData();
      std::cout << "[ldrobot] laser_scan.size() " << laser_scan.size() << std::endl;
      for (auto point : laser_scan) {
        std::cout << "[ldrobot] angle: " << point.angle << " "
                  << "distance(mm): " << point.distance << " "
                  << "intensity: " << (int)point.intensity << " "
                  << std::endl;
      }
    }

    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-last_time).count() > 1000) {
			std::cout << "[ldrobot] lidar pub data is time out, please check lidar device" << std::endl;
			exit(EXIT_FAILURE);
		}

    usleep(1000*100);  // sleep 100ms  == 10Hz
  }

  cmd_port->Close();

  delete lidar;
  lidar = nullptr;
  delete cmd_port;
  cmd_port = 0;

  return 0;
}

/********************* (C) COPYRIGHT SHENZHEN LDROBOT CO., LTD *******END OF
 * FILE ********/