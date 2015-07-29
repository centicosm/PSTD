#ifndef PVECTOR_4D_HPP
#define PVECTOR_4D_HPP

namespace PSTD {
	namespace PMath {

		template<typename T>
		class PVector4d {
		public:

			//! Default constructor initializing all components to 0
			PVector4d(void) : X(0), Y(0), Z(0), W(0) {};

			/** @brief Initialize vector with components
			*  @param x x
			*  @param y y
			*  @param z z
			*/
			PVector4d(T x, T y, T z, T w) : X(x), Y(y), Z(z), W(w) {};
			/** @brief  Initialize vector as point 2 - point 1
			*  @param p1 point 1
			*  @param p2 point 2
			*/


			void Normalize(void) { float mag = sqrt((X * X) + (Y * Y) + (Z * Z) + (W * W)); X /= mag; Y /= mag; Z /= mag; W /= mag; }

			__declspec(align(16)) union {
				__declspec(align(16)) struct {
					T X;
					T Y;
					T Z;
					T W;
				};
				__declspec(align(16)) T _Vec[4];
			};

		};



	};
};

typedef PSTD::PMath::PVector4d<float> PVector4df;
typedef PSTD::PMath::PVector4d<double> PVector4dd;


#endif