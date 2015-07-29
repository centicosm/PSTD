/** \file points.hpp
 *  \brief Classes defining geometric points
 *
 * Class for storing geometric points
 */

#ifndef PPOINTS_H
#define PPOINTS_H

namespace PSTD {
	namespace PMath {

		//! Three dimensional point
		template <typename T>
		class PPoint3d {
			public:
			T X; //!< X Coordinate
			T Y; //!< Y Coordinate
			T Z; //!< Z Coordinate

			PPoint3d(void) { X = Y = Z = 0; };
			PPoint3d(float x, float y, float z) { X = (T)x; Y = (T)y; Z = (T)z; };
			PPoint3d(int x, int y, int z) { X = (T)x; Y = (T)y; Z = (T)z; };
			PPoint3d(double x, double y, double z) { X = (T)x; Y = (T)y; Z = (T)z; };
			PPoint3d(const PPoint3d &p) { X = p.X; Y = p.Y; Z = p.Z; };
		};


		struct Point2dui {
			unsigned int X;
			unsigned int Y;
		};

		struct Point2dus {
			unsigned short X;
			unsigned short Y;
		};
	};
};



#endif
