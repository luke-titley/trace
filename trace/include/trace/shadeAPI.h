//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SHADEAPI
#define TC_SHADEAPI
//------------------------------------------------------------------------------

namespace tc
{
class GeoID;
class Shader;
class SurfaceFrame;

//------------------------------------------------------------------------------
/// \brief An abstract interface for querying the surface shader for a given
/// item of geometry and for querying the surface frame for a given item of
/// geometry.
///
/// These methods are used in the core 'shade' function and so concrete
/// implementations of the ShadeAPI have to be super high performance.
///
/// This interface is intended to provide a small set of routines which are able
/// to query local geoemetry information.
//------------------------------------------------------------------------------
class ShadeAPI
{
public:
    /// Concrete implementations of tc::ShadeAPI have to implement this method.
    /// \param geoID A reference to the item of geoemetry being rendered. geoID
    /// contains a unique number for the object being rendered (ie polymesh) and
    /// a unique number for the sub-object being rendered (ie triangle).
    /// \return A tc::SurfaceFrame instance for the given item of geoemtry.
    /// The tc::SurfaceFrame contains three axis; normal, tangent and
    /// bi-tangent and is used for creating a hemisphere around a given point.
    virtual const SurfaceFrame& shade_getSurfaceFrame(const GeoID& geoID)
        const = 0;

    /// Concrete implementations of tc::ShadeAPI have to implement this method.
    /// \param geoID A reference to the item of geoemetry being rendered. geoID
    /// contains a unique number for the object being rendered (ie polymesh) and
    /// a unique number for the sub-object being rendered (ie triangle).
    /// \return A shader object. This is a concrete implementation of the
    /// tc::Shader interface, containing a 'shade' implementation.
    virtual const Shader& shade_getSurfaceShader(const GeoID& geoID) const = 0;
};

}  // namespace tc
#endif  // TC_SHADEAPI
