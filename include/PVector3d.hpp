/** \file vector3d.hpp
 *  \brief 3-dimensional vector class
 *
 * Class for storing and manipulating 3-dimensional vectors
 */

#pragma once

#ifndef PVECTOR3D_H
#define PVECTOR3D_H

//#include <iostream>
#include <cmath>

#include "PPoints.hpp"
#include "PVector2d.hpp"
#include "PMath.h"
#include "PQuaternion.hpp"

namespace PSTD {

   namespace PMath {

      template <typename T> class Matrix3x3;

      //! 3-Dimensional Vector      
      template<typename T = float>
      class PVector3d {
      public:

         //! Default constructor initializing all components to 0
         PVector3d(void) { X = Y = Z = 0; };

         /** @brief Initialize vector with components
          *  @param x x
          *  @param y y 
          *  @param z z
          */
         PVector3d(T x, T y, T z)  { X = x; Y = y; Z = z; };

         /** @brief  Initialize vector as point 2 - point 1
          *  @param p1 point 1
          *  @param p2 point 2 
          */
         PVector3d(const PPoint3d<float> &p1, const PPoint3d<float> &p2) { X = (T)(p2.X - p1.X); Y = (T)(p2.Y - p1.Y); Z = (T)(p2.Z - p1.Z); };

         /** @brief  Initialize vector as point 2 - point 1
          *  @param p1 point 1
          *  @param p2 point 2 
          */
         PVector3d(const PPoint3d<double> &p1, const PPoint3d<double> &p2) { X = (T)(p2.X - p1.X); Y = (T)(p2.Y - p1.Y); Z = (T)(p2.Z - p1.Z); };

         /** @brief  Initialize vector as vector 2 - vector 1
          *  @param p1 vector 1
          *  @param p2 vector 2 
          */
         PVector3d(const PVector3d<float> &p1, const PVector3d<float> &p2) { X = p2.X - p1.X; Y = p2.Y - p1.Y; Z = p2.Z - p1.Z; };

         /** @brief  Initialize vector as vector 2 - vector 1
          *  @param p1 vector 1
          *  @param p2 vector 2 
          */
         PVector3d(const PVector3d<double> &p1, const PVector3d<double> &p2) { X = p2.X - p1.X; Y = p2.Y - p1.Y; Z = p2.Z - p1.Z; };

         //! Initialize vector as difference between given points with point components given individually
         /** @brief  Initialize vector as vector 2 - vector 1
          *  @param x1 point 1 x-coord
          *  @param y1 point 1 y-coord
          *  @param z1 point 1 z-coord
          *  @param x2 point 2 x-coord
          *  @param y2 point 2 y-coord
          *  @param z2 point 2 z-coord
          */
         PVector3d(T x1, T y1, T z1, T x2, T y2, T z2) { X = x1 - x2; Y = y1 - y2; Z = z1 - z2; };

         /** @brief  Copy constructor
          *  @param pv Vector to copy
          */
         PVector3d(const PVector3d &pv) { X = pv.X; Y = pv.Y; Z = pv.Z; };

         //! Deconstructor
         ~PVector3d(void) {};

         //! Print out the vector
         //void Print(void) { std::cout << X << "," << Y << "," << Z << '\n'; };

         /** @brief  Add two vectors together
          *  @param v Vector to add
          *  @return Resulting vector
          */
         PVector3d operator+ (const PVector3d &v) const { return PVector3d(X + v.X, Y + v.Y, Z + v.Z); };

         /** @brief Subtract a vector
          *  @param v Vector to subtract
          *  @return Resulting vector
          */
          PVector3d operator- (const PVector3d &v) const { return PVector3d(X - v.X, Y - v.Y, Z - v.Z); };

         /** @brief Multiply the vector by a scaling factor
          *  @param scale Scaling factor
          *  @return Scaled vector
          */
         PVector3d operator* (const T scale) const { return PVector3d(X * scale, Y * scale, Z * scale); };

		 bool operator== (const PVector3d &v) const { return (IS_EPSILON_EQUAL(v.X, X) && IS_EPSILON_EQUAL(v.Y, Y) && IS_EPSILON_EQUAL(v.Z, Z)); };

