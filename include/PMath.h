#pragma once

#ifndef PMATH_H
#define PMATH_H

#include <vector>
#include "pmath_constants.hpp"
#include "PVector3d.hpp"
#include "PVector4d.hpp"
#include "PMatrix3x3.hpp"
#include "PMatrix4x4.hpp"
#include "PQuaternion.hpp"
#include "PPoints.hpp"


// various mathmatics oriented functions

namespace PSTD {
	namespace PMath {


		/** \brief  Compute the greatest common divisor of two values
		 *
		 * Compute the greatest common divisor for two numbers U and V using the binary algorithm
		 * @param u U
		 * @param v V
		 * @return Greatest common divisor
		 */
		unsigned int GCD(unsigned int u, unsigned int v);


		/** \brief Compute the greatest common divisor of all values
		 *
		 * Compute the greatest common divisor for a vector of values using the binary algorithm
		 * @param val vector of values to compute the GCD of
		 * @return Greatest common divisor
		 */
		int GCD(std::vector<unsigned int> &val);


		/** \brief Compute the least common multiple for two values
		 *
		 * Compute the least common multiple for a and b
		 * @param a Number 1
		 * @param b Number 2
		 * @return Least common multiple
		 */
		int LCM(unsigned int a, unsigned int b);

		/** \brief Compute the least common multiple of all values
		 *
		 * Compute the least common multiple for all values in the given vector
		 * @param val Vector of values to compute the LCM of
		 * @return Least common multiple
		 */
		int LCM(std::vector<unsigned int> &val);


		// is x,y in the box defined by lower left point x1, y1 and width/height
		inline bool In_BoxDimension(int x, int y, int x1, int y1, int w, int h) {
			return ((x > x1) && (x < (x1 + w)) && (y > y1) && (y < (y1 + h)));
		}

		// is x,y in the box defined by lower left point x1, y1 and upper right point x2, y2
		inline bool In_Box(int x, int y, int x1, int y1, int x2, int y2) {
			return ((x > x1) && (x < x2) && (y > y1) && (y < y2));
		}


