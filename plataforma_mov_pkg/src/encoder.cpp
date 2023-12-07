/**
 * @author  Dwindra Sulistyoutomo
 */
#include <ros/ros.h>
#include <iostream>	
#include <chrono>
#include <wiringPi.h>
#include <geometry_msgs/TwistStamped.h>
#include "std_msgs/Float32.h"


// Pines fisicos encoder Izquierdo
int I_pinA=26;
int I_pinB=24;

//Pines fisicos encoder Derecho
int D_pinA=16;
int D_pinB=18;

int PPR=11.0; // resolucion del enconder, 1 vuelta -> 11 pulsos

volatile long tiempo_pas=0;
volatile int pulse_count_encI=0; // conteo de pulsos encoder izquierdo
volatile int pulse_count_encD=0; // conteo de pulsos encoder derecho


bool sent=false;


//ros::Publisher enc_MD_pub;
//ros::Publisher enc_MI_pub;

ros::Publisher enc_motors;



void handleEncoder(){

	int stateA_I = digitalRead(I_pinA);
	int stateB_I = digitalRead(I_pinB);
	pulse_count_encI++;

	if (stateA_I==1 & stateB_I==1){

		sent=false;
	}
	else if (stateA_I==1 & stateB_I==0){
		
		sent=true;
		
	}

}

void handleEncoder2(){

	int stateA_D = digitalRead(D_pinA);
	int stateB_D= digitalRead(D_pinB);

	if (stateA_D==1 & stateB_D==1){
		pulse_count_encD++;
		std::cout<<"Motor D: Horario \n"<<std::endl;
	}
	else if (stateA_D==1 & stateB_D==0){
		pulse_count_encD--;
		std::cout<<"Motor D: AntiHorario \n"<<std::endl;
	}

}

void provisional_handleEncoder2 (){
	int stateB_D= digitalRead(D_pinB);
	pulse_count_encD++;
	
	}

void provisional_handleEncoder (){
	int stateB_I= digitalRead(D_pinB);
	pulse_count_encI++;
	
	}

void calculate_RPM(){
	auto tiempo_act= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	// condicion para procesar la cantidad de pulsos tomados en 1seg y convertirlo en rpm
	if ((tiempo_act-tiempo_pas)>=63){

		//float rpm_I=pulse_count_encI*(60.0/(PPR*34.0));// rpm tomando en cuenta una caja de reduccion 1:34 Encoder Derecho
		//float rpm_D=pulse_count_encD*(60.0/(PPR*34.0));// rpm tomando en cuenta una caja de reduccion 1:34 Encoder Izquierdo
		
		float rpm_I=pulse_count_encI*(960.0/(PPR*20.74));// rpm tomando en cuenta una caja de reduccion 1:34 Encoder Derecho
		float rpm_D=pulse_count_encD*(960.0/(PPR*20.74));// rpm tomando en cuenta una caja de reduccion 1:34 Encoder Izquierdo
		
		float radS_I= rpm_I*3.1415/30;
		float radS_D= rpm_D*3.1415/30;
		
		
		if (sent==true){
			radS_D=radS_D*-1;
			radS_I=radS_I*-1;
			
		}

		//publicacion el mensaje al topico
		
		geometry_msgs::TwistStamped velocidades_M;
		
		velocidades_M.header.stamp=ros::Time::now();
		
		velocidades_M.twist.angular.x = radS_D;
		velocidades_M.twist.angular.y = radS_I;

		//enc_MD_pub.publish(velocidad_MD);
		//enc_MI_pub.publish(velocidad_MI);
		
		enc_motors.publish(velocidades_M);
		
		std::cout<<"\tVelocidades de cada Motor"<<"\n";
		std::cout<<" Rad/s motor Izquierdo:"<<radS_I<<"\n";
		std::cout<<" Rad/s motor Derecho:"<<radS_D<<"\n";
		std::cout<<"---------------------------------------------------"<<"\n";


		//Actualizamos variable de tiempo pasado, y e inicializamos el conteo de los pulsadores
		pulse_count_encI=0;
		pulse_count_encD=0;
		tiempo_pas=tiempo_act;
		
	}

}

int main (int argc, char **argv)
{

    ros::init(argc, argv, "encoders");
    ros::NodeHandle nh;

    //enc_MD_pub= nh.advertise<std_msgs::Float32>("motorD/velocity",10); // configuramos el publisher
    //enc_MI_pub= nh.advertise<std_msgs::Float32>("motorI/velocity",10);
    
    enc_motors= nh.advertise<geometry_msgs::TwistStamped>("motors/velocity",10);

    //Inicializacion de la libreria wiring PI
    //wiringPiSetup();
    wiringPiSetupPhys(); // inicializa con pines fisicos

    // definir entradas para pines de encoder Izquierdo
    pinMode(I_pinA,INPUT);
    pinMode(I_pinB,INPUT);

    // definir entradas para pines de encoder Derecho
    pinMode(D_pinA,INPUT);
    pinMode(D_pinB,INPUT);

    wiringPiISR(I_pinA,INT_EDGE_RISING , &handleEncoder); // Interrupcion para cambios en el pin A encoder Izquiero
    //wiringPiISR(D_pinA,INT_EDGE_RISING , &handleEncoder2); // Interrupcion para cambios en el pin A encoder Derecho
    wiringPiISR(D_pinB,INT_EDGE_RISING , &provisional_handleEncoder2); // interrupcion provisional por error en el encoder

    while(ros::ok()){

    	calculate_RPM();

    	ros::spinOnce();

    }

return 0;
}