         /** @brief Divide the vector by a scaling factor
          *  @param scale Scaling factor
          *  @return Scaled vector
          */
          PVector3d operator/ (const T scale) const { return PVector3d(X / scale, Y / scale, Z / scale); };

         /** @brief Add and asign a vector to this one
          *  @param v Vector to add
          *  @return Resulting vector
          */
          PVector3d &operator+= (const PVector3d &v) { X += v.X; Y += v.Y; Z += v.Z; return *this; };


         /** @brief Subtract and asign a vector to this one
          *  @param v Vector to subtract
          *  @return Resulting vector
          */
          PVector3d &operator-= (const PVector3d &v) { X -= v.X; Y -= v.Y; Z -= v.Z; return *this; };

         /** @brief multiply and assign this vector by a scaling factor
          *  @param scale Scaling factor
          *  @return Scaled vector
          */
         PVector3d &operator*= (const T scale) { X *= scale; Y *= scale; Z *=scale; return *this; };

         /** @brief Divide and assign this vector by a scaling factor
          *  @param scale Scaling factor
          *  @return Scaled vector
          */
          PVector3d &operator/= (const T scale)  { X /= scale; Y /= scale; Z /=scale; return *this; };
  

		  //PVector3d<T> operator-(const PVector3d<T> &v) { return PVector3d<T>((v.X * -1), (v.Y * -1), (v.Z * -1)); };

         /** @brief  Compute length of vector
          *  @return Length of vector
          */
          T Length(void) const { return sqrt(X * X + Y * Y + Z * Z); };

         /** @brief  Compute squared length of vector
          *  @return Squared length of vector
          */
          T LengthSquared(void) const { return (X * X + Y * Y + Z * Z); };

         /** @brief  Compute length of vector using only first two components
          *  @return Length of 2d vector
          */
          T Length2d(void) const { return sqrt(X * X + Y * Y); };

         /** @brief  Compute squared length of vector using only first two components
          *  @return Squared length of 2d vector
          */
          T Length2dSquared(void) const { return (X * X + Y * Y); };

         //! Normalize the vector length
          void Normalize(void) { T len = Length(); X /= len; Y /= len; Z /= len; };

         /** @brief Return a normalized copy of this vector
          *  @return Normalized vector
          */
          PVector3d const Get_Normalized(void) { T len = Length(); return PVector3d(X / len , Y / len, Z / len); };

         /** @brief Compute the dot product
          *  @param v vector to take dot product of
          *  @return Dot product
          */
          T Dot(const PVector3d &v) const { return (X * v.X) + (Y * v.Y) + (Z * v.Z); };

         /** @brief Compute the cross product
          *  @param v vector to take cross product of
          *  @return Cross product
          */
          PVector3d Cross(const PVector3d &v) const {  return PVector3d(((Y * v.Z) - (Z * v.Y)), ((Z * v.X) - (X * v.Z)), ((X * v.Y) - (Y * v.X))); };


		  T DistanceSquared(const PVector3d &v) { return ((X - v.X) * (X - v.X) + (Y - v.Y) * (Y - v.Y) + (Z - v.Z) * (Z - v.Z)); }
		  T Distance(const PVector3d &v) { return sqrt((X - v.X) * (X - v.X) + (Y - v.Y) * (Y - v.Y) + (Z - v.Z) * (Z - v.Z)); }

         /** @brief Generate a 3x3 rotation matrix to rotate this vector to the given vector
          *  @param pv Vector to rotate this vector to
          *  @return 3x3 Rotation matrix to achieve desired orientation
          */
         Matrix3x3<T> Generate_VectorRotation3x3(const PVector3d &pv) const;

         /** @brief Generate a 4x4 rotation matrix to rotate this vector to the given vector
          *  @param pv Vector to rotate this vector to
          *  @return 4x4 Rotation matrix to achieve desired orientation
          */
         Matrix4x4<T> Generate_VectorRotation4x4(const PVector3d &pv) const;


		 PVector3d<T> Transform(const Matrix4x4<T> &m);

