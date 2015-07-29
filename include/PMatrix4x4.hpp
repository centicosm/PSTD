#pragma once

#ifndef MATRIXFOURXFOUR_H
#define MATRIXFOURXFOUR_H

#include <xmmintrin.h>
#include "PVector3d.hpp"
#include "PVector4d.hpp"
#include "PQuaternion.hpp"
#include "PMatrix3x3.hpp"


namespace PSTD {
	namespace PMath {

		template <typename T> class PVector3d;
		template <typename T> class PQuaternion;
		template <typename T> class Matrix3x3;

		//!  4x4 Matrix
		template <typename T = float>
		class Matrix4x4 {
		public:

			//! Initialize matrix as identity
			Matrix4x4(void) : e11(1.0f), e12(0), e13(0), e14(0),
				e21(0), e22(1.0f), e23(0), e24(0),
				e31(0), e32(0), e33(1.0f), e34(0),
				e41(0), e42(0), e43(0), e44(1.0f)
			{
				// nothing 
			};

			//! Initialize matrix with the given diagonekas all 0s
			Matrix4x4(T v) : e11(v), e12(0), e13(0), e14(0),
				e21(0), e22(v), e23(0), e24(0),
				e31(0), e32(0), e33(v), e34(0),
				e41(0), e42(0), e43(0), e44(v)
			{
				// nothing 
			};



			Matrix4x4(T x1, T x2, T x3, T x4, T x5, T x6, T x7, T x8, T x9, T x10, T x11, T x12, T x13, T x14, T x15, T x16) :
			e11(x1), e12(x5), e13(x9), e14(x13),
				e21(x2), e22(x6), e23(x10), e24(x14),
				e31(x3), e32(x7), e33(x11), e34(x15),
				e41(x4), e42(x8), e43(x12), e44(x16)
			{
				// nothing
			};




			//! Copy constructor 
			Matrix4x4(const Matrix4x4<T> &m) { memcpy(Mat, m.Mat, sizeof(T)* 16); };


			//! Deconstructor
			~Matrix4x4(void) {};

			

			inline void Multiply(const PVector4df &v, PVector4df &out) {
#ifdef __SSE_AVAIL__

				__m128 v1 = _mm_load_ps(v._Vec);
				__m128 m0 = _mm_load_ps(Row1);
				__m128 m1 = _mm_load_ps(Row2);
				__m128 m2 = _mm_load_ps(Row3);
				__m128 m3 = _mm_load_ps(Row4);

				m0 = _mm_mul_ps(m0, v1);   //(e11 * v.X) , (e21 * v.Y), (e31 * v.Z), (e41 * v.W)
				m1 = _mm_mul_ps(m1, v1);	//(e12 * v.X) , (e22 * v.Y), (e32 * v.Z), (e42 * v.W)
				m2 = _mm_mul_ps(m2, v1);	//(e13 * v.X) , (e23 * v.Y), (e33 * v.Z), (e43 * v.W)
				m3 = _mm_mul_ps(m3, v1);	//(e14 * v.X) , (e24 * v.Y), (e34 * v.Z), (e44 * v.W)

				m0 = _mm_hadd_ps(m0, m1);
				m0 = _mm_hadd_ps(m0, m0);
				m2 = _mm_hadd_ps(m2, m3);
				m2 = _mm_hadd_ps(m2, m2);
				_mm_store_ps(out._Vec, _mm_movehl_ps(m2, m0));
#else
				out.X = v.X * e11 + v.Y * e21 + v.Z * e31 + v.W * e41;
				out.Y = v.X * e12 + v.Y * e22 + v.Z * e32 + v.W * e42;
				out.Z = v.X * e13 + v.Y * e23 + v.Z * e33 + v.W * e43;
				out.W = v.X * e14 + v.Y * e24 + v.Z * e34 + v.W * e44;
#endif
			}


