#include <ros/ros.h>
#include <wiringPi.h>
#include <std_msgs/Int32.h>
#include <iostream>
#include <sstream>

int main (int argc, char **argv)
{
	int sw = 17;  //switch pin
	int pul= 27; // pulsador pin

    ros::init(argc, argv, "gpio_read_publisher");
    ros::NodeHandle nh;
    ros::Publisher sw_status = nh.advertise<std_msgs::Int32>("/sw",10);
    ros::Publisher pul_status = nh.advertise<std_msgs::Int32>("/pul",10);

    wiringPiSetupGpio();
    pinMode(sw, INPUT);
    pinMode(pul, INPUT);

    std::cout<<"Pin "<<sw<<" Set as INPUT"<<"\n";
    std::cout<<"Pin "<<pul<<" Set as INPUT"<<"\n";

    ros::Rate loop_rate(3);

    while (ros::ok())
    {
    	std_msgs::Int32 statusw;
    	std_msgs::Int32 statupul;

        int giro = digitalRead(sw);
        int paro = digitalRead(pul);

        statusw.data = giro;
        sw_status.publish(statusw);

        statupul.data=paro;
        pul_status.publish(statupul);




        if(paro==1){
        	std::cout<<"Paro"<<"\n";
        }else{

        	if(giro==1){
        	    std::cout<<"Horario"<<"\n";
        	}else {
        	    std::cout<<"Antihorario"<<"\n";
        	}
        }


        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
