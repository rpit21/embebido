#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/TwistStamped.h>
#include <iostream>
#include <sstream>




int main(int argc, char **argv)
{
  ros::init(argc, argv, "robot_description");
  ros::NodeHandle n;
  
  ros::Rate loop_rate(1);
  
  //The node advertises the joint values of the pan-tilt
  ros::Publisher state_pub_D = n.advertise<std_msgs::Float64>("rigth/setpoint", 10);
  ros::Publisher state_pub_I = n.advertise<std_msgs::Float64>("left/setpoint", 10);

 
 while(ros::ok()){
 std_msgs::Float64 VM;
 
 VM.data=5;
 
 state_pub_D.publish(VM);
 
 VM.data=-5;
 state_pub_I.publish(VM);
  
 ros::spinOnce();
 loop_rate.sleep();
 }
  return 0;
}



