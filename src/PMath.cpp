#include "PMath.h"


namespace PSTD {
	namespace PMath {

		// Greatest common divisor - Binary algorithm
		unsigned int GCD(unsigned int u, unsigned int v) {

			if ((u == 0) || (v == 0) || (v == 0)) return u | v;

			if (!(u & 0x01)) {
				if (!(v & 0x01)) {
					return (PMath::GCD((u >> 1), (v >> 1)) << 1);
				}
				else {
					return PMath::GCD((u >> 1), v);
				}
			}
			else if (!(v & 0x01)) {
				return PMath::GCD(u, (v >> 1));
			}
			else {
				if ((u > v)) {
					return PMath::GCD(((u - v) >> 1), v);
				}
				else {
					return PMath::GCD(((v - u) >> 1), u);
				}
			}
		}

		int GCD(std::vector<unsigned int> &val) {
			size_t numVal = val.size();
			if (numVal < 2) return 0;

			unsigned int curGCD = PMath::GCD(val[0], val[1]);

			for (unsigned int i = 2; i < numVal; i++) {
				curGCD = PMath::GCD(curGCD, val[i]);
			}
			return curGCD;
		}


		// Least common multiple
		int LCM(unsigned int a, unsigned int b) {
			return (a * b) / PMath::GCD(a, b);
		}


		int LCM(std::vector<unsigned int> &val) {
			size_t numVal = val.size();
			if (numVal < 2) return 0;

			unsigned int curLCM = PMath::LCM(val[0], val[1]);

			for (unsigned int i = 2; i < numVal; i++) {
				curLCM = PMath::LCM(curLCM, val[i]);
			}
			return curLCM;
		}



		bool Unproject(PVector3df &pos, Matrix4x4f &modelProjection, int *viewport, PVector3df &objCoord) {

			Matrix4x4f invMatrix;
			Matrix4x4f ddd = modelProjection;
			Invert_Matrix4x4(modelProjection, invMatrix);

			//Transformation of normalized coordinates between -1 and 1
			PVector4df inVec;
			inVec.X = ((pos.X - (float)viewport[0]) / (float)viewport[2]) * 2.0f - 1.0f;
			inVec.Y = ((pos.Y - (float)viewport[1]) / (float)viewport[3]) * 2.0f - 1.0f;
			inVec.Z = (2.0f * pos.Z) - 1.0f;
			inVec.W = 1.0;

			//Objects coordinates
			PVector4df outVec = invMatrix * inVec;

			if (outVec.Z == 0.0f) return false;

			objCoord.X = outVec.X / outVec.W;
			objCoord.Y = outVec.Y / outVec.W;
			objCoord.Z = outVec.Z / outVec.W;
			return true;
		}


