#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>


class TrikTeleop {
public:
  TrikTeleop();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

  int joyLinearIndex, joyAngularIndex;
  double linearScale, angularScale;

  ros::NodeHandle nh;
  ros::Publisher velocityPublisher;
  ros::Subscriber joySubscriber;
};


TrikTeleop::TrikTeleop() {
  // Try to get params from params server or use default if fail
  nh.param("axis_linear", joyLinearIndex, 1);
  nh.param("axis_angular", joyAngularIndex, 2);
  nh.param("scale_linear", linearScale, 1.0);
  nh.param("scale_angular", angularScale, 1.0);

  velocityPublisher = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);
  joySubscriber = nh.subscribe<sensor_msgs::Joy>("joy", 10, &TrikTeleop::joyCallback, this);
}

void TrikTeleop::joyCallback(const sensor_msgs::Joy::ConstPtr& joy) {
  geometry_msgs::Twist twist;
  twist.angular.z = angularScale * joy->axes[joyAngularIndex];
  twist.linear.x = linearScale * joy->axes[joyLinearIndex];
  velocityPublisher.publish(twist);
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "trik_teleop");
  TrikTeleop trik_teleop;

  ros::spin();
}
