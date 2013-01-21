#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <vector>
using namespace std;

class Elevator
{
public:
    // descriptions for all of these functions exist in the source file: elevator.cpp

    Elevator(int = 1);
    void addFloor(int);
    void addOutsideFloor(int, bool);
    void deleteFloor(int);
    int getFloor(int);
    int getCurFloor();
    int getPrevFloor();
    vector<int> getFloors();
    void start();
    bool move();
    int getSize();

private:

    // holds the current floor
    int curFloor;

    // holds the previous floor that the elevator was in. whether the upper floor (if going down) or the lower floor (if going up)
    int prevFloor;

    // a vector of floors that the elevator has to stop in each one.
    // example: if the vector contains {2, 11, 4} the elevator will stop in each of those floors. However the order might differ.
    vector<int> vecFloors;

    // a vector of floors that are requested from outside of the elevator
    vector<int> vecHoldFloors;

    // true: the elevator is going Up -- false: the elevator is going Down
    bool moveDirection;

    bool process();

    // This function determines the direction of the elevator (whether it should go up or down)
    void setDirection();

    // This function will add all elements of vecHoldFloors to vecFloors when needed
    void mergeVectors();
};

#endif // ELEVATOR_H
