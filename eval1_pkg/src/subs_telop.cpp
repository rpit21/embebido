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
int sentido;
int stop;

#define IN1 3 // change pin number here
#define IN2 4 // change pin number here

#define LED1 0
#define LED2 5
#define LED3 6
#define LED4 13
#define LED5 19

void callback(const geometry_msgs::Twist::ConstPtr & msg)
{

	int vel = msg->linear.x;

	vel=10*vel;
	pwm=pwm+vel;

	if (stop==1){
		pwm=0;

	} else {

		if (pwm>=range){
			pwm=100;
		}
		else if (pwm<=0){
			pwm=0;
		}
	}

	if (pwm==20){
		digitalWrite(LED1, HIGH);
		digitalWrite(LED2, LOW);
		digitalWrite(LED3, LOW);
		digitalWrite(LED4, LOW);
		digitalWrite(LED5, LOW);

	}else if (pwm==40){
		digitalWrite(LED1, HIGH);
		digitalWrite(LED2, HIGH);
		digitalWrite(LED3, LOW);
		digitalWrite(LED4, LOW);
		digitalWrite(LED5, LOW);

	}else if (pwm==60){
		digitalWrite(LED1, HIGH);
		digitalWrite(LED2, HIGH);
		digitalWrite(LED3, HIGH);
		digitalWrite(LED4, LOW);
		digitalWrite(LED5, LOW);


	}else if (pwm==80){
		digitalWrite(LED1, HIGH);
		digitalWrite(LED2, HIGH);
		digitalWrite(LED3, HIGH);
		digitalWrite(LED4, HIGH);
		digitalWrite(LED5, LOW);


	}else if (pwm>=100){
		digitalWrite(LED1, HIGH);
		digitalWrite(LED2, HIGH);
		digitalWrite(LED3, HIGH);
		digitalWrite(LED4, HIGH);
		digitalWrite(LED5, HIGH);
	}else{
		digitalWrite(LED1, LOW);
		digitalWrite(LED2, LOW);
		digitalWrite(LED3, LOW);
		digitalWrite(LED4, LOW);
		digitalWrite(LED5, LOW);
	}

	softPwmWrite(pin,pwm);

	std::cout<<"pwm set in "<<pwm<<"\n";
}

void callback_sentido (const std_msgs::Int32::ConstPtr & msg){

	sentido = msg->data;

	if (sentido==1){
		digitalWrite(IN1, HIGH);
		digitalWrite(IN2, LOW);
		std::cout<<"Sentido:Horario"<<"\n";
	}else{
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, HIGH);
		std::cout<<"Sentido:Antihorario"<<"\n";
	}

}

void callback_parada (const std_msgs::Int32::ConstPtr & msg){
	stop=msg->data;
}


int main(int argc, char **argv)

{
	ros::init(argc, argv,"subs_teleop");

	//Created a nodehandle object

	ros::NodeHandle node_obj;

	//crear suscriber
	ros::Subscriber teleop_subscriber = node_obj.subscribe("/turtle1/cmd_vel",10,callback);

	ros::Subscriber sentido_subs = node_obj.subscribe("/sw",10,callback_sentido);

	ros::Subscriber parada_subs = node_obj.subscribe("/pul",10,callback_parada);


	//configuracion de pines como salida

	wiringPiSetupGpio();
	ROS_INFO("GPIO has been set as OUTPUT.");

	pinMode(pin,OUTPUT);
	pinMode(IN1, OUTPUT);
	pinMode(IN2, OUTPUT);

	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);
	pinMode(LED5, OUTPUT);

	// inicializamos al pin como pwm con un rango
	softPwmCreate(pin,0,range);

	//Spinning the node
	ros::spin();
	return 0;
}


