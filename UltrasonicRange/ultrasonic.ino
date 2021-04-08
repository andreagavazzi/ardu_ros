//Specific libraries
#include <ros.h>
#include <ros/time.h>
#include "Ultrasonic.h"
#include <sensor_msgs/Range.h>

Ultrasonic ultrasonic_left(2);
Ultrasonic ultrasonic_right(4);
unsigned long range_timer;


//create the ros node nh. The node will be used to publish to Arduino
ros::NodeHandle nh;


void sensor_msg_init(sensor_msgs::Range &range_name, char *frame_id_name)
{
  range_name.radiation_type = sensor_msgs::Range::ULTRASOUND;
  range_name.header.frame_id = frame_id_name;
  range_name.field_of_view = 0.26;
  range_name.min_range = 0.03;
  range_name.max_range = 3.50;
}


//Create instances for range messages.
sensor_msgs::Range range_left;
sensor_msgs::Range range_right;
 
//Create publisher objects for all sensors
ros::Publisher pub_range_left("/ultrasound_left", &range_left);
ros::Publisher pub_range_right("/ultrasound_right", &range_right);


void setup() {
   //Serial.begin(9600);
   nh.initNode();
   nh.advertise(pub_range_left);
   nh.advertise(pub_range_right);
 
   sensor_msg_init(range_left, "/ultrasound_left");
   sensor_msg_init(range_right, "/ultrasound_right");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis >= range_timer+50) //publish every 50 milliseconds
  {
    range_left.range   = ultrasonic_left.MeasureInCentimeters()/100.0;
    range_right.range  = ultrasonic_right.MeasureInCentimeters()/100.0;

    range_left.header.stamp = nh.now();
    range_right.header.stamp = nh.now();
 
    pub_range_left.publish(&range_left);
    pub_range_right.publish(&range_right);

    range_timer = currentMillis+50;

    }
   
   nh.spinOnce();
}
