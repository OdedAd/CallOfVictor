#pragma once

#include "Point2D.h"

enum class PickupType {
	ammo,
	med_kit
};

class PickupObject
{

public:

	PickupObject(Point2D* position , PickupType type, int quantity = 20);
	void pickup();

	Point2D* get_position() const;
	int get_quantity() const;
	PickupType get_type() const;

private:

	Point2D* m_position_;
	int m_quantity_;
	PickupType m_my_type_;


};