			inline PVector4df operator*(const PVector4df &v) {
		
#ifdef __SSE_AVAIL__

				__m128 v1 = _mm_load_ps(v._Vec);
				__m128 m0 = _mm_load_ps(Row1);
				__m128 m1 = _mm_load_ps(Row2);
				__m128 m2 = _mm_load_ps(Row3);
				__m128 m3 = _mm_load_ps(Row4);
				 m0 = _mm_mul_ps(m0, v1);   //(e11 * v.X) , (e21 * v.Y), (e31 * v.Z), (e41 * v.W)
				 m1 = _mm_mul_ps(m1, v1);	//(e12 * v.X) , (e22 * v.Y), (e32 * v.Z), (e42 * v.W)
				 m2 = _mm_mul_ps(m2, v1);	//(e13 * v.X) , (e23 * v.Y), (e33 * v.Z), (e43 * v.W)
				 m3 = _mm_mul_ps(m3, v1);	//(e14 * v.X) , (e24 * v.Y), (e34 * v.Z), (e44 * v.W)

				m0 = _mm_hadd_ps(m0, m1);
				m0 = _mm_hadd_ps(m0, m0);
				m2 = _mm_hadd_ps(m2, m3);
				m2 = _mm_hadd_ps(m2, m2);
				m0 = _mm_movehl_ps(m2, m0);

				PVector4df val;
				_mm_store_ps(val._Vec, m0);
				return val;
#else

				return PVector4df(v.X * e11 + v.Y * e21 + v.Z * e31 + v.W * e41,
					v.X * e12 + v.Y * e22 + v.Z * e32 + v.W * e42,
					v.X * e13 + v.Y * e23 + v.Z * e33 + v.W * e43,
					v.X * e14 + v.Y * e24 + v.Z * e34 + v.W * e44);
#endif
			}


			inline void Multiply(Matrix4x4<float> const &ma, Matrix4x4<float> &out) const {

#ifdef __SSE_AVAIL__
				__m128 m0 = _mm_load_ps(Row1);
				__m128 m1 = _mm_load_ps(Row2);
				__m128 m2 = _mm_load_ps(Row3);
				__m128 m3 = _mm_load_ps(Row4);

				__m128 brod1 = _mm_set1_ps(ma.Mat[0]);
				__m128 brod2 = _mm_set1_ps(ma.Mat[1]);
				__m128 brod3 = _mm_set1_ps(ma.Mat[2]);
				__m128 brod4 = _mm_set1_ps(ma.Mat[3]);
				__m128 row = _mm_add_ps(
					_mm_add_ps(
					_mm_mul_ps(brod1, m0),
					_mm_mul_ps(brod2, m1)),
					_mm_add_ps(
					_mm_mul_ps(brod3, m2),
					_mm_mul_ps(brod4, m3)));
					//out.ssem1 = row;
				_mm_store_ps(out.Row1, row);

				brod1 = _mm_set1_ps(ma.Mat[4]);
				brod2 = _mm_set1_ps(ma.Mat[5]);
				brod3 = _mm_set1_ps(ma.Mat[6]);
				brod4 = _mm_set1_ps(ma.Mat[7]);
				row = _mm_add_ps(
					_mm_add_ps(
					_mm_mul_ps(brod1, m0),
					_mm_mul_ps(brod2, m1)),
					_mm_add_ps(
					_mm_mul_ps(brod3, m2),
					_mm_mul_ps(brod4, m3)));
				_mm_store_ps(out.Row2, row);
				//out.ssem2 = row;

				brod1 = _mm_set1_ps(ma.Mat[8]);
				brod2 = _mm_set1_ps(ma.Mat[9]);
				brod3 = _mm_set1_ps(ma.Mat[10]);
				brod4 = _mm_set1_ps(ma.Mat[11]);
				row = _mm_add_ps(
					_mm_add_ps(
					_mm_mul_ps(brod1, m0),
					_mm_mul_ps(brod2, m1)),
					_mm_add_ps(
					_mm_mul_ps(brod3, m2),
					_mm_mul_ps(brod4, m3)));
				_mm_store_ps(out.Row3, row);
				//out.ssem3 = row;

				brod1 = _mm_set1_ps(ma.Mat[12]);
				brod2 = _mm_set1_ps(ma.Mat[13]);
				brod3 = _mm_set1_ps(ma.Mat[14]);
				brod4 = _mm_set1_ps(ma.Mat[15]);
				row = _mm_add_ps(
					_mm_add_ps(
					_mm_mul_ps(brod1, m0),
					_mm_mul_ps(brod2, m1)),
					_mm_add_ps(
					_mm_mul_ps(brod3, m2),
					_mm_mul_ps(brod4, m3)));
				_mm_store_ps(out.Row4, row);
				//out.ssem4 = row;
#else

#endif

			}

