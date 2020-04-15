#include "Node.h"

#include <iostream>
#include <math.h>



Node::Node()
{
	value_ = SPACE;
	parent_ = nullptr;
	g_ = 0;
}

//
//Node::Node(const Node& other)
//{
//	std::cout<<"In copy node"<<std::endl;
//}

Node::Node(Point2D &pt, Point2D* t, int v, double g, Node* pr) {
	point_ = pt;
	target_ = t;
	value_ = v;
	parent_ = pr;
	this->g_ = g;
	h_ = compute_h();
}

void Node::set_value(const int value)
{
	this->value_ = value;
}

void Node::set_point(const Point2D point)
{
	this->point_ = point;
}

void Node::set_target(Point2D* target)
{
	target_ = target;
}

int Node::get_value() const
{
	return value_;
}

double Node::get_g() const
{
	return g_;
}



double Node::compute_h() const
{
	return sqrt(pow(point_.get_row()-target_->get_row(),2)+
		pow(point_.get_col() - target_->get_col(), 2));
}

double Node::get_f() const
{
	return g_+h_;
}

Point2D& Node::get_point()
{
	return point_;
}

Node * Node::get_parent() const
{
	return parent_;
}

Point2D * Node::get_target() const
{
	return target_;
}
