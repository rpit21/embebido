
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64.h>


#define b  0.7327
#define r  0.062



std::vector<double> x;

std_msgs::Float64 vel_d;
std_msgs::Float64 vel_i;

bool state_ready_to_pub = false;

bool obs = false;


double xd=0;
double yd=0;
double kv;
double kw;

//ros::Publisher cmd_vel_pub_ ;
ros::Publisher state_pub_D;
ros::Publisher state_pub_I;

void ultCallback(const geometry_msgs::Twist::ConstPtr& msg)
{

	
	float i= msg->linear.x;
	float c = msg->linear.y;
	float d = msg->linear.z;
	
	if (i<=30.0||c<=30.0||d<=30.0) {
	   obs=true;
	   }
	
	
}


std::vector<std::vector<double>> waypoints = {
        {0.5, 0},
        {1, 0},
	{1, 1}
	
      
    };
double waypoint_count = 0;



/*******************************************************************************
* Control Loop function
*******************************************************************************/
bool controlLoop()
{
	double wp_x = waypoints[waypoint_count][0];
	double wp_y = waypoints[waypoint_count][1];
	std::cout<<"x: "<<wp_x<<"y: "<<wp_y<<std::endl;

	//error
	double ex = wp_x- x[0];
	double ey = wp_y - x[1];
	
	
	double err_norm = sqrt(ex*ex + ey*ey);
	if(err_norm<=0.3){
	waypoint_count++;	
	}

	double v,w;
	if(waypoint_count<waypoints.size()){
	

	 double th_g = atan2(ey,ex+1e-6)-x[2];
	 
	 v = kv*sqrt(ex*ex + ey*ey)*cos(th_g);
	 w = kv*cos(th_g) * sin(th_g) + kw*th_g;
	 }else{
	 v=0;
	 w=0;
	 }

	 //accion de control
	 //aqui se deberia calcular y publicar wr y wl
	  double wr = (v+w*b/2.0)/r;
	  double wl = (v-w*b/2.0)/r;
	  
	  

	 //la simulación me permite enviar directamente velocidad lineal y angular
	 //geometry_msgs::Twist cmd_vel;
	 //cmd_vel.linear.x = v;
	 //cmd_vel.angular.z = w;
	 //cmd_vel_pub_.publish(cmd_vel);
	 
	 if(obs==true){
	 vel_d.data = 0.0;
	 vel_i.data = 0.0;
	 
	 obs = false;
	    }else{
	       
	 vel_d.data = wr;
	 vel_i.data = wl;
	       }
	
	 
	 
	 state_ready_to_pub = true;
	 
	 //state_pub_D.publish(vel_d);
	 //state_pub_I.publish(vel_i);
	 
	 //std::cout<<" x: "<<x[0]<<" y: "<<x[1]<<" th: "<<x[2]<<" v "<<v<<" vx: "<<cmd_vel.linear.x<<" vy "<<cmd_vel.linear.y  <<" w "<<w<<"\n";
	 


  return true;
}

void odomCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
	//get yaw angle from quaternion
	double siny = 2.0 * (msg->pose.pose.orientation.w * msg->pose.pose.orientation.z + msg->pose.pose.orientation.x * msg->pose.pose.orientation.y);
	double cosy = 1.0 - 2.0 * (msg->pose.pose.orientation.y * msg->pose.pose.orientation.y + msg->pose.pose.orientation.z * msg->pose.pose.orientation.z);
	double theta = atan2(siny, cosy);

	//get position from odometry
  x[0] = msg->pose.pose.position.x;
  x[1] = msg->pose.pose.position.y;
  x[2] = theta;
  controlLoop();
  std::cout<<"odom vx "<<msg->pose.pose.position.x<<" vy "<<msg->pose.pose.position.y<<" th "<<theta<<"\n";


}




/*******************************************************************************
* Main function
*******************************************************************************/
int main(int argc, char* argv[])
{
  ros::init(argc, argv, "turtlebot3_drive");
  // initialize ROS parameter
  ros::NodeHandle nh_("~");

   nh_.param<double>("x_setpoint",xd,0);
   nh_.param<double>("y_setpoint",yd,0);
   nh_.param<double>("kv",kv,0);
   nh_.param<double>("kw",kw,0);

    std::cout<<"set point: "<< waypoints[waypoint_count][0]<<" , "<< waypoints[waypoint_count][1]<<"\n";
   //state size
   x.resize(3);

   // velocity publishers
   //cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
   
   state_pub_D = nh_.advertise<std_msgs::Float64>("/rigth/setpoint", 10);
   state_pub_I = nh_.advertise<std_msgs::Float64>("/left/setpoint", 10);

   // odometry subscribers
   ros::Subscriber odom_sub_ = nh_.subscribe("/kinematics_odom", 10, odomCallback);
   ros::Subscriber ult_sub = nh_.subscribe("/ultrasonics", 10, ultCallback);
   
   //ros::Subscriber odom_sub_ = nh_.subscribe("/odometry/filtered", 10, odomCallback);
   
   while(ros::ok()){
 
	 if(state_ready_to_pub){
	 	state_pub_D.publish(vel_d);
		state_pub_I.publish(vel_i);
		std::cout<<"wr: "<<vel_d.data<<" wl "<<vel_i.data  <<"\n";
	 	state_ready_to_pub = false;
	 }
  
 ros::spinOnce();
 }

  return 0;
}
