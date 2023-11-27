/*
 *  This code will subscriber integer values from demo_topic_publisher
*/


#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <cmath>
#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <sstream>


int range = 255;

int sentido;
int stop;

int L;


//definicion de pines para motor 1
#define MD_IN1 5 // Pin In1 Motor Derecho
#define MD_IN2 6 // Pin In2 Motor Derecho

#define MD_ENA 13 // pin del pwm Motor Derecho

//definicion de pines para motor 2

#define MI_IN1 17 // Pin In1 Motor Izquierdo
#define MI_IN2 27 // Pin In2 Motor Izquierdo

#define MI_ENA 12 // pin del pwm Motor Izquierdo




void callback(const geometry_msgs::Twist::ConstPtr & msg)
{

	
	float v = msg->linear.x;
	float w = msg->angular.z;
	
	
	
	//transformacion de volaje a 255
	//v=v*(13.78);
	//w=w*(13.78);


	//ecuacion que describe el movimiento del carro
	int VL= static_cast<int>(v-w);
	int VR= static_cast<int>(v+w);

	//Adecuacion de limites
	if ( VL>255){VL=255;}
	if(VL<-255){ VL=-255;}
	if ( VR>255){VL=255;}
	if(VR<-255){ VL=-255;}


	if (VL>0){
		digitalWrite(MI_IN1, HIGH);
		digitalWrite(MI_IN2, LOW);
	} else if (VL<0){
		digitalWrite(MI_IN1, LOW);
		digitalWrite(MI_IN2, HIGH);
	}

	if (VR>0){
			digitalWrite(MD_IN1, HIGH);
			digitalWrite(MD_IN2, LOW);
		} else if (VR<0){
			digitalWrite(MD_IN1, LOW);
			digitalWrite(MD_IN2, HIGH);
		}



	softPwmWrite(MD_ENA,abs(VR));
	softPwmWrite(MI_ENA,abs(VL));


	std::cout<<"pwm MD set in "<<VL<<"\n";
	std::cout<<"pwm MI set in "<<VR<<"\n";
}


int main(int argc, char **argv)

{
	//Inicio de Ros
	ros::init(argc, argv,"subs_teleop");

	//Created a nodehandle object

	ros::NodeHandle node_obj;

	//Crear subscriber para cada GPIO

	ros::Subscriber teleop_subscriber = node_obj.subscribe("/voltaje",10,callback);


	//Configuracion de pines

	wiringPiSetupGpio();
	ROS_INFO("GPIO has been set as OUTPUT.");

	pinMode(MD_ENA,OUTPUT);
	pinMode(MD_IN1, OUTPUT);
	pinMode(MD_IN2, OUTPUT);

	pinMode(MI_ENA,OUTPUT);
	pinMode(MI_IN1, OUTPUT);
	pinMode(MI_IN2, OUTPUT);


	// inicializamos al pin como pwm con un rango de 0 a 255
	softPwmCreate(MD_ENA,0,range);
	softPwmCreate(MI_ENA,0,range);

	//Spinning the node

	ros::spin();

	return 0;
}


