#include "elevator.h"
#include <algorithm>
#include <vector>

// The constructor. Initializes the elevator with the passed value as the initial floor
Elevator::Elevator(int floor)
{
    curFloor = floor;
}

// Adds the passed value (floor number) to the stop list
void Elevator::addFloor(int floor)
{
    vecFloors.push_back(floor);
}

// This function to process the stops that are requested from outside of the elevetor in each floor
void Elevator::addOutsideFloor(int floor, bool btnUpDown)
{
    // isPresent indicates whether the stop request already exists in the stop list
    bool isPresent = (std::find(vecFloors.begin(), vecFloors.end(), floor) != vecFloors.end());

    // isPresentInHold holds whether the requested floor exists in the hold or not
    bool isPresentInHold = (std::find(vecHoldFloors.begin(), vecHoldFloors.end(), floor) != vecHoldFloors.end());

    // decides whether the requested floor can be instantly added to stop list or it should wait until the direction changes
    switch (moveDirection)
    {
        case true: // if the elevator is going up

        // if requested floor is upper than the current one and is not in wait list and the up button was pressed
        if (floor >= curFloor && !isPresent && btnUpDown)
            vecFloors.push_back(floor); // adds the floor to the stop list

        else // if any of the conditions in the above if didn't satisfy
            if (!isPresentInHold && (floor < curFloor || !btnUpDown))
                vecHoldFloors.push_back(floor);
        break;

        case false: // if the elevator is going down

        // if requested floor is lower than the current one and is not in wait list and the down button was pressed
        if (floor <= curFloor && !isPresent && !btnUpDown)
            vecFloors.push_back(floor); // adds the floor to the stop list

        else // if any of the conditions in the above if didn't satisfy
            if (!isPresentInHold && (floor > curFloor || btnUpDown))
                vecHoldFloors.push_back(floor);
    }
}

// remove a floor from stop list (this will be called each time the elevator reaches a floor)
void Elevator::deleteFloor(int floor)
{
    vecFloors.erase(std::find(vecFloors.begin(), vecFloors.end(), floor));
}

// returns the total number of the floors in the stop list
int Elevator::getSize()
{
    return vecFloors.size();
}

// returns the floor associated with the passed index
int Elevator::getFloor(int index)
{
    return vecFloors[index];
}

// a utility function that returns the current floor
int Elevator::getCurFloor()
{
    return curFloor;
}

// a utility function that returns the previous floor
int Elevator::getPrevFloor()
{
    return prevFloor;
}

// returns the vector of stop list
vector<int> Elevator::getFloors()
{
    return vecFloors;
}

// used to initialize. at start, the moving direction is upwards and the elevator is at the first floor.
void Elevator::start()
{
    moveDirection = true;
    curFloor = 1;
}

// determines the direction in which the elevator should move
void Elevator::setDirection()
{

    if (vecFloors.size() < 1) // if there are no floors in the stop list
        mergeVectors(); // elements of vecHoldFloors will be added to vecFloors (main stop list) and vecHoldFloors will be cleared

    if (moveDirection) // if elevator is going up
    {
        if (vecFloors.size() == 1) // whenever there is only one floor remaining in the stop list

            // if the current floor number is bigger than the smallest floor number in the stop list
            if (curFloor > *std::min_element(vecFloors.begin(),vecFloors.end()))
                moveDirection = false; // moving direction will be changed to downwards
    }
    else // if elevator is going down
    {
        if (vecFloors.size() == 1) // if there is only one floor remaining in the stop list

            // if the current floor number is smaller than the biggest floor number in the stop list
            if (curFloor < *std::max_element(vecFloors.begin(),vecFloors.end()))
                moveDirection = true; // set the moving direction to upwards

    }
}

bool Elevator::move()
{
    setDirection(); // determines the direction before moving

    if (!(vecFloors.empty() && vecHoldFloors.empty())) // if stop list and hold list are not empty
    {
        // stores the value of current floor in prevFloor variable.
        // When the elevator moves, the current floor will become the previous one
        prevFloor = curFloor;

        if (moveDirection) // if elevator is going up
            curFloor++; // go to the next floor, which is the one above the current one
        else // if elevator is going down
            curFloor--; // go to the next floor, which is the one below the current one

        return process(); // calls process function and returns the return value of it
    }
}

// returns true if the current floor exists in the stop list (if the elevator has to stop in the current floor)
bool Elevator::process()
{
    int intCount = vecFloors.size(); // gets the size of stop list
    for (int i = 0; i < intCount; i++) // loops through all of members (floors)
        if (vecFloors[i] == curFloor)
            return true; // if current floor exists in the stop list, the function will return true
}

// Merges vecHoldFloors with vecFloors
void Elevator::mergeVectors()
{
    // inserts all members of the hold list (vecHoldFloors) to the stop list (vecFloors)
    vecFloors.insert(vecFloors.begin(),vecHoldFloors.begin(),vecHoldFloors.end());

    // removes all of the elements of the hold list (vecHoldFloors) after they are inserted to the stop list
    vecHoldFloors.clear();
}
