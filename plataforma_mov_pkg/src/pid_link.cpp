#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/TwistStamped.h>
#include <iostream>
#include <sstream>

bool state_ready_to_pub = false;
bool control_ready_to_pub_d=false;
bool control_ready_to_pub_i=false;

std_msgs::Float64 vel_d;
std_msgs::Float64 vel_i;

std_msgs::Float32 control_action_d;
std_msgs::Float32 control_action_i;

void vel_callback(const geometry_msgs::TwistStamped::ConstPtr & msg)
{
	double v_d = msg->twist.angular.x;
	double v_i = msg->twist.angular.y;
	
	
	vel_d.data = v_d;
	vel_i.data = v_i;
	
	state_ready_to_pub = true;
	
	
}

void act_callback_D(const std_msgs::Float64::ConstPtr & msg)
{
	double a = msg->data;
	
	control_action_d.data = a;
	control_ready_to_pub_d = true;
}

void act_callback_I(const std_msgs::Float64::ConstPtr & msg)
{
	double a = msg->data;
	
	control_action_i.data = a;
	control_ready_to_pub_i= true;
}




int main(int argc, char **argv)
{
  ros::init(argc, argv, "robot_description");
  ros::NodeHandle n;
  
  //The node advertises the joint values of the pan-tilt
  ros::Publisher state_pub_D = n.advertise<std_msgs::Float64>("rigth/state", 10);
  ros::Publisher state_pub_I = n.advertise<std_msgs::Float64>("left/state", 10);
  
  ros::Publisher control_pub_d = n.advertise<std_msgs::Float32>("motorD/command", 10);
  ros::Publisher control_pub_i = n.advertise<std_msgs::Float32>("motorI/command", 10);
  
  ros::Subscriber sub_vel = n.subscribe("motors/velocity",10,vel_callback);
  
  ros::Subscriber sub_eff_D = n.subscribe("rigth/control_effort",10,act_callback_D);
  ros::Subscriber sub_eff_I = n.subscribe("left/control_effort",10,act_callback_I);
  
  std::cout<<" Ready link controller"<<std::endl; 
 
 while(ros::ok()){
 if(state_ready_to_pub){
 	state_pub_D.publish(vel_d);
	state_pub_I.publish(vel_i);
 	state_ready_to_pub = false;
 }
  if(control_ready_to_pub_d){
 	control_pub_d.publish(control_action_d);
 	control_ready_to_pub_d = false;
 }
 if(control_ready_to_pub_i){
	control_pub_i.publish(control_action_i);
 	control_ready_to_pub_i = false;
 }
 ros::spinOnce();
 }
  return 0;
}



