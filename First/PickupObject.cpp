#include "PickupObject.h"


PickupObject::PickupObject(Point2D* position, const PickupType type, const int quantity)
	: m_quantity_(quantity), m_my_type_(type)
{
	m_position_ = position;
}


void PickupObject::pickup()
{
	--m_quantity_;
}

Point2D* PickupObject::get_position() const
{
	return m_position_;
}

int PickupObject::get_quantity() const
{
	return m_quantity_;
}


PickupType PickupObject::get_type() const
{
	return m_my_type_;
}