         /** @brief Generate a quaternion to rotate this vector to the given vector
          *  @param pv Vector to rotate this vector to
          *  @return Quaternion to achieve desired orientation
          */
         PQuaternion<T> Generate_VectorRotQuaternion(const PVector3d &pv) const;


		 PVector2d<T> Get_Vector2d(void) const { return PVector2d<T>(X, Y); };


         //! Vector components accessible via array index or coordinate name
         union {
            struct {
               T X;
               T Y;
               T Z;
            };
            T _Vec[3];
         };
      };
   

#include "PMatrix3x3.hpp"

      // generate a rotation to rotate vector from current location to pv
      template <typename T> Matrix3x3<T> PVector3d<T>::Generate_VectorRotation3x3(const PVector3d<T> &pv) const {
            Matrix3x3<T> res;
            T f;

            PVector3d<T> v = Cross(pv);
            T c = Dot(pv);
            T h = 1.0f / (1.0f + c); 
            
            if (c < 0) {
               f = -c;
            }
            else {
               f = c;
            }
            
            // handle the case where the two vectors are almost parallel
			if (f > 1.0 - PM_EPSILON)   {

               PVector3d<T> u, v;
               float x[3];       /* vector most nearly orthogonal to "from" */
               
               x[0] = (X > 0.0) ? X : -X;
               x[1] = (Y > 0.0) ? Y : -Y;
               x[2] = (Z > 0.0) ? Z : -Z;
               
               if (x[0] < x[1]) {
                  if (x[0] < x[2]) {
                     x[0] = 1.0; 
                     x[1] = x[2] = 0.0;
                  }
                  else {
                     x[2] = 1.0;
                     x[0] = x[1] = 0.0;
                  }
               }
               else {
                  if (x[1] < x[2]) {
                     x[1] = 1.0;
                     x[0] = x[2] = 0.0;
                  }
                  else {
                     x[2] = 1.0;
                     x[0] = x[1] = 0.0;
                  }
               }
               
               u.X = x[0] - X;
               u.Y = x[1] - Y;
               u.Z = x[2] - Z;
               v.X = x[0] - pv.X;
               v.Y = x[1] - pv.Y;
               v.Z = x[2] - pv.Z;
               
               T c1 = 2.0f / u.Dot(u);
               T c2 = 2.0f / v.Dot(v);
               T c3 = c1 * c2  * u.Dot(v);

               res.e11 = -c1 * u.X * u.X - c2 * v.X * v.X + c3 * v.X * u.X;
               res.e12 = -c1 * u.X * u.Y - c2 * v.X * v.Y + c3 * v.X * u.Y;
               res.e13 = -c1 * u.X * u.Z - c2 * v.X * v.Z + c3 * v.X * u.Z;
               res.e11 += 1.0f;
               res.e21 = -c1 * u.Y * u.X - c2 * v.Y * v.X + c3 * v.Y * u.X;
               res.e22 = -c1 * u.Y * u.Y - c2 * v.Y * v.Y + c3 * v.Y * u.Y;
               res.e23 = -c1 * u.Y * u.Z - c2 * v.Y * v.Z + c3 * v.Y * u.Z;
               res.e22 += 1.0f;
               res.e31 = -c1 * u.Z * u.X - c2 * v.Z * v.X + c3 * v.Z * u.X;
               res.e32 = -c1 * u.Z * u.Y - c2 * v.Z * v.Y + c3 * v.Z * u.Y;
               res.e33 = -c1 * u.Z * u.Z - c2 * v.Z * v.Z + c3 * v.Z * u.Z;
               res.e33 += 1.0f;       
            }

            // normal case
            else {
               res.e11 = c + h * v.X * v.X;
               res.e12 = h * v.X * v.Y - v.Z;
               res.e13 = h * v.X * v.Z + v.Y;
               
               res.e21 = h * v.X * v.Y + v.Z;
               res.e22 = c + h * v.Y * v.Y;
               res.e23 = h * v.Y * v.Z - v.X;
               
               res.e31 = h * v.X * v.Z - v.Y;
               res.e32 = h * v.Y * v.Z + v.X;
               res.e33 = c + h * v.Z * v.Z;
            }
            return res;
         };
   

#include "PMatrix4x4.hpp"

