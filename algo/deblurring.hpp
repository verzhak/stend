
#ifndef DEBLURRING_HPP
#define DEBLURRING_HPP

#include "all.hpp"
#include "algo/algo.hpp"

// ############################################################################ 
// Модели шума

class c_noise
{
	public:

		virtual double operator()(const double, const double) = 0;
};

class c_gauss_noise : public c_noise
{
	const double sigma, sigma_2_2, pi_sigma_2_2;

	public:

		c_gauss_noise(const double);

		double operator()(const double, const double);
};

// ############################################################################ 
// Алгоритмы deblurring'а

class c_deblurring
{
	protected:

		const unsigned kernel_height, kernel_width;
		Mat kernel;

	public:

		c_deblurring(c_noise *, const unsigned, const unsigned);
		virtual Mat operator()(const Mat &) = 0;
};

class c_wiener : public c_deblurring
{
	public:

		c_wiener(c_noise *, const unsigned, const unsigned, const unsigned, const unsigned, const double);

		Mat operator()(const Mat &);
};

class c_richardson_lucy : public c_deblurring
{

	const unsigned max_iter;
	Mat kernel_flip;

	public:

		c_richardson_lucy(c_noise *, const unsigned, const unsigned, const unsigned);

		Mat operator()(const Mat &);

};

#endif

