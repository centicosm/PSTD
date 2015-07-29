/** \file quaternion.hpp
 *  \brief Quaternion class
 *
 * Class for storing and manipulating quaternions 
 */

#ifndef P_QUATERNION_H
#define P_QUATERNION_H

#include <math.h>
#include <stdio.h>
#include "PVector3d.hpp"
#include "PMatrix4x4.hpp"


namespace PSTD {
   namespace PMath {

      template <typename T> class PVector3d;
      template <typename T> class Matrix4x4;

      //! Constant to prevent divide by zero errors when normalizing quaternions
      const float PQ_NORMALIZE_TOLERANCE = 0.00001f;

      
      const float SLERP_LERP_THRESHOLD = 0.95f;

      //! Class for manipulating quaternions
      template<typename T>
      class PQuaternion {

      public:

         /** Initialize quaternion with individual components
          *  @param w w
          *  @param x x 
          *  @param y y
          *  @param z z
          */
         PQuaternion(T w, T x, T y, T z) { W = w; X = x; Y = y; Z = z; };

         
         /** Initialize quaternion with vector
          *  @param v Vector to init quaternion from
          */
         PQuaternion(PVector3d<T> v);

         //! Initialize quaternion with the identity
         PQuaternion(void) { W = 1; X = 0; Y = 0; Z = 0; };

         
        /** Create a quaternion based on an axis and rotation angle
          *  @param axis axis to rotate around
          *  @param ang amount to rotate around that axis 
          */
         PQuaternion(const PVector3d<T> &axis, T ang); 

         
         /** Create a quaternion based on roll, pitch, and yaw values
          *  @param roll amount to roll
          *  @param pitch amount to pitch
          *  @param yaw amount to yaw
          */
         PQuaternion(T roll, T pitch, T yaw) {
            T p = (T)(pitch * PMath::DEG_TO_RAD<T>() / 2.0f);  // (all used to be like:    roll * M_PI / 180.0 / 2.0
            T y = (T)(yaw * PMath::DEG_TO_RAD<T>() / 2.0f);
            T r = (T)(roll * PMath::DEG_TO_RAD<T>() / 2.0f);
            T sinp = sin(p);
            T siny = sin(y);
            T sinr = sin(r);
            T cosp = cos(p);
            T cosy = cos(y);
            T cosr = cos(r);
            X = sinr * cosp * cosy - cosr * sinp * siny;
            Y = cosr * sinp * cosy + sinr * cosp * siny;
            Z = cosr * cosp * siny - sinr * sinp * cosy;
            W = cosr * cosp * cosy + sinr * sinp * siny;
            Normalize(); 
         };
         

         //! Deconstructor
         ~PQuaternion(void) {};
	
         //! Normalize the quaternion
         void Normalize(void) {	
            T mag = ((W * W) + (X * X) + (Y * Y) + (Z * Z));
            if (fabs(1.0f - mag) < PQ_NORMALIZE_TOLERANCE) return;
            mag = sqrt(mag);
            W /= mag; X /= mag; Y /= mag; Z /= mag; 
         };
         

         /** Return a copy of normalized quaternion
          * @return Normalized quaternion
          */
         PQuaternion<T> Get_Normalized(void) { 
            T mag = ((W * W) + (X * X) + (Y * Y) + (Z * Z));
            if (fabs(1.0f - mag) < PQ_NORMALIZE_TOLERANCE) {
               return PQuaternion(W, X, Y, Z);
            }
            mag = sqrt(mag);
            return PQuaternion<T>(W / mag, X / mag, Y / mag, Z / mag);
         };

         /** Return the conjugate quaternion
          * @return Conjugate quaternion
          */
         PQuaternion<T> Get_Conjugate(void) { return PQuaternion<T>(W, -X, -Y, -Z); };
		 
         /** Multiply the quaternion by another
          * @param rq Quaternion to multiply by
          * @return Multiplication result
          */
         PQuaternion<T> operator* (const PQuaternion<T> &rq) {
            return PQuaternion<T>(((W * rq.W) - (X * rq.X) - (Y * rq.Y) - (Z * rq.Z)),
                                  ((W * rq.X) + (X * rq.W) + (Y * rq.Z) - (Z * rq.Y)),
                                  ((W * rq.Y) + (Y * rq.W) + (Z * rq.X) - (X * rq.Z)),
                                  ((W * rq.Z) + (Z * rq.W) + (X * rq.Y) - (Y * rq.X))); 
         };

         /** Scale the quaternion
          * @param s Scale factor
          * @return Scaled quaternion
          */
         PQuaternion<T> operator* (T s) const {	return PQuaternion<T>(W * s, X * s, Y * s, Z * s); };