			inline Matrix4x4<float> operator*(Matrix4x4<float> const &ma) const {
				Matrix4x4<float> val;

#ifdef __SSE_AVAIL__
				__m128 m0 = _mm_load_ps(Row1);
				__m128 m1 = _mm_load_ps(Row2);
				__m128 m2 = _mm_load_ps(Row3);
				__m128 m3 = _mm_load_ps(Row4);


				__m128 brod1 = _mm_set1_ps(ma.Mat[0]);
				__m128 brod2 = _mm_set1_ps(ma.Mat[1]);
				__m128 brod3 = _mm_set1_ps(ma.Mat[2]);
				__m128 brod4 = _mm_set1_ps(ma.Mat[3]);
				__m128 row = _mm_add_ps(
					_mm_add_ps(
					_mm_mul_ps(brod1, m0),
					_mm_mul_ps(brod2, m1)),
					_mm_add_ps(
					_mm_mul_ps(brod3, m2),
					_mm_mul_ps(brod4, m3)));
				_mm_store_ps(val.Row1, row);

				brod1 = _mm_set1_ps(ma.Mat[4]);
				brod2 = _mm_set1_ps(ma.Mat[5]);
				brod3 = _mm_set1_ps(ma.Mat[6]);
				brod4 = _mm_set1_ps(ma.Mat[7]);
				row = _mm_add_ps(
					_mm_add_ps(
					_mm_mul_ps(brod1, m0),
					_mm_mul_ps(brod2, m1)),
					_mm_add_ps(
					_mm_mul_ps(brod3, m2),
					_mm_mul_ps(brod4, m3)));
				_mm_store_ps(val.Row2, row);

				brod1 = _mm_set1_ps(ma.Mat[8]);
				brod2 = _mm_set1_ps(ma.Mat[9]);
				brod3 = _mm_set1_ps(ma.Mat[10]);
				brod4 = _mm_set1_ps(ma.Mat[11]);
				row = _mm_add_ps(
					_mm_add_ps(
					_mm_mul_ps(brod1, m0),
					_mm_mul_ps(brod2, m1)),
					_mm_add_ps(
					_mm_mul_ps(brod3, m2),
					_mm_mul_ps(brod4, m3)));
				_mm_store_ps(val.Row3, row);

				brod1 = _mm_set1_ps(ma.Mat[12]);
				brod2 = _mm_set1_ps(ma.Mat[13]);
				brod3 = _mm_set1_ps(ma.Mat[14]);
				brod4 = _mm_set1_ps(ma.Mat[15]);
				row = _mm_add_ps(
					_mm_add_ps(
					_mm_mul_ps(brod1, m0),
					_mm_mul_ps(brod2, m1)),
					_mm_add_ps(
					_mm_mul_ps(brod3, m2),
					_mm_mul_ps(brod4, m3)));
				_mm_store_ps(val.Row4, row);

				
#else

#endif

				return val;
			}



