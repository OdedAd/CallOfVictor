#pragma once

#include "Point2D.h"

static enum class PickupType {
	Ammo,
	Medkit
};

class PickupObject
{

public:

	PickupObject(Point2D* position , PickupType type, int quantity = 10);
	void Pickup();

	Point2D* GetPosition();
	int GetQuantity();
	PickupType GetType();

private:

	Point2D* m_position;
	int m_quantity;
	PickupType m_myType;


};