	  template<typename T> PVector3d<T> PVector3d<T>::Transform(const Matrix4x4<T> &m) {
			  PVector3d<T> v2;
			  v2.X = (X * m.e11) + (Y * m.e21) + (Z * m.e31) + m.e41;
			  v2.Y = (X * m.e12) + (Y * m.e22) + (Z * m.e32) + m.e42;
			  v2.Z = (X * m.e13) + (Y * m.e23) + (Z * m.e33) + m.e43;
			  return v2;
	  }


      template<typename T> Matrix4x4<T> PVector3d<T>::Generate_VectorRotation4x4(const PVector3d<T> &pv) const {
         Matrix4x4<T> res;
         T f;

         res.Make_Identity();
         PVector3d<T> v = Cross(pv);
         T c = Dot(pv);
         T h = 1.0f / (1.0f + c); 

         if (c < 0) {
            f = -c;
         }
         else {
            f = c;
         }
         
         // handle the case where the two vectors are almost parallel
         if (f > 1.0 - PM_EPSILON)   {
            PVector3d<T> u, v;
            T x[3];       /* vector most nearly orthogonal to "from" */
            
            x[0] = (X > 0.0) ? X : -X;
            x[1] = (Y > 0.0) ? Y : -Y;
            x[2] = (Z > 0.0) ? Z : -Z;
            
            if (x[0] < x[1]) {
               if (x[0] < x[2]) {
                  x[0] = 1.0; 
                  x[1] = x[2] = 0.0;
               }
               else {
                  x[2] = 1.0;
                  x[0] = x[1] = 0.0;
               }
            }
            else {
               if (x[1] < x[2]) {
                  x[1] = 1.0;
                  x[0] = x[2] = 0.0;
               }
               else {
                  x[2] = 1.0;
                  x[0] = x[1] = 0.0;
               }
            }
            
            u.X = x[0] - X;
            u.Y = x[1] - Y;
            u.Z = x[2] - Z;
            v.X = x[0] - pv.X;
            v.Y = x[1] - pv.Y;
            v.Z = x[2] - pv.Z;
            
            T c1 = 2.0f / u.Dot(u);
            T c2 = 2.0f / v.Dot(v);
            T c3 = c1 * c2  * u.Dot(v);

            res.e11 = -c1 * u.X * u.X - c2 * v.X * v.X + c3 * v.X * u.X;
            res.e12 = -c1 * u.X * u.Y - c2 * v.X * v.Y + c3 * v.X * u.Y;
            res.e13 = -c1 * u.X * u.Z - c2 * v.X * v.Z + c3 * v.X * u.Z;
            res.e11 += 1.0f;
            res.e21 = -c1 * u.Y * u.X - c2 * v.Y * v.X + c3 * v.Y * u.X;
            res.e22 = -c1 * u.Y * u.Y - c2 * v.Y * v.Y + c3 * v.Y * u.Y;
            res.e23 = -c1 * u.Y * u.Z - c2 * v.Y * v.Z + c3 * v.Y * u.Z;
            res.e22 += 1.0f;
            res.e31 = -c1 * u.Z * u.X - c2 * v.Z * v.X + c3 * v.Z * u.X;
            res.e32 = -c1 * u.Z * u.Y - c2 * v.Z * v.Y + c3 * v.Z * u.Y;
            res.e33 = -c1 * u.Z * u.Z - c2 * v.Z * v.Z + c3 * v.Z * u.Z;
            res.e33 += 1.0f;
            
         }
         
         // normal case
         else {
            res.e11 = c + h * v.X * v.X;
            res.e12 = h * v.X * v.Y - v.Z;
            res.e13 = h * v.X * v.Z + v.Y;
            
            res.e21 = h * v.X * v.Y + v.Z;
            res.e22 = c + h * v.Y * v.Y;
            res.e23 = h * v.Y * v.Z - v.X;
            
            res.e31 = h * v.X * v.Z - v.Y;
            res.e32 = h * v.Y * v.Z + v.X;
            res.e33 = c + h * v.Z * v.Z;
         }
         
         return res;
      }

#include "PQuaternion.hpp"

