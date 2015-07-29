/** \file matrix3x3.hpp
 *  \brief 3x3 Matrix class
 *
 * Class for storing and manipulating 3x3 matricies
 */

#pragma once

#ifndef MATRIX3x3_H
#define MATRIX3x3_H

#include <iostream>

#include "PVector3d.hpp"
#include "PQuaternion.hpp"


namespace PSTD {
   namespace PMath {

      template <typename T> class PVector3d;
      template <typename T> class PQuaternion;

      //!  3x3 Matrix
      template<typename T = float>
      class Matrix3x3 {
      public:

         //! Initialize matrix as all 0s
         Matrix3x3(void) :  e11(0.0f), e12(0.0f), e13(0.0f), e21(0.0f), e22(0.0f), e23(0.0f), e31(0.0f), e32(0.0f), e33(0.0f) { };

		 Matrix3x3(T v) : e11(v), e12(0.0f), e13(0.0f), e21(0.0f), e22(v), e23(0.0f), e31(0.0f), e32(0.0f), e33(v) { };

         //! Initialize matrix with given values
         Matrix3x3(T x1, T x2, T x3, T x4, T x5, T x6, T x7, T x8, T x9) :
            e11(x1), e12(x2), e13(x3), e21(x4), e22(x5), e23(x6),  e31(x7), e32(x8), e33(x9) { };

         //! Initialize matrix with the given matrix
         Matrix3x3(Matrix3x3 &m) { memcpy(Mat, m.Mat, sizeof(T) * 9); };

         //! Deconstructor
         ~Matrix3x3(void) {};

         /** Multiply two 3x3 matricies together
          * @param ma matrix to multiply by
          * @return Multiplied matrix
          */
         Matrix3x3 operator*(Matrix3x3 const &ma) {
            Matrix3x3 val;
            val.e11 = (ma.e11 * e11) + (ma.e12 * e21) + (ma.e13 * e31);
            val.e21 = (ma.e21 * e11) + (ma.e22 * e21) + (ma.e23 * e31);
            val.e31 = (ma.e31 * e11) + (ma.e32 * e21) + (ma.e33 * e31);
            val.e12 = (ma.e11 * e12) + (ma.e12 * e22) + (ma.e13 * e32);
            val.e22 = (ma.e21 * e12) + (ma.e22 * e22) + (ma.e23 * e32);
            val.e32 = (ma.e31 * e12) + (ma.e32 * e22) + (ma.e33 * e32);
            val.e13 = (ma.e11 * e13) + (ma.e12 * e23) + (ma.e13 * e33);
            val.e23 = (ma.e21 * e13) + (ma.e22 * e23) + (ma.e23 * e33);
            val.e33 = (ma.e31 * e13) + (ma.e32 * e23) + (ma.e33 * e33);
            return val;
         };
            
		 
		 Matrix3x3 InvTrans() {
			 Matrix3x3 m;
			 float determinant = e11 * (e22 * e33 - e32 * e23)
				 - e12 * (e21 * e33 - e23 * e31)
				 + e13 * (e21 * e32 - e22 * e31);
			 float invdet = 1 / determinant;
			 m.e11 = (e22 * e33 - e32 * e23) * invdet;
			 m.e21 = -(e12 * e33 - e13 * e32) * invdet;
			 m.e31 = (e12 * e23 - e13 * e22) * invdet;
			 m.e12 = -(e21 * e33 - e23 * e31) * invdet;
			 m.e22 = (e11 * e33 - e13 * e31) * invdet;
			 m.e32 = -(e11 * e23 - e21 * e13) * invdet;
			 m.e13 = (e21 * e32 - e31 * e22) * invdet;
			 m.e23 = -(e11 * e32 - e31 * e12) * invdet;
			 m.e33 = (e11 * e22 - e21 * e12) * invdet;
			 return m;
		 }
		 

         PVector3d<T> operator*(PVector3d<T> const &v);
		 PPoint3d<T> operator*(PPoint3d<T> const &v);
         PQuaternion<T> To_Quaternion(void);

