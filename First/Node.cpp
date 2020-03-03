#include "Node.h"
#include <math.h>



Node::Node()
{
	value_ = SPACE;
	parent_ = nullptr;
	g_ = 0;
}


Node::~Node()
{
}

Node::Node(Point2D &pt, Point2D* t, int v, double g, Node* pr) {
	point_ = pt;
	target_ = t;
	value_ = v;
	parent_ = pr;
	this->g_ = g;
	h_ = compute_h();
}

void Node::set_value( int value)
{
	this->value_ = value;
}

void Node::set_point(Point2D point)
{
	this->point_ = point;
}

int Node::get_value()
{
	return value_;
}

double Node::get_g()
{
	return g_;
}



double Node::compute_h()
{
	return sqrt(pow(point_.get_row()-target_->get_row(),2)+
		pow(point_.get_col() - target_->get_col(), 2));
}

double Node::get_f()
{
	return g_+h_;
}

Point2D& Node::get_point()
{
	return point_;
}

Node * Node::get_parent()
{
	return parent_;
}

Point2D * Node::get_target()
{
	return target_;
}
