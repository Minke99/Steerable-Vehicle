/* 
 * Auto Mode:    controls vehicle automatically 
 *                  for wall following
 * File:   AutoTraverse.h
 * Author: Jiashu Wu, Jay Zhang
 * Last updated on 19 Nov 2021, 3:03 PM
 */

#ifndef AUTOTRAVERSE_H
#define	AUTOTRAVERSE_H

// Function for performing Wall Following
// Input: Void
// Output: integer value of 0-250 for steering input
int WallFollowing(void);

// Function for detection of obstacle
// Input: Void
// Output: 1 if obstacle is detected, 0 if not
unsigned char IfObstacle(void);

// Caller function for auto state
// Input: Void
// Output: Void
void AutoTraverse(void);

#endif

