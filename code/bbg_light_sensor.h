#ifndef _BBG_LIGHT_SENSOR_H_
#define _BBG_LIGHT_SENSOR_H_

/**   A2D Reading	   % Value		        Light Level
 * ------------------------------------------------------------------
 * 		   0.37			 0.00		        Pitch black	
 * 		   9.19			 0.22		        Dim hallway
 * 		  60.50			 1.48		        Moonlit night
 * 		 161.69			 3.95		  Twilight under clear sky
 * 		 372.27			 9.09		         Dark room
 * 		1461.24			35.68		          Hallway
 * 		1638.21			40.00		Bright room/Dark overcast day
 * 		2769.66			67.64		 Sunrise/sunset on clear day
 * 		3343.12			81.64		       Overcast day
 * 		3998.44			97.64		       Full daylight
 * 		4063.69			99.23		        Direct sun
 */

void initLightSensor();

double getLightReading(void);

#endif