			bool Invert(Matrix4x4<T> &invOut) const	{
				T inv[16], det;
				int i;

				inv[0] = Mat[5] * Mat[10] * Mat[15] -
					Mat[5] * Mat[11] * Mat[14] -
					Mat[9] * Mat[6] * Mat[15] +
					Mat[9] * Mat[7] * Mat[14] +
					Mat[13] * Mat[6] * Mat[11] -
					Mat[13] * Mat[7] * Mat[10];

				inv[4] = -Mat[4] * Mat[10] * Mat[15] +
					Mat[4] * Mat[11] * Mat[14] +
					Mat[8] * Mat[6] * Mat[15] -
					Mat[8] * Mat[7] * Mat[14] -
					Mat[12] * Mat[6] * Mat[11] +
					Mat[12] * Mat[7] * Mat[10];

				inv[8] = Mat[4] * Mat[9] * Mat[15] -
					Mat[4] * Mat[11] * Mat[13] -
					Mat[8] * Mat[5] * Mat[15] +
					Mat[8] * Mat[7] * Mat[13] +
					Mat[12] * Mat[5] * Mat[11] -
					Mat[12] * Mat[7] * Mat[9];

				inv[12] = -Mat[4] * Mat[9] * Mat[14] +
					Mat[4] * Mat[10] * Mat[13] +
					Mat[8] * Mat[5] * Mat[14] -
					Mat[8] * Mat[6] * Mat[13] -
					Mat[12] * Mat[5] * Mat[10] +
					Mat[12] * Mat[6] * Mat[9];

				inv[1] = -Mat[1] * Mat[10] * Mat[15] +
					Mat[1] * Mat[11] * Mat[14] +
					Mat[9] * Mat[2] * Mat[15] -
					Mat[9] * Mat[3] * Mat[14] -
					Mat[13] * Mat[2] * Mat[11] +
					Mat[13] * Mat[3] * Mat[10];

				inv[5] = Mat[0] * Mat[10] * Mat[15] -
					Mat[0] * Mat[11] * Mat[14] -
					Mat[8] * Mat[2] * Mat[15] +
					Mat[8] * Mat[3] * Mat[14] +
					Mat[12] * Mat[2] * Mat[11] -
					Mat[12] * Mat[3] * Mat[10];

				inv[9] = -Mat[0] * Mat[9] * Mat[15] +
					Mat[0] * Mat[11] * Mat[13] +
					Mat[8] * Mat[1] * Mat[15] -
					Mat[8] * Mat[3] * Mat[13] -
					Mat[12] * Mat[1] * Mat[11] +
					Mat[12] * Mat[3] * Mat[9];

				inv[13] = Mat[0] * Mat[9] * Mat[14] -
					Mat[0] * Mat[10] * Mat[13] -
					Mat[8] * Mat[1] * Mat[14] +
					Mat[8] * Mat[2] * Mat[13] +
					Mat[12] * Mat[1] * Mat[10] -
					Mat[12] * Mat[2] * Mat[9];

				inv[2] = Mat[1] * Mat[6] * Mat[15] -
					Mat[1] * Mat[7] * Mat[14] -
					Mat[5] * Mat[2] * Mat[15] +
					Mat[5] * Mat[3] * Mat[14] +
					Mat[13] * Mat[2] * Mat[7] -
					Mat[13] * Mat[3] * Mat[6];

				inv[6] = -Mat[0] * Mat[6] * Mat[15] +
					Mat[0] * Mat[7] * Mat[14] +
					Mat[4] * Mat[2] * Mat[15] -
					Mat[4] * Mat[3] * Mat[14] -
					Mat[12] * Mat[2] * Mat[7] +
					Mat[12] * Mat[3] * Mat[6];

				inv[10] = Mat[0] * Mat[5] * Mat[15] -
					Mat[0] * Mat[7] * Mat[13] -
					Mat[4] * Mat[1] * Mat[15] +
					Mat[4] * Mat[3] * Mat[13] +
					Mat[12] * Mat[1] * Mat[7] -
					Mat[12] * Mat[3] * Mat[5];

				inv[14] = -Mat[0] * Mat[5] * Mat[14] +
					Mat[0] * Mat[6] * Mat[13] +
					Mat[4] * Mat[1] * Mat[14] -
					Mat[4] * Mat[2] * Mat[13] -
					Mat[12] * Mat[1] * Mat[6] +
					Mat[12] * Mat[2] * Mat[5];

				inv[3] = -Mat[1] * Mat[6] * Mat[11] +
					Mat[1] * Mat[7] * Mat[10] +
					Mat[5] * Mat[2] * Mat[11] -
					Mat[5] * Mat[3] * Mat[10] -
					Mat[9] * Mat[2] * Mat[7] +
					Mat[9] * Mat[3] * Mat[6];

				inv[7] = Mat[0] * Mat[6] * Mat[11] -
					Mat[0] * Mat[7] * Mat[10] -
					Mat[4] * Mat[2] * Mat[11] +
					Mat[4] * Mat[3] * Mat[10] +
					Mat[8] * Mat[2] * Mat[7] -
					Mat[8] * Mat[3] * Mat[6];

				inv[11] = -Mat[0] * Mat[5] * Mat[11] +
					Mat[0] * Mat[7] * Mat[9] +
					Mat[4] * Mat[1] * Mat[11] -
					Mat[4] * Mat[3] * Mat[9] -
					Mat[8] * Mat[1] * Mat[7] +
					Mat[8] * Mat[3] * Mat[5];

				inv[15] = Mat[0] * Mat[5] * Mat[10] -
					Mat[0] * Mat[6] * Mat[9] -
					Mat[4] * Mat[1] * Mat[10] +
					Mat[4] * Mat[2] * Mat[9] +
					Mat[8] * Mat[1] * Mat[6] -
					Mat[8] * Mat[2] * Mat[5];

				det = Mat[0] * inv[0] + Mat[1] * inv[4] + Mat[2] * inv[8] + Mat[3] * inv[12];

				if (det == 0)
					return false;

				det = 1.0f / det;

				for (i = 0; i < 16; i++)
					invOut.Mat[i] = inv[i] * det;

				return true;
			}

