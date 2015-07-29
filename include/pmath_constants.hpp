/** \file pmath_constants.hpp
 *  \brief Mathmatical constants
 *
 * Collection of mathmatical constants automatically cast to the proper type via template parameters: e.g.  PSTD:PMath::constants::PI<double>()
 */




#ifndef PMATH_CONSTANTS_HPP
#define PMATH_CONSTANTS_HPP

//! Patrick's Standard Library
namespace PSTD {

	//! PSTD mathmatics functionality
	namespace PMath {


//#ifndef SKIP_DOXYGEN

#define PM_EPSILON         0.000001
#define PM_PI              3.1415926535897932384626433832795029L
#define PM_PI_2            1.5707963267948966192313216916397514L
#define PM_PI_4        	   0.7853981633974483096156608458198757L 
#define PM_RAD2DEG         (180.0L / PM_PI)
#define PM_DEG2RAD         (PM_PI / 180.0L)

#define PM_PIf				3.1415926535897932384626433832795029f
#define PM_RAD2DEGf         (180.0f / PM_PIf)
#define PM_DEG2RADf         (PM_PIf / 180.0f)


#define IS_EPSILON_EQUAL(x, y)  (fabs(x - y) < PM_EPSILON)



		//! Epsilon
		template <class T = float>  inline T EPSILON() { static const T res = (T)PM_EPSILON; return res; };

		//! PI 
		template <class T = float>  inline T PI() { static const T res = (T)PM_PI; return res; };

		//! PI / 2
		template <class T = float>  inline T PI_2() { static const T res = (T)PM_PI_2; return res; };

		//! PI / 4
		template <class T = float>  inline T PI_4() { static const T res = (T)PM_PI_4; return res; };

		//! Constant for converting radians to degrees
		template <class T = float>  inline T RAD_TO_DEG() { static const T res = (T)PM_RAD2DEG; return res; };

		//! Constant for converting degrees to radians
		template <class T = float>  inline T DEG_TO_RAD() { static const T res = (T)PM_DEG2RAD; return res; };


		inline bool NearZero(float x) { return (fabs(x) < 0.000001f); }
		inline float ClampDeg(float x) { while (x > 360.0f) x -= 360.0f; while (x < 0.0f) x += 360.0f; return x; }
		inline float ClampRad(float x) { while (x > 2.0f * PI<float>()) x -= (2.0f * PI<float>()); while (x < 0.0f) x += (2.0f * PI<float>()); return x; }
		inline double ClampDeg(double x) { while (x > 360.0) x -= 360.0; while (x < 0.0f) x += 360.0; return x; }
		inline double ClampRad(double x) { while (x > 2 * PI<double>()) x -= (2 * PI<double>()); while (x < 0.0) x += (2 * PI<double>()); return x; }
		inline float ClampDeg180(float x) { while (x > 180.0f) x -= 360.0f; while (x <= -180.0f) x += 360.0f; return x; }
		inline float ClampRad180(float x) { while (x > PI<float>()) x -= (2.0f * PI<float>()); while (x <= -PI<float>()) x += (2.0f * PI<float>()); return x; }
		inline double ClampDeg180(double x) { while (x > 180.0) x -= 360.0; while (x <= -180.0) x += 360.0; return x; }
		inline double ClampRad180(double x) { while (x > PI<double>()) x -= (2.0 * PI<double>()); while (x <= -PI<double>()) x += (2.0 * PI<double>()); return x; }
	};
};


#endif
