//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SHADESHACK
#define TC_SHADESHACK
//------------------------------------------------------------------------------
#include "trace/constvector.h"
#include "trace/ray.h"
#include "trace/traceResult.h"
#include "trace/sampledspectrum.h"
//------------------------------------------------------------------------------

namespace tc
{

#define TC_SHADING_DONE (1 << 31)

//------------------------------------------------------------------------------
// ShadeStackFrame
//------------------------------------------------------------------------------
/// \brief A single frame of the tc::ShadeStack.
///
/// A frame in the tc::ShadeStack represents a point in the scene that is
/// currently being shaded.
//------------------------------------------------------------------------------
class ShadeStackFrame
{
public:
    /// Where we are with computing the shading value of this frame. If this
    /// point requires additional rays to be traced to compute an estimate for
    /// its irradiance, then the value of m_i will be in the range of 0 to
    /// numSamples, where numSamples is the number of samples that will be
    /// traced for it. When m_i == numSamples we are ready to compute the final
    /// irradiance value.
    unsigned int m_i;
    /// The aggregate result of radiance values that contribute to this point's
    /// irradiance.
    SampledSpectrum m_radianceSum;
    /// The ray this point is on.
    const Ray m_ray;
    /// The result of the intersection of m_ray with the geometry in the scene.
    const TraceResult m_traceResult;

    bool done() const
    {
        return m_i & TC_SHADING_DONE;
    }

    void setDone()
    {
        m_i |= TC_SHADING_DONE;
    }

    unsigned int getI() const
    {
        return m_i & ~TC_SHADING_DONE;
    }

    void incrI()
    {
        assert(!done());
        ++m_i;
    }

    /// \brief Initialises a tc::ShadeStackFrame.
    ///
    /// \param ray The ray this point is on.
    /// \param traceResult The result of the intersection of ray with the
    /// geometry in the scene.
    ShadeStackFrame(const Ray& ray, const TraceResult& traceResult)
        : m_i(0), m_ray(ray), m_traceResult(traceResult)
    {
    }

    ShadeStackFrame(): m_i(0)
    {
    }

    ShadeStackFrame(const ShadeStackFrame& shadeStackFrame)
        : m_i(shadeStackFrame.m_i),
          m_radianceSum(shadeStackFrame.m_radianceSum),
          m_ray(shadeStackFrame.m_ray),
          m_traceResult(shadeStackFrame.m_traceResult)
    {
    }
};

//------------------------------------------------------------------------------
// ShadeStack
//------------------------------------------------------------------------------
/// \brief The ShadeStack keeps track of shader dependencies.
/// It is a tc::ConstVector of ShadeStackFrame instances. Each frame in the
/// stack represents a three dimensional point in the scene that references a
/// shader. The ShadeStack is used internally for processing the radiance tree.
//------------------------------------------------------------------------------
class ShadeStack : public ConstVector<ShadeStackFrame>
{
};

}  // namespace tc
#endif  // TC_SHADESHACK
