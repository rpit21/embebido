#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/TwistStamped.h>
#include <iostream>
#include <sstream>

bool state_ready_to_pub = false;

std_msgs::Float64 vel_d;
std_msgs::Float64 vel_i;


void vel_callback(const geometry_msgs::TwistStamped::ConstPtr & msg)
{
	double v = msg->twist.linear.x;
	double w = msg->twist.angular.z;
	
	double v_i= v-w;
	double v_d= v+w;
	
	
	vel_d.data = v_d;
	vel_i.data = v_i;
	
	state_ready_to_pub = true;
	
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "transformacion_vel_setpoits");
  ros::NodeHandle n;
  
  ros::Rate loop_rate(1);
  
  //The node advertises the joint values of the pan-tilt
  ros::Publisher state_pub_D = n.advertise<std_msgs::Float64>("rigth/setpoint", 10);
  ros::Publisher state_pub_I = n.advertise<std_msgs::Float64>("left/setpoint", 10);
  
  ros::Subscriber sub_vel = n.subscribe("/cmd_vel",10,vel_callback);

 
 while(ros::ok()){
 
	 if(state_ready_to_pub){
	 	state_pub_D.publish(vel_d);
		state_pub_I.publish(vel_i);
	 	state_ready_to_pub = false;
	 }
  
 ros::spinOnce();
 }
  return 0;
}



