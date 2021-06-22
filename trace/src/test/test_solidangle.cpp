//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/log.h"
#include "trace/random.h"
#include "trace/test.h"
#include "trace/solidangle.h"
//------------------------------------------------------------------------------
#include <algorithm>

namespace
{

//------------------------------------------------------------------------------
void sphericaltocartesian(const tc::LogContext& logContext)
{
    /// [test_solidangle sphericaltocartesian]
    tc::Vector3<float> result =
        tc::sphericalToCartesian(M_PI / 4.0f, M_PI / 4.0f).normalized();
    TC_IS(logContext, result == tc::Vector3<float>(0.5f, 0.707106829f, 0.5f));
    /// [test_solidangle sphericaltocartesian]
}

//------------------------------------------------------------------------------
void halfwaypoint(const tc::LogContext& logContext)
{
    /// [test_solidangle halfwaypoint]
    TC_IS(logContext, tc::halfwayPoint(2.0f, 4.0f) == 3.0f);
    /// [test_solidangle halfwaypoint]
}

//------------------------------------------------------------------------------
void stratifiedsampling(const tc::LogContext& logContext)
{
    /// [test_solidangle stratifiedsampling]

    // Test stratified sampling
    tc::Matrix<float> frame(tc::Vector3<float>(1.0f, 0.0f, 0.0f, 0.0f),
                            tc::Vector3<float>(0.0f, 1.0f, 0.0f, 0.0f),
                            tc::Vector3<float>(0.0f, 0.0f, 1.0f, 0.0f),
                            tc::Vector3<float>(0.0f, 0.0f, 0.0f, 1.0f));

    const size_t pitchSamples = 1;
    const size_t yawSamples = pitchSamples * 4;

    const tc::Vector3<float> d0 =
        tc::generateStratifiedDirection<tc::halfwayPoint>(1.0f, yawSamples,
                                                          pitchSamples, 0, 0)
            .transform(frame.m_x, frame.m_y, frame.m_z);

    const tc::Vector3<float> d1 =
        tc::generateStratifiedDirection<tc::halfwayPoint>(1.0f, yawSamples,
                                                          pitchSamples, 1, 0)
            .transform(frame.m_x, frame.m_y, frame.m_z);

    const tc::Vector3<float> d2 =
        tc::generateStratifiedDirection<tc::halfwayPoint>(1.0f, yawSamples,
                                                          pitchSamples, 2, 0)
            .transform(frame.m_x, frame.m_y, frame.m_z);

    const tc::Vector3<float> d3 =
        tc::generateStratifiedDirection<tc::halfwayPoint>(1.0f, yawSamples,
                                                          pitchSamples, 3, 0)
            .transform(frame.m_x, frame.m_y, frame.m_z);

    TC_IS(logContext, d0.equals(tc::Vector3<float>(0.5f, 0.70717f, 0.5f)));
    TC_IS(logContext, d1.equals(tc::Vector3<float>(0.5f, 0.70717f, -0.5f)));
    TC_IS(logContext, d2.equals(tc::Vector3<float>(-0.5f, 0.70717f, -0.5f)));
    TC_IS(logContext, d3.equals(tc::Vector3<float>(-0.5f, 0.70717f, 0.5f)));

    /// [test_solidangle stratifiedsampling]
}

//------------------------------------------------------------------------------
class LightCurve
{
public:
    LightCurve(const float position, const float range):
        m_gradient(range/0.5f),
        m_position(position),
        m_range(range)
    {
    }


    float operator()(const float x) const
    {
        const float i = x-m_position;

        // If we're inside of the working range for this triangle then
        // compute it, otherwise return 0.0f.
        if(i >= 0.0f && i < m_range)
        {
            // Going up
            const float mid = m_range / 2.0f;
            if(i < mid)
            {
                const float y = m_gradient*i;
                return y;
            }
            // Going down
            else
            {
                const float y = 1.0f - (m_gradient*(i-mid));
                return y;
            }
        }
        return 0.0f;
    }