			/*
			bool Invert2(Matrix4x4<T> &invOut) const	{	{

				 __m128 minor0, minor1, minor2, minor3;
				 __m128 row0, row1, row2, row3;
				 __m128 det, tmp1;

				 //Transpose (skip as we are already in transposed form?)
				 //tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src + 4));
				 //row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src + 8)), (__m64*)(src + 12));
				 //row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
				 //row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
				 //tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src + 2)), (__m64*)(src + 6));
				 //row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src + 10)), (__m64*)(src + 14));
				 //row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
				 //row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);
				 
				 row0 = _mm_load_ps(Row1);
				 row1 = _mm_load_ps(Row2);
				 row2 = _mm_load_ps(Row3);
				 row3 = _mm_load_ps(Row4);

				// -----------------------------------------------
				 tmp1 = _mm_mul_ps(row2, row3);
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				 minor0 = _mm_mul_ps(row1, tmp1);
				 minor1 = _mm_mul_ps(row0, tmp1);
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				 minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
				 minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
				 minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
				// -----------------------------------------------
				 tmp1 = _mm_mul_ps(row1, row2);
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				 minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
				 minor3 = _mm_mul_ps(row0, tmp1);
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				 minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
				 minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
				 minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
				// -----------------------------------------------
				 tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				 row2 = _mm_shuffle_ps(row2, row2, 0x4E);
				 minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
				 minor2 = _mm_mul_ps(row0, tmp1);
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				 minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
				 minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
				 minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
				// -----------------------------------------------
				 tmp1 = _mm_mul_ps(row0, row1);
				
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				 minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
				 minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				 minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
				 minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
				// -----------------------------------------------
				 tmp1 = _mm_mul_ps(row0, row3);
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				 minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
				 minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				 minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
				 minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
				// -----------------------------------------------
				 tmp1 = _mm_mul_ps(row0, row2);
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
				 minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
				 minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
				 tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
				 minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
				 minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
				// -----------------------------------------------
				 det = _mm_mul_ps(row0, minor0);
				 det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
				 det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
				 tmp1 = _mm_rcp_ss(det);
				 det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
				 det = _mm_shuffle_ps(det, det, 0x00);
				 minor0 = _mm_mul_ps(det, minor0);
				 _mm_storel_pi((__m64*)(src), minor0);
				 _mm_storeh_pi((__m64*)(src + 2), minor0);
				 minor1 = _mm_mul_ps(det, minor1);
				 _mm_storel_pi((__m64*)(src + 4), minor1);
				 _mm_storeh_pi((__m64*)(src + 6), minor1);
				 minor2 = _mm_mul_ps(det, minor2);
				 _mm_storel_pi((__m64*)(src + 8), minor2);
				 _mm_storeh_pi((__m64*)(src + 10), minor2);
				 minor3 = _mm_mul_ps(det, minor3);
				 _mm_storel_pi((__m64*)(src + 12), minor3);
				 _mm_storeh_pi((__m64*)(src + 14), minor3);
			}
			*/


