#pragma once

#ifndef PVECTOR2D_H
#define PVECTOR2D_H

namespace PSTD {
	namespace PMath {

		template <typename T>
		class PVector2d {
			public:

			union {
				struct {
					T X;
					T Y;
				};
				T Val[2];
			};

			PVector2d(void) { X = Y = 0; };
			PVector2d(float x, float y) { X = (T)x; Y = (T)y; };
			PVector2d(int x, int y) { X = (T)x; Y = (T)y; };
			PVector2d(double x, double y) { X = (T)x; Y = (T)y; };
			PVector2d(const PVector2d &p) { X = p.X; Y = p.Y; };


			PVector2d Add(T x, T y) { return PVector2d(X + x, Y + y); };
			T DistanceSquared(const PVector2d &v) { return ((X - v.X) * (X - v.X) + (Y - v.Y) * (Y - v.Y)); }

			/** @brief  Add two vectors together
			*  @param v Vector to add
			*  @return Resulting vector
			*/
			PVector2d operator+(const PVector2d &v) const { return PVector2d(X + v.X, Y + v.Y); };

			/** @brief Subtract a vector
			*  @param v Vector to subtract
			*  @return Resulting vector
			*/
			PVector2d operator- (const PVector2d &v) const { return PVector2d(X - v.X, Y - v.Y); };

			/** @brief Multiply the vector by a scaling factor
			*  @param scale Scaling factor
			*  @return Scaled vector
			*/
			PVector2d operator* (const T scale) const { return PVector2d(X * scale, Y * scale); };

			bool operator== (const PVector2d &v) const { return (IS_EPSILON_EQUAL(v.X, X) && IS_EPSILON_EQUAL(v.Y, Y)) };

			/** @brief Divide the vector by a scaling factor
			*  @param scale Scaling factor
			*  @return Scaled vector
			*/
			PVector2d operator/ (const T scale) const { return PVector2d(X / scale, Y / scale); };

			/** @brief Add and asign a vector to this one
			*  @param v Vector to add
			*  @return Resulting vector
			*/
			PVector2d &operator+= (const PVector2d &v) { X += v.X; Y += v.Y; return *this; };


			/** @brief Subtract and asign a vector to this one
			*  @param v Vector to subtract
			*  @return Resulting vector
			*/
			PVector2d &operator-= (const PVector2d &v) { X -= v.X; Y -= v.Y; return *this; };

			/** @brief multiply and assign this vector by a scaling factor
			*  @param scale Scaling factor
			*  @return Scaled vector
			*/
			PVector2d &operator*= (const T scale) { X *= scale; Y *= scale; return *this; };

			/** @brief Divide and assign this vector by a scaling factor
			*  @param scale Scaling factor
			*  @return Scaled vector
			*/
			PVector2d &operator/= (const T scale)  { X /= scale; Y /= scale; return *this; };

			
		};
	};
};

typedef PSTD::PMath::PVector2d<float> PVector2df;
typedef PSTD::PMath::PVector2d<int> PVector2di;


#endif
