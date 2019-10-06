
#include "algo/vector.hpp"

CRect::CRect()
{
	for(auto & __pnt : pnt)
		__pnt = Point_<double>(0, 0);
}

CRect::CRect(const Point_<double> pnt_1, const Point_<double> pnt_2, const Point_<double> pnt_3, const Point_<double> pnt_4)
{
	pnt[0] = pnt_1;
	pnt[1] = pnt_2;
	pnt[2] = pnt_3;
	pnt[3] = pnt_4;
}

bool CRect::is_in(const CVector & vct) const
{
	// TODO
	
	return true;
}

// ############################################################################ 

CVector::CVector(const CVector & vct)
{
	pnt[0] = vct.pnt[0];
	pnt[1] = vct.pnt[1];
	
	__x = vct.__x;
	__y = vct.__y;

	is_pnt_init = true;
}

CVector::CVector(const Point & pnt_1, const Point & pnt_2)
{
	pnt[0] = pnt_1;
	pnt[1] = pnt_2;
	
	__x = pnt[0].x - pnt[1].x;
	__y = pnt[0].y - pnt[1].y;

	is_pnt_init = true;
}

CVector::CVector(const double x, const double y)
{
	__x = x;
	__y = y;

	is_pnt_init = false;
}

CVector::CVector(const Point_<double> & vct)
{
	__x = vct.x;
	__y = vct.y;

	is_pnt_init = false;
}

void CVector::operator=(const CVector & vct)
{
	__x = vct.__x;
	__y = vct.__y;

	if((is_pnt_init = vct.is_pnt_init))
	{
		pnt[0] = vct.pnt[0];
		pnt[1] = vct.pnt[1];
	}
}

// ############################################################################ 

bool CVector::line_coef(double & k, double & b) const
{
	if(pnt[0].x != pnt[1].x)
	{
		k = (pnt[1].y - pnt[0].y) / (pnt[1].x - pnt[0].x);
		b = pnt[0].y - k * pnt[0].x;

		return true;
	}

	return false;
}

bool CVector::line_intersect(const CVector & vct, Point_<double> & ipnt) const
{
	double k_1, b_1, k_2, b_2;
	const bool is_1 = line_coef(k_1, b_1);
	const bool is_2 = vct.line_coef(k_2, b_2);

	if(is_1)
	{
		if(is_2)
		{
			const double d_k = k_1 - k_2;

			if(d_k == 0)
				return false; // Прямые совпадают => отрезки пересекаются на всем своем протяжении

			ipnt.x = (b_2 - b_1) / d_k;
		}
		else
			ipnt.x = vct.pnt[0].x; // Прямая 2 параллельна OY

		ipnt.y = k_1 * ipnt.x + b_1;

		return true;
	}
	
	ipnt.x = pnt[0].x; // Прямая 1 параллельна OY

	if(is_2)
	{
		ipnt.y = k_2 * ipnt.x + b_1;

		return true;
	}
		
	// Прямая 2 параллельна OY

	return false;
}

bool CVector::is_in(const Point_<double> & __pnt) const
{
	// TODO Костыль

	const bool in_x =
		(__pnt.x >= pnt[0].x && __pnt.x <= pnt[1].x) || (__pnt.x >= pnt[1].x && __pnt.x <= pnt[0].x);
	const bool in_y =
		(__pnt.y >= pnt[0].y && __pnt.y <= pnt[1].y) || (__pnt.y >= pnt[1].y && __pnt.y <= pnt[0].y);

	return in_x && in_y;
}
	
double CVector::distance(const Point_<double> & __pnt) const
{
	const CVector b(pnt[0] - __pnt), c(pnt[1] - __pnt);
	const double alpha = (* this) ^ b, beta = (* this) ^ c;
	double d;

	if(alpha >= M_PI_2)
		d = b.norm();
	else if(beta >= M_PI_2)
		d = c.norm();
	else
		d = norm() / (1. / tan(alpha) + 1. / tan(beta));

	return d;
}