			void Make_RotationD(T ang, T x, T y, T z) {
				T rad = PMath::DEG_TO_RAD<T>() * ang;
				T c = cos(rad);
				T s = sin(rad);
				T omc = 1 - c;

				e11 = x * x * omc + c;
				e12 = y * x * omc + z * s;
				e13 = x * z * omc - y * s;
				e14 = 0;

				e21 = x * y * omc - z * s;
				e22 = y * y * omc + c;
				e23 = y * z * omc + x * s;
				e24 = 0;

				e31 = x * z * omc + y * s;
				e32 = y * z * omc - x * s;
				e33 = z * z * omc + c;
				e34 = 0;

				e41 = 0;
				e42 = 0;
				e43 = 0;
				e44 = 1;
			}

			template <typename T> void Transpose(void) {
#ifdef __SSE_AVAIL__
				_MM_TRANSPOSE4_PS(ssem1, ssem2, ssem3, ssem4);
#else
				Matrix4x4<T> m = *this;

					Mat[0] = m.Mat[0]; Mat[1] = m.Mat[4]; Mat[2] = m.Mat[8]; Mat[3] = m.Mat[12];
					Mat[4] = m.Mat[1]; Mat[5] = m.Mat[5]; Mat[6] = m.Mat[9]; Mat[7] = m.Mat[13];
					Mat[8] = m.Mat[2]; Mat[9] = m.Mat[6]; Mat[10] = m.Mat[10]; Mat[11] = m.Mat[14];
					Mat[12] = m.Mat[3]; Mat[13] = m.Mat[7]; Mat[14] = m.Mat[11]; Mat[15] = m.Mat[15];
#endif
			};
			



			/** \brief Transform the vector by the rotation/translation represented by the matrix
			 *
			 *       x = v.X * e11 + v.Y * e21 + v.Z * e31 + e41
			 *       y = v.X * e12 + v.Y * e22 + v.Z * e32 + e42
			 *       z = v.X * e13 + v.Y * e23 + v.Z * e33 + e43
			 * @param v Vector to transform
			 * @return Transformed vector
			 */
			PVector3d<T> operator*(PVector3d<T> const &v) const;

			PPoint3d<T> operator*(PPoint3d<T> const &v) const;


			/** Convert matrix into a quaternion
			 * @return Equivilent quaternion
			 */
			PQuaternion<T> To_Quaternion(void) const;


			/** Get translation component of the matrix
			 * @return Vector with 3d translation
			 */
			PVector3d<T> Get_Translation(void) const;


			Matrix3x3<T> Get_Mat3x3(void) const;

			/** Set translation component of the matrix
			 * @param v Translation to set within matrix
			 */
			void Set_Translation(PVector3d<T> v);


			void Set_Translation(T x, T y, T z);

			/** Get element of the matrix
			 * @param x column of matrix
			 * @param y row of matrix
			 * @return Matrix element
			 */
			T Element(int x, int y) const { return Mat[x + y * 4]; }


			//! Make the matrix all 0s
			void Make_Zero(void) { memset(Mat, 0, 16 * sizeof(T)); };


			//! Make the matrix the identity matrix
			void Make_Identity(void) { Make_Zero();  e11 = e22 = e33 = e44 = 1; };

	

			//! Print the matrix
			void Print(void) const {
				std::cout << e11 << ", " << e21 << ", " << e31 << ", " << e41 << std::endl;
				std::cout << e12 << ", " << e22 << ", " << e32 << ", " << e42 << std::endl;
				std::cout << e13 << ", " << e23 << ", " << e33 << ", " << e43 << std::endl;
				std::cout << e14 << ", " << e24 << ", " << e34 << ", " << e44 << std::endl;
			};


