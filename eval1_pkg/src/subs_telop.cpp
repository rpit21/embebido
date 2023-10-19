/*
 *  This code will subscriber integer values from demo_topic_publisher
*/


#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <std_msgs/Int32.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <sstream>


int range = 100;
int pin = 12; // pin del pwm
int pwm;

void callback(const geometry_msgs::Twist::ConstPtr & msg)
{

	int vel = msg->linear.x;

	vel=10*vel;
	pwm=pwm+vel;

	if (pwm>=range){
		pwm=100;
	}
	else if (pwm<=0){
		pwm=0;
	}

	softPwmWrite(pin,pwm);

	std::cout<<"pwm set in "<<pwm<<"\n";
}

int main(int argc, char **argv)

{
	ros::init(argc, argv,"subs_teleop");

	//Created a nodehandle object

	ros::NodeHandle node_obj;

	//crear suscriber
	ros::Subscriber teleop_subscriber = node_obj.subscribe("/turtle1/cmd_vel",10,callback);

	//configuracion de pines como salida

	wiringPiSetupGpio();
	ROS_INFO("GPIO has been set as OUTPUT.");

	pinMode(pin,OUTPUT);

	// inicializamos al pin como pwm con un rango
	softPwmCreate(pin,0,range);

	//Spinning the node
	ros::spin();
	return 0;
}


