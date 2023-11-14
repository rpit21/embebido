/**
 * @author  Dwindra Sulistyoutomo
 */
#include <ros/ros.h>
#include <iostream>	
#include <chrono>
#include <wiringPi.h>
#include <geometry_msgs/Vector3.h>

// Pines encoder Izquierdo
int I_pinA=7;
int I_pinB=8;

//Pines encoder Derecho
int D_pinA=23;
int D_pinB=24;

int PPR=11; // resolucion del enconder, 1 vuelta -> 11 pulsos

volatile long tiempo_pas=0;
volatile int pulse_count_encI=0; // conteo de pulsos encoder izquierdo
volatile int pulse_count_encD=0; // conteo de pulsos encoder derecho


ros::Publisher encoders_pub;



void handleEncoder(){

	int stateA_I = digitalRead(I_pinA);
	int stateB_I = digitalRead(I_pinB);

	if (stateA_I==1 & stateB_I==1){
		pulse_count_encI++;
		std::cout<<"Motor I: Antihorario \n";
	}
	else if (stateA_I==1 & stateB_I==0){
		pulse_count_encI--;
		std::cout<<"Motor I: Horario \n";
	}

}

void handleEncoder2(){

	int stateA_D = digitalRead(D_pinA);
	int stateB_D= digitalRead(D_pinB);

	if (stateA_D==1 & stateB_D==1){
		pulse_count_encD++;
		std::cout<<"Motor D: Antihorario \n";
	}
	else if (stateA_D==1 & stateB_D==0){
		pulse_count_encD--;
		std::cout<<"Motor D: Horario \n";
	}

}

void calculate_RPM(){
	auto tiempo_act= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	// condicion para procesar la cantidad de pulsos tomados en 1seg y convertirlo en rpm
	if ((tiempo_act-tiempo_pas)>=1000){

		float rpm_I=pulse_count_encI*(60/(PPR*34)); // rpm tomando en cuenta una caja de reduccion 1:34 Encoder Derecho
		float rpm_D=pulse_count_encD*(60/(PPR*34)); // rpm tomando en cuenta una caja de reduccion 1:34 Encoder Izquierdo

		//publicacion el mensaje al topico
		geometry_msgs::Vector3 velocidades;
		velocidades.x=rpm_I;
		velocidades.y=rpm_D;

		encoders_pub.publish(velocidades);


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


    encoders_pub= nh.advertise<geometry_msgs::Vector3>("/rpm",10); // configuramos el publisher

    //Inicializacion de la libreria wiring PI
    wiringPiSetup();

    // definir entradas para pines de encoder Izquierdo
    pinMode(I_pinA,INPUT);
    pinMode(I_pinB,INPUT);

    // definir entradas para pines de encoder Derecho
    pinMode(D_pinA,INPUT);
    pinMode(D_pinB,INPUT);

    wiringPiISR(I_pinA,INT_EDGE_RISING , &handleEncoder); // Interrupcion para cambios en el pin A encoder Izquiero
    wiringPiISR(D_pinA,INT_EDGE_RISING , &handleEncoder2); // Interrupcion para cambios en el pin A encoder Derecho

    while(ros::ok()){

    	calculate_RPM();

    	ros::spinOnce();

    }

return 0;
}