double CVector::distance(const CVector & vct) const
{
	throw_if(! is_pnt_init, "TODO");
	throw_if(! vct.is_pnt_init, "TODO");

	Point_<double> ipnt;

	if(line_intersect(vct, ipnt))
		if(
			is_in(ipnt)
			&&
			vct.is_in(ipnt)
		  )
			return 0; // Отрезки пересекаются

	return min(
		vct.distance(pnt[0]),
		vct.distance(pnt[1]));
}

double CVector::distance_muratov(const CVector & vct) const
{
	const bool this_large_then_vct = norm() > vct.norm();
	const CVector & big = this_large_then_vct ? * this : vct;
	const CVector & small = this_large_then_vct ? vct : * this;

	// ############################################################################ 
	// Угол поворота
	
	const double dx = big.pnt[0].x - big.pnt[1].x;
	const double alpha =
		dx == 0
		?
			M_PI_2
			:
			atan((big.pnt[0].y - big.pnt[1].y) / dx);

	// ############################################################################ 
	// Поворот

	const double sin_alpha = sin(alpha), cos_alpha = cos(alpha);
	auto rotate = [ & sin_alpha, & cos_alpha ](const Point_<double> & pnt)
	{
		Point_<double> res;

		res.x = cos_alpha * pnt.x + sin_alpha * pnt.y;
		res.y = - sin_alpha * pnt.x + cos_alpha * pnt.y;

		return res;
	};
	const CVector small_rotate(rotate(small.pnt[0]), rotate(small.pnt[1]));
	const CVector big_rotate(rotate(big.pnt[0]), rotate(big.pnt[1]));

	// ############################################################################ 
	// Упорядочивание точек
	
	auto sort = [](const Point_<double> pnt[2], Point_<double> * res)
	{
		if(pnt[0].x < pnt[1].x)
		{
			res[0] = pnt[0];
			res[1] = pnt[1];
		}
		else
		{
			res[0] = pnt[1];
			res[1] = pnt[0];
		}
	};
	Point_<double> small_pnt[2];
	Point_<double> big_pnt[2];

	sort(small_rotate.pnt, small_pnt);
	sort(big_rotate.pnt, big_pnt);

	// ############################################################################ 
	// Поиск центра в отрезке с упорядоченными точками
	
	const Point_<double> small_center(
		(small_pnt[0].x + small_pnt[1].x) / 2,
		(small_pnt[0].y + small_pnt[1].y) / 2);

	// ############################################################################ 
	// Проецирование
	
	const double small_norm_2 = small.norm() / 2;
	Point_<double> big_center;

	big_center.y = 0;

	if(small_pnt[0].x < big_pnt[0].x)
		// small левее big
		big_center.x = big_pnt[0].x + small_norm_2;
	else if(small_pnt[1].x > big_pnt[1].x)
		// small правее big
		big_center.x = big_pnt[1].x - small_norm_2;
	else
		// small в big
		big_center.x = (small_pnt[0].x + small_pnt[1].x) / 2;

	// ############################################################################ 
	// Расчет расстояния
	
	const double rdx = small_center.x - big_center.x;
	const double rdy = small_center.y - big_center.y;

	return sqrt(rdx * rdx + rdy * rdy);
}

CRect CVector::rect(const double r) const
{
	throw_if(! is_pnt_init, "TODO");

	const double r_2 = r / 2;
	Point_<double> left, right;

	if(pnt[0].x < pnt[1].x)
	{
		left = pnt[0];
		right = pnt[1];
	}
	else
	{
		left = pnt[1];
		right = pnt[0];
	}

	return CRect
	(
		Point_<double>(left.x - r_2, left.y - r_2),
		Point_<double>(left.x - r_2, left.y + r_2),
		Point_<double>(right.x + r_2, right.y - r_2),
		Point_<double>(right.x + r_2, right.y + r_2)
	);
}

