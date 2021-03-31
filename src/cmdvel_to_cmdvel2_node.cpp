#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Imu.h"

float gain = 1.8;
float offset = 0.2;
//float gain2 = 0.5;

float cmd_ang_vel=0.0;
float imu_ang_vel=0.0;

float linear_x = 0.0;
float angular_z=0.0;

void cmdvel_Callback(const geometry_msgs::Twist::ConstPtr& msg)
{
   linear_x = msg->linear.x;
   cmd_ang_vel=msg->angular.z;
}

void imu_Callback(const sensor_msgs::Imu::ConstPtr& msg)
{
   imu_ang_vel=msg->angular_velocity.z;
   float diff =0.0;

   if(cmd_ang_vel>0){
     diff = gain*(cmd_ang_vel-imu_ang_vel)+offset;
   }
   else if(cmd_ang_vel<0){
     diff = gain*(cmd_ang_vel-imu_ang_vel)-offset;
   }
   angular_z = cmd_ang_vel+diff;


}




int main(int argc, char **argv)
{
  ros::init(argc, argv, "cmdvel_to_cmdvel2_node");
  ros::NodeHandle nh;

  ros::Publisher cmdvel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
  ros::Subscriber cmdvel2_sub = nh.subscribe("cmd_vel2", 1000, cmdvel_Callback);
  ros::Subscriber imu_sub = nh.subscribe("/imu", 1000, imu_Callback);

  ros::Rate loop_rate(100);
  while (ros::ok())
  {
    geometry_msgs::Twist msg;
    msg.linear.x = linear_x;
    msg.angular.z= angular_z;


    cmdvel_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
