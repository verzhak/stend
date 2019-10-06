
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "all.hpp"

class CVector;

class CRect
{
	Point_<double> pnt[4];

	public:

		CRect();
		CRect(const Point_<double> pnt_1, const Point_<double> pnt_2, const Point_<double> pnt_3, const Point_<double> pnt_4); 

		bool is_in(const CVector & vct) const;
};

class CVector
{
	bool is_pnt_init;
	double __x, __y;
	Point_<double> pnt[2];

	friend class CRect;

	bool line_coef(double & k, double & b) const;
	bool line_intersect(const CVector & vct, Point_<double> & ipnt) const;
	bool is_in(const Point_<double> & __pnt) const;
	double distance(const Point_<double> & pnt) const;

	public:

		CVector(const CVector & vct);
		CVector(const Point & pnt_1, const Point & pnt_2);
		CVector(const double x = 0, const double y = 0);
		CVector(const Point_<double> & vct);

		double norm() const								{ return sqrt((* this) * (* this)); };
		double operator*(const CVector & vct) const		{ return (__x * vct.__x + __y * vct.__y); };
		double operator^(const CVector & vct) const		{ return acos((* this) * vct / (norm() * vct.norm())); };

		void operator=(const CVector & vct);
		double distance(const CVector & vct) const;
		double distance_muratov(const CVector & vct) const;
		CRect rect(const double r) const;
};

#endif