    const float m_gradient;
    const float m_position;
    const float m_range;
};

//------------------------------------------------------------------------------
void importance_sampling_experiment(const tc::LogContext& logContext)
{
    LightCurve fcurve(1.0f, 1.0f);

    const size_t NUM_SAMPLES = 4;
    const float range=2.0f;

    const float step =
            static_cast<float>(range)/static_cast<float>(NUM_SAMPLES);

    // Monte Carlo Estimator no importance sampling.
    float mc_no_is_estimate=0.0f;
    for(size_t i=0; i != NUM_SAMPLES; ++i)
    {
#if 1
        const float x = static_cast<float>(i)*step;
        mc_no_is_estimate += fcurve(x)*step;
#else
        const float x = tc::generateRandomFloat(0.0f, range);
        mc_no_is_estimate += fcurve(x)*step;
#endif
    }

    logContext.log("Uniformly Distributed Random Variables:");
    logContext.log(mc_no_is_estimate);
    logContext.log("\n");

#if 0
    // Monte Carlo Estimator with importance sampling.
    float mc_is_estimate=0.0f;
    for(size_t i=0; i != NUM_SAMPLES; ++i)
    {
        const float x = static_cast<float>(i)*step;
        mc_is_estimate += fcurve(x)*step;
    }

    logContext.log("Importance Sampled Random Variables:");
    logContext.log(mc_is_estimate);
    logContext.log("\n");
#endif
}

//------------------------------------------------------------------------------
const float importance_sampling_weight(const float x, const float offset,
                                       const float radius, const float strength)
{
    // Only blend if we are in the blending range.
    const float t = x-offset;
    if(t < -radius || t > radius)
    {
        return 0.0f;
    }
    const float weight = (cos(((x-offset)/radius)*M_PI)+1.0f)/2.0f;
    const float weight_scaled = pow(weight, 1.0f/strength); // Square root, cube root, etc
    return weight_scaled;
}

//------------------------------------------------------------------------------
const float importance_sampling_fcurve(const float x, const float target,
                                       const float radius,
                                       const float strength)
{
    // Smooth interpolation curve. (sin(((x*(pi*2))-(pi/2)))+1)/2
    // Smooth interpolation curve. (cos((x*pi*2)-pi)+1)/2
    // Smooth interpolation curve, parameterized. (cos(((x-offset)/scale)*pi)+1)/2
    const float weight =
            importance_sampling_weight(x, target, radius, strength);
    const float blend = (target*weight) + (x*(1.0f-weight));
    return blend;
}

//------------------------------------------------------------------------------
void importance_sampling_weighted_curve(const tc::LogContext& logContext)
{
    const float target = 3.0f;
    const float radius = 3.0f;
    const float reach = target + radius;

    const size_t NUM_SAMPLES = 5;
    const float initial_range=4.0f;
    const float range = std::max(initial_range, reach);

    const float step =
            static_cast<float>(range)/static_cast<float>(NUM_SAMPLES);

    const float light_range = 1.0f;
    const float strength = 10.0f;

#define USE_IMPORTANCE_SAMPLING

#if 0
    // Monte Carlo Estimator no importance sampling.
    logContext.log("Points\n");
    for(size_t i=0; i != NUM_SAMPLES; ++i)
    {
        const float x = static_cast<float>(i)*step;
        logContext.log(x);
        logContext.log("\n");
    }

    // Monte Carlo Estimator no importance sampling.
    logContext.log("Curved Points\n");
    for(size_t i=0; i != NUM_SAMPLES; ++i)
    {
        const float x = static_cast<float>(i)*step;
        const float y = importance_sampling_fcurve(x, target, radius, strength);
        logContext.log(y);
        logContext.log("\n");
    }

    // Weight
    logContext.log("Weights\n");
    for(size_t i=0; i != NUM_SAMPLES; ++i)
    {
        const float x = static_cast<float>(i)*step;
        const float y = importance_sampling_weight(x, target, radius, strength);
        logContext.log(y);
        logContext.log("\n");
    }
#endif

    LightCurve lightA(target-(light_range/2.0f), light_range);

#if 0
    logContext.log("---------------------------------------\n");
    size_t resolution = 20;
    // The radiance curve of the light source.
    for(size_t i=0; i != resolution; ++i)
    {
        const float j = 4.0f / static_cast<float>(resolution);
        const float x = static_cast<float>(i)*j;
        logContext.log(x);
        logContext.log("\n");
    }

    // The radiance curve of the light source.
    for(size_t i=0; i != resolution; ++i)
    {
        const float j = 4.0f / static_cast<float>(resolution);
        const float x = static_cast<float>(i)*j;
        const float y = light(x);
        logContext.log(y);
        logContext.log("\n");
    }
    logContext.log("---------------------------------------\n");
#endif

    // Accurate integral
    float integral = 0.0f;
    const float samples = 10.0f;
    for(size_t i=0; i != samples; ++i)
    {
        const float width = 4.0f / static_cast<float>(samples);
        const float x = static_cast<float>(i)*width;
        const float height = lightA(x);
        integral += height*width;
    }
    logContext.log("Integral: ");
    logContext.log(integral);
    logContext.log("\n");

    // Monte Carlo Estimator with importance sampling.
    logContext.log("Final Result\n");
    float barWidth = step;

    const float first_weighted_x = importance_sampling_fcurve(0.0f, target,
                                                              radius, strength);

    logContext.log(first_weighted_x);
    logContext.log("\n");

    float result = lightA(first_weighted_x)*barWidth;
    float totalWidths = 0.0f;
    for(size_t i=1; i < NUM_SAMPLES; ++i)
    {
        float x = static_cast<float>(i)*step;
        const float weighted_x = importance_sampling_fcurve(x, target, radius,
                                                            strength);
        float j = static_cast<float>(i-1)*step;
        const float previous_weighted_x =
                importance_sampling_fcurve(j, target, radius, strength);
        barWidth = weighted_x-previous_weighted_x;

        result += lightA(weighted_x)*barWidth;
        totalWidths += barWidth;

        logContext.log(weighted_x);
        logContext.log("\n");
    }

    logContext.log("Result: ");
    logContext.log(result);
    logContext.log("\n");
}

}  // namespace

//------------------------------------------------------------------------------
void tc::solidangleRunUnitTests(const tc::LogContext& logContext)
{
    sphericaltocartesian(logContext);
    halfwaypoint(logContext);
    stratifiedsampling(logContext);
    importance_sampling_experiment(logContext);
    importance_sampling_weighted_curve(logContext);
}