		// is x and y within the triangle defined by the 3 points x1,y1 and x2,y2, and x3,y3
		inline bool Is_PointInTriangle2d(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y) {
			float denominator = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
			float a = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3)) / denominator;
			float b = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3)) / denominator;
			float c = 1 - a - b;

			return (0 <= a) && (a <= 1) && (0 <= b) && (b <= 1) && (0 <= c) && (c <= 1);
		}

		inline bool Is_PointInTriangle2d(PVector2df &tp1, PVector2df &tp2, PVector2df &tp3, PVector2df &p) {
			return Is_PointInTriangle2d(tp1.X, tp1.Y, tp2.X, tp2.Y, tp3.X, tp3.Y, p.X, p.Y);
		}


		// Return an orthographic projection matrix with the given params
		template <class T> Matrix4x4<T> Make_Ortho(T left, T right, T bottom, T top, T nearC, T farC) {

			float aa = -(farC + nearC) / (farC - nearC);
			return Matrix4x4<T>(
				2.0f / (right - left), 0, 0, -(right + left) / (right - left),
				0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
				0.0f, 0.0f, -2.0f / (farC - nearC), -(farC + nearC) / (farC - nearC),
				0.0f, 0.0f, 0.0f, 1.0f);
		}


		// Return an perspective projection matrix with the given params
		template <class T> Matrix4x4<T> Make_Perspective(T fovy, T aspect, T zNear, T zFar) {
			float f = 1.0f / tan(fovy / 2.0f);
			return Matrix4x4<T>(
				f / aspect, 0, 0, 0,
				0, f, 0, 0,
				0, 0, (zFar + zNear) / (zNear - zFar), (2 * zFar * zNear) / (zNear - zFar),
				0.0f, 0.0f, -1.0f, 0.0f);
		}


		// Return a scaling matrix with the given parameters
		template <class T> Matrix4x4<T> Make_Scale(T x, T y, T z) {
			return Matrix4x4<T>(
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1);
		}

		// Return a scaling matrix with the given parameters
		template <class T> Matrix4x4<T> Make_Scale(T x) {
			return Matrix4x4<T>(
				x, 0, 0, 0,
				0, x, 0, 0,
				0, 0, x, 0,
				0, 0, 0, 1);
		}

		// Return a scaling matrix with the given parameters
		template <class T> Matrix4x4<T> Make_Scale(PVector3d<T> &v) {
			return Matrix4x4<T>(
				v.X, 0, 0, 0,
				0, v.Y, 0, 0,
				0, 0, v.Z, 0,
				0, 0, 0, 1);
		}

		// Return a translation matrix with the given parameters
		template <class T> Matrix4x4<T> Make_Translation(T x, T y, T z) {
			return Matrix4x4<T>(
				1, 0, 0, x,
				0, 1, 0, y,
				0, 0, 1, z,
				0, 0, 0, 1);
		}

		// Return a translation matrix with the given parameters
		template <class T> Matrix4x4<T> Make_Translation(PVector3d<T> v) {
			return Matrix4x4<T>(
				1, 0, 0, v.X,
				0, 1, 0, v.Y,
				0, 0, 1, v.Z,
				0, 0, 0, 1);
		}


		// Uniformly scale matrix toScale by amount 'scale'
		template <class T> void Scale(Matrix4x4<T> &toScale, T scale) {
			Matrix4x4<T> m(scale);
			m.e44 = 1;
			toScale = toScale * m;
		};


		// Scale matrix toScale by along the primary axis as paramterized
		template <class T> void Scale(Matrix4x4<T> &toScale, T scaleX, T scaleY, T scaleZ) {
			Matrix4x4<T> m = Make_Scale(scaleX, scaleY, scaleZ);
			toScale = toScale * m;
		};


		// Rotate matrix toRot by ang randians around the given axis
		template <class T> void RotateR(Matrix4x4<T> &toRot, T ang, T x, T y, T z) {
			Matrix4x4<T> mat = Make_RotationMatrixR(ang, x, y, z);
			toRot = toRot * mat;
		}


		// Translate matrix toTranslate by the given amounts 
		template <class T> void Translate(Matrix4x4<T> &toTranslate, T x, T y, T z) {
			toTranslate = toTranslate * Make_Translation(x, y, z);
		}

		// Return a rotation matrix corresponding to a ang radian rotation around axis <x, y, z>
		template <class T> Matrix4x4<T> Make_RotationMatrixR(T ang, T x, T y, T z) {
			Matrix4x4<T> mat;
			T c = cos(ang);
			T s = sin(ang);
			T omc = 1 - c;

			mat.e11 = x * x * omc + c;
			mat.e12 = y * x * omc + z * s;
			mat.e13 = x * z * omc - y * s;
			mat.e14 = 0;

			mat.e21 = x * y * omc - z * s;
			mat.e22 = y * y * omc + c;
			mat.e23 = y * z * omc + x * s;
			mat.e24 = 0;

			mat.e31 = x * z * omc + y * s;
			mat.e32 = y * z * omc - x * s;
			mat.e33 = z * z * omc + c;
			mat.e34 = 0;

			mat.e41 = 0;
			mat.e42 = 0;
			mat.e43 = 0;
			mat.e44 = 1;
			return mat;
		};


		// Return a rotation matrix corresponding to a ang degree rotation around axis <x, y, z>
		Matrix4x4<float> Make_RotationMatrixD(float ang, float x, float y, float z); 


		// Return a view matrix corresponding to a camera at eye, looking at center, with an up vector up
		template <class T> PSTD::PMath::Matrix4x4<T> Make_LookAt(PVector3d<T> eye, PVector3d<T> center, PVector3d<T> up) {
			Matrix4x4f m(1.0f);

			PVector3df forward = center - eye;
			forward.Normalize();

			PVector3df side = forward.Cross(up);
			side.Normalize();

			up = side.Cross(forward);

			m.e11 = side.X;
			m.e21 = side.Y;
			m.e31 = side.Z;

			m.e12 = up.X;
			m.e22 = up.Y;
			m.e32 = up.Z;

			m.e13 = -forward.X;
			m.e23 = -forward.Y;
			m.e33 = -forward.Z;

			Matrix4x4f tran(1.0f);
			tran.Set_Translation(-eye.X, -eye.Y, -eye.Z);
			return m * tran;
		};


		template <typename T> class PVector3d;

		// Determine world-space location (point) based on the projection matrix and pos
		bool Unproject(PVector3d<float> &pos, Matrix4x4<float> &modelProjection, int *viewport, PVector3d<float> &objCoord);

		// 4x4 matrix inversion
		void Invert_Matrix4x4(Matrix4x4<float> &src);
		void Invert_Matrix4x4(const Matrix4x4<float> &src, Matrix4x4<float> &dest);


	};

};

#endif