         /** Get an element of the matrix
          * @param x horizontal location
          * @param y vertical location
          * @return element at that location in the matrix
          */
         T Element(int x, int y) { return Mat[x + y * 3]; }

         //! Make the matrix all 0s
         void Make_Zero(void) { memset(Mat, 0, 9 * sizeof(T)); };

         //! Make the matrix the identity matrix
         void Make_Identity(void) { Make_Zero(); e11 = e22 = e33 = 1.0; };
		
		 void Make_RotateUR(T f) {
			 e11 = cos(f);   e21 = sin(f); e31 = 0.0f;
			 e12 = -sin(f);  e22 = cos(f); e32 = 0.0f;
			 e13 = 0.0f;  e23 = 0.0f; e33 = 0.0f;
		 }

         //! Print out the matrix
         void Print(void) { 
            std::cout << e11 << ", " << e21 << ", " << e31 << std::endl;
            std::cout << e12 << ", " << e22 << ", " << e32 << std::endl;
            std::cout << e13 << ", " << e23 << ", " << e33 << std::endl;
         };

         //! Matrix in column major order accessible via array or individual matrix elements
         union {
            struct {
               T e11, e12, e13;            // opengl matrix is column major
               T e21, e22, e23;            // 
               T e31, e32, e33;
            };
            T Mat[9];
         };
      };

      
#include "PVector3d.hpp"
      /** Multiply the matrix by a vector
       * @param v vector to multiply by
       * @return Resulting vector
       */
      template <typename T> PVector3d<T> Matrix3x3<T>::operator*(PVector3d<T> const &v)  {
         PVector3d<T> val;
         return PVector3d<T>( v.X * e11 + v.Y * e21 + v.Z * e31, 
                              v.X * e12 + v.Y * e22 + v.Z * e32,
                              v.X * e13 + v.Y * e23 + v.Z * e33);
      };


	  template <typename T> PPoint3d<T> Matrix3x3<T>::operator*(PPoint3d<T> const &v)  {
		  PPoint3d<T> val;
		  return PPoint3d<T>(v.X * e11 + v.Y * e21 + v.Z * e31,
			  v.X * e12 + v.Y * e22 + v.Z * e32,
			  v.X * e13 + v.Y * e23 + v.Z * e33);
	  };

#include "PQuaternion.hpp"
      /** Convert the matrix to a quaternion
       * @return Resulting quaternion
       */
      template <typename T> PQuaternion<T> Matrix3x3<T>::To_Quaternion(void) {
         PQuaternion<T> quat;
         
         T trace = e11 + e22 + e33;
         if( trace > 0 ) {
            T s = 0.5f / sqrt(trace + 1.0f);
            quat.W = 0.25f / s;
            quat.X = ( e32 - e23 ) * s;
            quat.Y = ( e13 - e31 ) * s;
            quat.Z = ( e21 - e12 ) * s;
         } 
         else {
            if ( e11 > e22 && e11 > e33 ) {
               T s = 2.0f * sqrt( 1.0f + e11 - e22 - e33);
               quat.W = (e32 - e23 ) / s;
               quat.X = 0.25f * s;
               quat.Y = (e12 + e21 ) / s;
               quat.Z = (e13 + e31 ) / s;
            } 
            else if (e22 > e33) {
               T s = 2.0f * sqrt( 1.0f + e22 - e11 - e33);
               quat.W = (e13 - e31 ) / s;
               quat.X = (e12 + e21 ) / s;
               quat.Y = 0.25f * s;
               quat.Z = (e23 + e32 ) / s;
            } 
            else {
               T s = 2.0f * sqrt( 1.0f + e33 - e11 - e22 );
               quat.W = (e21 - e12 ) / s;
               quat.X = (e13 + e31 ) / s;
               quat.Y = (e23 + e32 ) / s;
               quat.Z = 0.25f * s;
            }
         }
         return quat;
      };


	  
   };
};

typedef PSTD::PMath::Matrix3x3<float> Matrix3x3f;
typedef PSTD::PMath::Matrix3x3<double> Matrix3x3d;

#endif