			//! Matrix in column major order accessible via array or individual matrix elements
			union {
				__declspec(align(16)) struct {
					T e11, e12, e13, e14;            // opengl matrix is column major
					T e21, e22, e23, e24;            //   e11 e21 e31 e41
					T e31, e32, e33, e34;			 //   e12 e22 e32 e42
					T e41, e42, e43, e44;			 //   e13 e23 e33 e43
				};									 //   e14 e24 e34 e44
				__declspec(align(16)) T Mat[16];

				__declspec(align(16)) struct {
					T Row1[4];
					T Row2[4];
					T Row3[4];
					T Row4[4];
				};
			};
		};

#include "PVector3d.hpp"
		template <typename T> PVector3d<T> Matrix4x4<T>::operator*(PVector3d<T> const &v) const {
			return PVector3d<T>(v.X * e11 + v.Y * e21 + v.Z * e31 + e41,
				v.X * e12 + v.Y * e22 + v.Z * e32 + e42,
				v.X * e13 + v.Y * e23 + v.Z * e33 + e43);
		};

		template <typename T> PPoint3d<T> Matrix4x4<T>::operator*(PPoint3d<T> const &v) const {
			return PPoint3d<T>(v.X * e11 + v.Y * e21 + v.Z * e31 + e41,
				v.X * e12 + v.Y * e22 + v.Z * e32 + e42,
				v.X * e13 + v.Y * e23 + v.Z * e33 + e43);
		};

		template <typename T> PVector3d<T> Matrix4x4<T>::Get_Translation(void) const {
			return PVector3d<T>(e41, e42, e43);
		};

		template <typename T> void Matrix4x4<T>::Set_Translation(PVector3d<T> v) {
			e41 = v.X; e42 = v.Y; e43 = v.Z;
		};

		template <typename T> void Matrix4x4<T>::Set_Translation(T x, T y, T z) {
			e41 = x; e42 = y; e43 = z;
		};





#include "PQuaternion.hpp"
		template <typename T> PQuaternion<T> Matrix4x4<T>::To_Quaternion(void) const {
			PQuaternion<T> quat;

			T trace = e11 + e22 + e33; // I removed + 1.0f; see discussion with Ethan
			if (trace > 0) {// I changed M_EPSILON to 0
				T s = 0.5f / sqrt(trace + 1.0f);
				quat.W = 0.25f / s;
				quat.X = (e32 - e23) * s;
				quat.Y = (e13 - e31) * s;
				quat.Z = (e21 - e12) * s;
			}
			else {
				if (e11 > e22 && e11 > e33) {
					T s = 2.0f * sqrt(1.0f + e11 - e22 - e33);
					quat.W = (e32 - e23) / s;
					quat.X = 0.25f * s;
					quat.Y = (e12 + e21) / s;
					quat.Z = (e13 + e31) / s;
				}
				else if (e22 > e33) {
					T s = 2.0f * sqrt(1.0f + e22 - e11 - e33);
					quat.W = (e13 - e31) / s;
					quat.X = (e12 + e21) / s;
					quat.Y = 0.25f * s;
					quat.Z = (e23 + e32) / s;
				}
				else {
					T s = 2.0f * sqrt(1.0f + e33 - e11 - e22);
					quat.W = (e21 - e12) / s;
					quat.X = (e13 + e31) / s;
					quat.Y = (e23 + e32) / s;
					quat.Z = 0.25f * s;
				}
			}
			return quat;
		};


		template <typename T> Matrix3x3<T> Matrix4x4<T>::Get_Mat3x3(void) const {
			return Matrix3x3<T>(e11, e12, e13, e21, e22, e23, e31, e32, e33);
		};

		
	};

};

typedef PSTD::PMath::Matrix4x4<float> Matrix4x4f;
typedef PSTD::PMath::Matrix4x4<double> Matrix4x4d;


#endif