		void Invert_Matrix4x4(Matrix4x4<float> &src) {

			// XXX DANGER VSC SPECIFIC ALIGNMENT OPERATOR
			__declspec(align(16)) const unsigned int _Sign_PNNP[4] = { 0x00000000, 0x80000000, 0x80000000, 0x00000000 };

			// Load the full matrix into registers
			__m128 _L1 = _mm_load_ps(src.Row1);
			__m128 _L2 = _mm_load_ps(src.Row2);
			__m128 _L3 = _mm_load_ps(src.Row3);
			__m128 _L4 = _mm_load_ps(src.Row4);

			_MM_TRANSPOSE4_PS(_L1, _L2, _L3, _L4);

			// The inverse is calculated using "Divide and Conquer" technique. The
			// original matrix is divide into four 2x2 sub-matrices. Since each
			// register holds four matrix element, the smaller matrices are
			// represented as a registers. Hence we get a better locality of the
			// calculations.

			__m128 A, B, C, D; // the four sub-matrices
			{
				A = _mm_movelh_ps(_L1, _L2);
				B = _mm_movehl_ps(_L2, _L1);
				C = _mm_movelh_ps(_L3, _L4);
				D = _mm_movehl_ps(_L4, _L3);
			}

			__m128 iA, iB, iC, iD, DC, AB;         // partial inverse of the sub-matrices
			__m128 dA, dB, dC, dD;                 // determinant of the sub-matrices
			__m128 det, d, d1, d2;
			__m128 rd;                             // reciprocal of the determinant

			//  AB = A# * B
			AB = _mm_mul_ps(_mm_shuffle_ps(A, A, 0x0F), B);
			AB = _mm_sub_ps(AB, _mm_mul_ps(_mm_shuffle_ps(A, A, 0xA5), _mm_shuffle_ps(B, B, 0x4E)));
			//  DC = D# * C
			DC = _mm_mul_ps(_mm_shuffle_ps(D, D, 0x0F), C);
			DC = _mm_sub_ps(DC, _mm_mul_ps(_mm_shuffle_ps(D, D, 0xA5), _mm_shuffle_ps(C, C, 0x4E)));

			//  dA = |A|
			dA = _mm_mul_ps(_mm_shuffle_ps(A, A, 0x5F), A);
			dA = _mm_sub_ss(dA, _mm_movehl_ps(dA, dA));
			//  dB = |B|
			dB = _mm_mul_ps(_mm_shuffle_ps(B, B, 0x5F), B);
			dB = _mm_sub_ss(dB, _mm_movehl_ps(dB, dB));

			//  dC = |C|
			dC = _mm_mul_ps(_mm_shuffle_ps(C, C, 0x5F), C);
			dC = _mm_sub_ss(dC, _mm_movehl_ps(dC, dC));
			//  dD = |D|
			dD = _mm_mul_ps(_mm_shuffle_ps(D, D, 0x5F), D);
			dD = _mm_sub_ss(dD, _mm_movehl_ps(dD, dD));

			//  d = trace(AB*DC) = trace(A#*B*D#*C)
			d = _mm_mul_ps(_mm_shuffle_ps(DC, DC, 0xD8), AB);

			//  iD = C*A#*B
			iD = _mm_mul_ps(_mm_shuffle_ps(C, C, 0xA0), _mm_movelh_ps(AB, AB));
			iD = _mm_add_ps(iD, _mm_mul_ps(_mm_shuffle_ps(C, C, 0xF5), _mm_movehl_ps(AB, AB)));
			//  iA = B*D#*C
			iA = _mm_mul_ps(_mm_shuffle_ps(B, B, 0xA0), _mm_movelh_ps(DC, DC));
			iA = _mm_add_ps(iA, _mm_mul_ps(_mm_shuffle_ps(B, B, 0xF5), _mm_movehl_ps(DC, DC)));

			//  d = trace(AB*DC) = trace(A#*B*D#*C) [continue]
			d = _mm_add_ps(d, _mm_movehl_ps(d, d));
			d = _mm_add_ss(d, _mm_shuffle_ps(d, d, 1));
			d1 = _mm_mul_ss(dA, dD);
			d2 = _mm_mul_ss(dB, dC);

			//  iD = D*|A| - C*A#*B
			iD = _mm_sub_ps(_mm_mul_ps(D, _mm_shuffle_ps(dA, dA, 0)), iD);

			//  iA = A*|D| - B*D#*C;
			iA = _mm_sub_ps(_mm_mul_ps(A, _mm_shuffle_ps(dD, dD, 0)), iA);

			//  det = |A|*|D| + |B|*|C| - trace(A#*B*D#*C)
			det = _mm_sub_ss(_mm_add_ss(d1, d2), d);
			rd = _mm_div_ss(_mm_set_ss(1.0f), det);

			//  iB = D * (A#B)# = D*B#*A
			iB = _mm_mul_ps(D, _mm_shuffle_ps(AB, AB, 0x33));
			iB = _mm_sub_ps(iB, _mm_mul_ps(_mm_shuffle_ps(D, D, 0xB1), _mm_shuffle_ps(AB, AB, 0x66)));
			//  iC = A * (D#C)# = A*C#*D
			iC = _mm_mul_ps(A, _mm_shuffle_ps(DC, DC, 0x33));
			iC = _mm_sub_ps(iC, _mm_mul_ps(_mm_shuffle_ps(A, A, 0xB1), _mm_shuffle_ps(DC, DC, 0x66)));

			rd = _mm_shuffle_ps(rd, rd, 0);
			rd = _mm_xor_ps(rd, _mm_load_ps((float*)_Sign_PNNP));

			//  iB = C*|B| - D*B#*A
			iB = _mm_sub_ps(_mm_mul_ps(C, _mm_shuffle_ps(dB, dB, 0)), iB);

			//  iC = B*|C| - A*C#*D;
			iC = _mm_sub_ps(_mm_mul_ps(B, _mm_shuffle_ps(dC, dC, 0)), iC);

			//  iX = iX / det
			iA = _mm_mul_ps(rd, iA);
			iB = _mm_mul_ps(rd, iB);
			iC = _mm_mul_ps(rd, iC);
			iD = _mm_mul_ps(rd, iD);

			_mm_store_ps(src.Row1, _mm_shuffle_ps(iA, iB, 0x77));
			_mm_store_ps(src.Row2, _mm_shuffle_ps(iA, iB, 0x22));
			_mm_store_ps(src.Row3, _mm_shuffle_ps(iC, iD, 0x77));
			_mm_store_ps(src.Row4, _mm_shuffle_ps(iC, iD, 0x22));
		}
	

