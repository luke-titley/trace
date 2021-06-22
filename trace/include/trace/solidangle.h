//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SOLIDANGLE
#define TC_SOLIDANGLE
//------------------------------------------------------------------------------
#include "trace/log.h"
#include "trace/matrix.h"
#include "trace/random.h"
#include "trace/test.h"
#include "trace/vector.h"
//------------------------------------------------------------------------------
#include <cmath>
#include <cstdlib>
//------------------------------------------------------------------------------

namespace tc
{

//------------------------------------------------------------------------------
// sphericalToCartesian
//------------------------------------------------------------------------------
/// \brief Given the polar coordinates pitch and yaw, return a 3 dimensional
/// vector in cartesian space.
//------------------------------------------------------------------------------
inline Vector3<float> sphericalToCartesian(const float pitch, const float yaw)
{
    const float cosYaw = cos(yaw);
    const float sinYaw = sin(yaw);
    const float cosPitch = cos(pitch);
    const float sinPitch = sin(pitch);

    const Vector3<float> result(sinYaw * sinPitch, cosYaw, sinYaw * cosPitch);
    return result;
}

//------------------------------------------------------------------------------
// halfwayPoint
//------------------------------------------------------------------------------
/// \brief Returns the average of the given two values.
//------------------------------------------------------------------------------
inline float halfwayPoint(const float a, const float b)
{
    return (a + b) / 2.0f;
}

typedef float (*OffsetFunction)(const float a, const float b);

//------------------------------------------------------------------------------
// generateStratifiedDirection
//------------------------------------------------------------------------------
/// \brief This generates a ray in a solid angle of radius 1 around the origin
/// (0,0) facing along the Y axis.
/// The solid angle is split into a number of strata specified by the maximum
/// yaw ('yawSamples') and pitch ('pitchSamples'). One ray is produced for each
/// strata. The ray chosen is specified by yaw and pitch where:
///	- yaw >= 0 && yaw < yawSamples
///	- pitch >= 0 && pitch < pitchSamples
///
/// \tparam offsetFunction Makes it possible to specify the function used to
/// pick a direction inside a given strata.
///
/// The options are:
///                  - The centre of the strata
///                     offsetFunction=halfwayPoint
///                  - A random direction inside the strata
/// 					offsetFunction=randomDirection (from random.h)
///
/// \param pitchAngleRange: A value from 0 to 1 which specifies how open the
/// solid angle is. A value of 1 means completely open (which produces a
/// hemisphere). A value of 0.1 would produce a cone like shape around the y
/// axis.
/// \param yawSamples: The number of divisions in the yaw rotation of the
/// hemisphere.
/// \param pitchSamples: The number of divisions in the pitch rotation of the
/// hemisphere.
/// \param yaw: Specifies the yaw index of the strata we want a ray for.
/// \param pitch: Specifies the pitch index of the strata we want a ray for.
/// \return A direction (as a tc::Vector3<float>) within the strata given by yaw
/// and pitch.
//------------------------------------------------------------------------------
template <OffsetFunction offsetFunction>
Vector3<float> generateStratifiedDirection(const float pitchAngleRange,
                                           const size_t yawSamples,
                                           const size_t pitchSamples,
                                           const size_t yaw, const size_t pitch)
{
    // Could be cached
    const float pitchOffset = 1.0f - pitchAngleRange;
    const float pitchStep =
        (pitchAngleRange * M_PI_2l) / static_cast<float>(pitchSamples);
    const float yawStep = (M_PI * 2.0f) / static_cast<float>(yawSamples);

    // Pitch
    const float pitchLowerStrata =
        pitchOffset + (pitchStep * static_cast<float>(pitch + 0));
    const float pitchUpperStrata =
        pitchOffset + (pitchStep * static_cast<float>(pitch + 1));
    const float pitchMidStrata =
        offsetFunction(pitchLowerStrata, pitchUpperStrata);
    // Yaw
    const float yawLowerStrata = yawStep * static_cast<float>(yaw);
    const float yawUpperStrata = yawStep * static_cast<float>(yaw + 1);
    const float yawMidStrata = offsetFunction(yawLowerStrata, yawUpperStrata);
    const Vector3<float> result =
        sphericalToCartesian(yawMidStrata, pitchMidStrata);
    return result;
}

//------------------------------------------------------------------------------
// generateRandomDirection
//------------------------------------------------------------------------------
/// \brief Produces a random direction in a box specified by 'radius'. The
/// orientation of the box is given by the frame.m_x/m_y/m_z vectors.
//------------------------------------------------------------------------------
inline Vector3<float> generateRandomDirection(const Matrix<float>& frame,
                                              const float radius)
{
    const Vector3<float> ti = generateRandomFloat(-radius, radius);
    const Vector3<float> tj = generateRandomFloat(0, radius);
    const Vector3<float> tk = generateRandomFloat(-radius, radius);

    const Vector3<float> randomRay =
        ((frame.m_x * ti) + (frame.m_y * tj) + (frame.m_z * tk)).normalized();
    return randomRay;
}

//------------------------------------------------------------------------------
// Runs all the unit tests for the 'solidangle' header file.
/// \cond
void solidangleRunUnitTests(const tc::LogContext& logContext);
/// \endcond

}  // namespace tc
#endif  // TC_SOLIDANGLE
