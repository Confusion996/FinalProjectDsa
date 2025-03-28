#ifndef VEHICLE_H
#define VEHICLE_H

typedef struct Vehicle {
    int id;
    int type; // 0 = car, 1 = truck, 2 = emergency
    float x, y; // Position on the road
    struct Vehicle* next; // Linked list pointer
} Vehicle;

Vehicle* createVehicle(int id, int type, float x, float y);
void freeVehicle(Vehicle* v);

#endif
