#pragma once

#ifndef PGEOMETRY_H
#define PGEOMETRY_H

#include "PVector3d.hpp"
#include "PVector2d.hpp"

namespace PSTD {
	namespace PMath {

		struct PSphere {
			PVector3df _Center;
			float _Radius;

			PSphere(void) : _Center(PVector3df(0.0f, 0.0f, 0.0f)), _Radius(0.0f) { };
			bool In_Sphere(const PVector3df &pos) {
				return (_Center.DistanceSquared(pos) <= (_Radius * _Radius));
			}

		};

		struct PCircle {
			PVector2df _Center;
			float _Radius;

			bool In_Circle(const PVector2df &pos) {
				return (_Center.DistanceSquared(pos) <= (_Radius * _Radius));
			}
		};


		struct PPlane {
			PVector3df _Normal;
			PVector3df _Point;
			void Set(const PVector3df &n, const PVector3df &p) { _Normal = n, _Point = p; };
		};

		struct PPlane2 {
			PVector4df _Point;
		};

		struct AABB {
			PVector3df	_Center;
			PVector3df _Extent;
		};

	};
};

#endif