      template <typename T> PQuaternion<T> PVector3d<T>::Generate_VectorRotQuaternion(const PVector3d<T> &pv) const {
            Matrix3x3<T> res;
            T f;

            PVector3d<T> v = Cross(pv);
            T c = Dot(pv);
            T h = 1.0f / (1.0f + c); 

            if (c < 0) {
               f = -c;
            }
            else {
               f = c;
            }
            
            // handle the case where the two vectors are almost parallel
			if (f > 1.0 - PM_EPSILON)   {
               PVector3d<T> u, v;
               T x[3];       /* vector most nearly orthogonal to "from" */

               x[0] = (X > 0.0) ? X : -X;
               x[1] = (Y > 0.0) ? Y : -Y;
               x[2] = (Z > 0.0) ? Z : -Z;
               
               if (x[0] < x[1]) {
                  if (x[0] < x[2]) {
                     x[0] = 1.0; 
                     x[1] = x[2] = 0.0;
                  }
                  else {
                     x[2] = 1.0;
                     x[0] = x[1] = 0.0;
                  }
               }
               else {
                  if (x[1] < x[2]) {
                     x[1] = 1.0;
                     x[0] = x[2] = 0.0;
                  }
                  else {
                     x[2] = 1.0;
                     x[0] = x[1] = 0.0;
                  }
               }
               
               u.X = x[0] - X;
               u.Y = x[1] - Y;
               u.Z = x[2] - Z;
               v.X = x[0] - pv.X;
               v.Y = x[1] - pv.Y;
               v.Z = x[2] - pv.Z;
               
               float c1 = 2.0f / u.Dot(u);
               float c2 = 2.0f / v.Dot(v);
               float c3 = c1 * c2  * u.Dot(v);
               
               res.e11 = -c1 * u.X * u.X - c2 * v.X * v.X + c3 * v.X * u.X;
               res.e12 = -c1 * u.X * u.Y - c2 * v.X * v.Y + c3 * v.X * u.Y;
               res.e13 = -c1 * u.X * u.Z - c2 * v.X * v.Z + c3 * v.X * u.Z;
               res.e11 += 1.0f;
               res.e21 = -c1 * u.Y * u.X - c2 * v.Y * v.X + c3 * v.Y * u.X;
               res.e22 = -c1 * u.Y * u.Y - c2 * v.Y * v.Y + c3 * v.Y * u.Y;
               res.e23 = -c1 * u.Y * u.Z - c2 * v.Y * v.Z + c3 * v.Y * u.Z;
               res.e22 += 1.0f;
               res.e31 = -c1 * u.Z * u.X - c2 * v.Z * v.X + c3 * v.Z * u.X;
               res.e32 = -c1 * u.Z * u.Y - c2 * v.Z * v.Y + c3 * v.Z * u.Y;
               res.e33 = -c1 * u.Z * u.Z - c2 * v.Z * v.Z + c3 * v.Z * u.Z;
               res.e33 += 1.0f;
            }

            // normal case
            else {
               res.e11 = c + h * v.X * v.X;
               res.e12 = h * v.X * v.Y - v.Z;
               res.e13 = h * v.X * v.Z + v.Y;
               
               res.e21 = h * v.X * v.Y + v.Z;
               res.e22 = c + h * v.Y * v.Y;
               res.e23 = h * v.Y * v.Z - v.X;
               
               res.e31 = h * v.X * v.Z - v.Y;
               res.e32 = h * v.Y * v.Z + v.X;
               res.e33 = c + h * v.Z * v.Z;
            }
            return res.To_Quaternion();
         };  
   };
};





typedef PSTD::PMath::PVector3d<float> PVector3df;
typedef PSTD::PMath::PVector3d<double> PVector3dd;
typedef PSTD::PMath::PVector3d<int> PVector3di;

const PVector3df X_AXIS_VECTOR(1.0f, 0.0f, 0.0f);
const PVector3df Y_AXIS_VECTOR(0.0f, 1.0f, 0.0f);
const PVector3df Z_AXIS_VECTOR(0.0f, 0.0f, 1.0f);



#endif