		void Invert_Matrix4x4(const Matrix4x4<float> &src, Matrix4x4<float> &dest) {


			// XXX DANGER VSC SPECIFIC ALIGNMENT OPERATOR
			__declspec(align(16)) const unsigned int _Sign_PNNP[4] = { 0x00000000, 0x80000000, 0x80000000, 0x00000000 };

			// Load the full matrix into registers
			__m128 _L1 = _mm_load_ps(src.Row1);
			__m128 _L2 = _mm_load_ps(src.Row2);
			__m128 _L3 = _mm_load_ps(src.Row3);
			__m128 _L4 = _mm_load_ps(src.Row4);
			_MM_TRANSPOSE4_PS(_L1, _L2, _L3, _L4);

			// The inverse is calculated using "Divide and Conquer" technique. The
			// original matrix is divide into four 2x2 sub-matrices. Since each
			// register holds four matrix element, the smaller matrices are
			// represented as a registers. Hence we get a better locality of the
			// calculations.

			__m128 A, B, C, D; // the four sub-matrices
			{
				A = _mm_movelh_ps(_L1, _L2);
				B = _mm_movehl_ps(_L2, _L1);
				C = _mm_movelh_ps(_L3, _L4);
				D = _mm_movehl_ps(_L4, _L3);
			}

			__m128 iA, iB, iC, iD,                 // partial inverse of the sub-matrices
				DC, AB;
			__m128 dA, dB, dC, dD;                 // determinant of the sub-matrices
			__m128 det, d, d1, d2;
			__m128 rd;                             // reciprocal of the determinant

			//  AB = A# * B
			AB = _mm_mul_ps(_mm_shuffle_ps(A, A, 0x0F), B);
			AB = _mm_sub_ps(AB, _mm_mul_ps(_mm_shuffle_ps(A, A, 0xA5), _mm_shuffle_ps(B, B, 0x4E)));
			//  DC = D# * C
			DC = _mm_mul_ps(_mm_shuffle_ps(D, D, 0x0F), C);
			DC = _mm_sub_ps(DC, _mm_mul_ps(_mm_shuffle_ps(D, D, 0xA5), _mm_shuffle_ps(C, C, 0x4E)));

			//  dA = |A|
			dA = _mm_mul_ps(_mm_shuffle_ps(A, A, 0x5F), A);
			dA = _mm_sub_ss(dA, _mm_movehl_ps(dA, dA));
			//  dB = |B|
			dB = _mm_mul_ps(_mm_shuffle_ps(B, B, 0x5F), B);
			dB = _mm_sub_ss(dB, _mm_movehl_ps(dB, dB));

			//  dC = |C|
			dC = _mm_mul_ps(_mm_shuffle_ps(C, C, 0x5F), C);
			dC = _mm_sub_ss(dC, _mm_movehl_ps(dC, dC));
			//  dD = |D|
			dD = _mm_mul_ps(_mm_shuffle_ps(D, D, 0x5F), D);
			dD = _mm_sub_ss(dD, _mm_movehl_ps(dD, dD));

			//  d = trace(AB*DC) = trace(A#*B*D#*C)
			d = _mm_mul_ps(_mm_shuffle_ps(DC, DC, 0xD8), AB);

			//  iD = C*A#*B
			iD = _mm_mul_ps(_mm_shuffle_ps(C, C, 0xA0), _mm_movelh_ps(AB, AB));
			iD = _mm_add_ps(iD, _mm_mul_ps(_mm_shuffle_ps(C, C, 0xF5), _mm_movehl_ps(AB, AB)));
			//  iA = B*D#*C
			iA = _mm_mul_ps(_mm_shuffle_ps(B, B, 0xA0), _mm_movelh_ps(DC, DC));
			iA = _mm_add_ps(iA, _mm_mul_ps(_mm_shuffle_ps(B, B, 0xF5), _mm_movehl_ps(DC, DC)));

			//  d = trace(AB*DC) = trace(A#*B*D#*C) [continue]
			d = _mm_add_ps(d, _mm_movehl_ps(d, d));
			d = _mm_add_ss(d, _mm_shuffle_ps(d, d, 1));
			d1 = _mm_mul_ss(dA, dD);
			d2 = _mm_mul_ss(dB, dC);

			//  iD = D*|A| - C*A#*B
			iD = _mm_sub_ps(_mm_mul_ps(D, _mm_shuffle_ps(dA, dA, 0)), iD);

			//  iA = A*|D| - B*D#*C;
			iA = _mm_sub_ps(_mm_mul_ps(A, _mm_shuffle_ps(dD, dD, 0)), iA);

			//  det = |A|*|D| + |B|*|C| - trace(A#*B*D#*C)
			det = _mm_sub_ss(_mm_add_ss(d1, d2), d);
			rd = _mm_div_ss(_mm_set_ss(1.0f), det);

			//     #ifdef ZERO_SINGULAR
			//         rd = _mm_and_ps(_mm_cmpneq_ss(det,_mm_setzero_ps()), rd);
			//     #endif

			//  iB = D * (A#B)# = D*B#*A
			iB = _mm_mul_ps(D, _mm_shuffle_ps(AB, AB, 0x33));
			iB = _mm_sub_ps(iB, _mm_mul_ps(_mm_shuffle_ps(D, D, 0xB1), _mm_shuffle_ps(AB, AB, 0x66)));
			//  iC = A * (D#C)# = A*C#*D
			iC = _mm_mul_ps(A, _mm_shuffle_ps(DC, DC, 0x33));
			iC = _mm_sub_ps(iC, _mm_mul_ps(_mm_shuffle_ps(A, A, 0xB1), _mm_shuffle_ps(DC, DC, 0x66)));

			rd = _mm_shuffle_ps(rd, rd, 0);
			rd = _mm_xor_ps(rd, _mm_load_ps((float*)_Sign_PNNP));

			//  iB = C*|B| - D*B#*A
			iB = _mm_sub_ps(_mm_mul_ps(C, _mm_shuffle_ps(dB, dB, 0)), iB);

			//  iC = B*|C| - A*C#*D;
			iC = _mm_sub_ps(_mm_mul_ps(B, _mm_shuffle_ps(dC, dC, 0)), iC);

			//  iX = iX / det
			iA = _mm_mul_ps(rd, iA);
			iB = _mm_mul_ps(rd, iB);
			iC = _mm_mul_ps(rd, iC);
			iD = _mm_mul_ps(rd, iD);

			_mm_store_ps(dest.Row1, _mm_shuffle_ps(iA, iB, 0x77));
			_mm_store_ps(dest.Row2, _mm_shuffle_ps(iA, iB, 0x22));
			_mm_store_ps(dest.Row3, _mm_shuffle_ps(iC, iD, 0x77));
			_mm_store_ps(dest.Row4, _mm_shuffle_ps(iC, iD, 0x22));
		}


		Matrix4x4f Make_RotationMatrixD(float ang, float x, float y, float z) {
			Matrix4x4f mat;
			float c = (float)cos(PM_DEG2RAD * ang);
			float s = (float)sin(PM_DEG2RAD * ang);
			float omc = 1 - c;

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


  };
};




