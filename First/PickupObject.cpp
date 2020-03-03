#include "PickupObject.h"


PickupObject::PickupObject(Point2D* position, PickupType type, int quantity) : m_myType(type), m_quantity(quantity)
{
	m_position = position;
}


void PickupObject::Pickup()
{
	--m_quantity;
}

Point2D* PickupObject::GetPosition()
{
	return m_position;
}

int PickupObject::GetQuantity()
{
	return m_quantity;
}


PickupType PickupObject::GetType()
{
	return m_myType;
}