         /** Scale the quaternion
          * @param s Scale factor
          * @return Scaled quaternion
          */
         PQuaternion<T> operator/ (T s) const { return PQuaternion<T>(W / s, X / s, Y / s, Z / s); };

         /** Add two quaternions
          * @param q Quaternion to addr
          * @return Sum
          */
         PQuaternion<T> operator+ (PQuaternion<T> &q) const { return PQuaternion<T>(W + q.W, X + q.X, Y + q.Y, Z + q.Z); };


         /** \brief Multiply vector by quaternion
          *
          * This will rotate vector the vector by the rotation stored by the quaternions \n
          * q * v * ~q
          * @param v Vector to rotate
          * @return Rotated vector
          */
         PVector3d<T> operator* (const PVector3d<T> &v);

         /** Set the quaternion's rotation based on a axis and rotation in radians
          * @param axis Rotation axis
          * @param ang Amount to rotate in radians
          */
         void SetFromAxisR(const PVector3d<T> &axis, T ang);

         /** Set the quaternion's rotation based on a axis and rotation in degrees
          * @param axis Rotation axis
          * @param ang Amount to rotate in degrees
          */
         void SetFromAxisD(const PVector3d<T> &axis, T ang);

         /** Compute the dot product between two quaternions
          * @param q quaternion
          * @return Dot product
          */         
         T Dot(const PQuaternion &q) {return ((W * q.W) + (X * q.X) + (Y * q.Y) + (Z * q.Z)); };

         /** Set the quaternion's rotation based on roll, pitch, and yaw in degrees
          * @param roll Roll
          * @param pitch Pitch
          * @param yaw Yaw
          */     
         void SetFromEulerD(T roll, T pitch, T yaw) {
            SetFromEulerR(roll * PMath::constants::DEG_TO_RAD<T>(), pitch *  PMath::constants::DEG_TO_RAD<T>(), yaw *  PMath::constants::DEG_TO_RAD<T>());
         };

         /** Set the quaternion's rotation based on roll, pitch, and yaw in radians
          * @param roll Roll
          * @param pitch Pitch
          * @param yaw Yaw
          */   
         void SetFromEulerR(T roll, T pitch, T yaw) {
            T p = (T)(pitch / 2.0);
            T y = (T)(yaw / 2.0);
            T r = (T)(roll / 2.0);
            T sinp = sin(p);
            T siny = sin(y);
            T sinr = sin(r);
            T cosp = cos(p);
            T cosy = cos(y);
            T cosr = cos(r);
            X = sinr * cosp * cosy - cosr * sinp * siny;
            Y = cosr * sinp * cosy + sinr * cosp * siny;
            Z = cosr * cosp * siny - sinr * sinp * cosy;
            W = cosr * cosp * cosy + sinr * sinp * siny;
            Normalize(); 
         };
         
         //! Print the quaternion
         void Print(void) { std::cout << "Quat: " << W << ", " << X << ", " << Y << ", " << Z << std::endl; };
         
         /** Get the pitch of the quanternions rotation
          * @return Pitch angle
          */
         T Get_Pitch(void) { return asin(2* (W * Y - X * Z)); };

         /** Get the Roll of the quanternions rotation
          * @return Roll angle
          */
         T Get_Roll(void) {return atan2(2 * (Y * Z + W * X), 1 - 2 * ((X * X) + (Y * Y))); };
         
         /** Get the Yaw of the quanternions rotation
          * @return Yaw angle
          */
         T Get_Yaw(void) { return atan2(2*(X*Y + W*Z), 1 - 2 * (Y*Y + Z*Z)); };
         
         /** Get an axis and angle representation of the quanternions rotation
          * @param axis the quaternion rotates around 
          * @return Amount of rotation
          */
         T Get_AxisAngle(PVector3d<T> *axis);

         //inline T Get_LerpSpeed(PQuaternion &q, int timesteps) { };

         /** Linearly interpolate between this quaternion and the given quaternion
          * @param q Goal rotation
          * @param amt Percentage (1 - 0) of the distance interpolated
          * @return Interpolated quaternion
          */
         PQuaternion<T> Lerp(const PQuaternion<T> &q, T amt) { 
            T omt = 1 - amt;
            return PQuaternion((W * omt) + (q.W * amt),
                               (X * omt) + (q.X * amt),
                               (Y * omt) + (q.Y * amt),
                               (Z * omt) + (q.Z * amt)).Get_Normalized(); 
         };


         /** \brief Sperically linearly interpolate between this quaternion and the given quaternion
          *
          * Interpolate between two quaternions using a spherical interpolation for large distances
          *   and switching to a linear interpolation at smaller distances
          * @param q Goal rotation
          * @param amt Percentage (1 - 0) of the distance interpolated
          * @return Interpolated quaternion
          */
         PQuaternion<T> Slerp(const PQuaternion<T> &q, T amt) { 
            PQuaternion<T> tq;
            T dot = Dot(q);
            if (dot < 0)  { dot = -dot; tq = q * -1; }
            if (dot > SLERP_LERP_THRESHOLD) return Lerp(tq, amt);
            T ang = acosf(dot);
            return (*this * sinf(ang * (1 - amt)) + (tq * sinf(ang * amt)) / sinf(ang));
         };
            
         /** Convert the quaternion to its equivelent 4x4 matrix
          * @return Equivalent 4x4 matrix
          */
         Matrix4x4<T> To_Matrix(void) const;
//		 Matrix4x4<T> To_Matrix2(void);


		 void Get_EulerAngles(float &roll, float &pitch, float &yaw) {
			 roll = atan2f(2 * Y * W - 2 * X * Z, 1 - 2 * Y * Y - 2 * Z * Z);
			 pitch = atan2(2 * X * W - 2 * Y * Z, 1 - 2 * X * X - 2 * Z * Z);
			 yaw = asinf(2 * X * Y + 2 * Z * W);
		 }

		 union {
			 struct {
				 T W;  //!< W component
				 T X;  //!< X component
				 T Y;  //!< Y component
				 T Z;  //!< Z component
			 };	
			 T Vec[4];
		 };

      };
      
   

#include "PVector3d.hpp"
      template <typename T> PQuaternion<T>::PQuaternion(PVector3d<T> v) { 
         W = 0; X = v.X; Y = v.Y, Z = v.Z;
      };

      
      // Constructor derived from axis and angle in radians
      template <typename T> PQuaternion<T>::PQuaternion(const PVector3d<T> &axis, T ang) { 
         ang *= 0.5f;                                                   
         PVector3d<T> vn(axis.X / axis.Length(),  axis.Y / axis.Length(), axis.Z / axis.Length());
         T sinang = sin(ang); X = (vn.X * sinang);                  
         Y = (vn.Y * sinang); Z = (vn.Z * sinang);
         W = cos(ang); 
      };


      // Rotate vector v by quaternion q
      // q * v * ~q
      template <typename T> PVector3d<T> PQuaternion<T>::operator* (const PVector3d<T> &v) { 
         PQuaternion<T> v_quat(v);
         PQuaternion<T> rotvec = v_quat * Get_Conjugate();
         rotvec = *this * rotvec;
         return PVector3d<T>(rotvec.X, rotvec.Y, rotvec.Z);
      };

      // Convert to quaternion from axis and angle in rad
      template <typename T> void PQuaternion<T>::SetFromAxisR(const PVector3d<T> &axis, T ang) { 
         ang *= 0.5f; PVector3d<T> vn(axis.X / axis.Length(), axis.Y / axis.Length(), axis.Z / axis.Length()); 
         T sinang = sin(ang); X = (vn.X * sinang);
         Y = (vn.Y * sinang); Z = (vn.Z * sinang);
         W = cos(ang);
      };
      
      // Convert to quaternion from axis and angle in deg
      template <typename T> void PQuaternion<T>::SetFromAxisD(const PVector3d<T> &axis, T ang) { 
		  ang *= (T)(PM_DEG2RADf);
         SetFromAxisR(axis, ang);
      };

      template <typename T> T PQuaternion<T>::Get_AxisAngle(PVector3d<T> *axis) { 
         T scale = sqrt(X * X + Y * Y + Z * Z);
         axis->X = X / scale;
         axis->Y = Y / scale;
         axis->Z = Z / scale;
         return acos(W) * 2.0f;
      };


	  


#include "PMatrix4x4.hpp"
  

	  template <typename T> Matrix4x4<T> PQuaternion<T>::To_Matrix(void) const {
		  Matrix4x4<T> m;

		  T x2 = X * X;
		  T y2 = Y * Y;
		  T z2 = Z * Z;
		  T xy = X * Y;
		  T xz = X * Z;
		  T yz = Y * Z;
		  T wx = W * X;
		  T wy = W * Y;
		  T wz = W * Z;

		  // This calculation would be a lot more complicated for non-unit length quaternions
		  // Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
		  //   OpenGL
		  return Matrix4x4<T>(1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
			  2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
			  2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
			  0.0f, 0.0f, 0.0f, 1.0f);
	  }
	  
   };

   

   //typedef PSTD::PMath::PQuaternion<float> PQuaternionf;
};

typedef PSTD::PMath::PQuaternion<float> PQuaternionf;
      
#endif
