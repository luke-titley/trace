//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_SAMPLED_SPECTRUM
#define TC_SAMPLED_SPECTRUM
//------------------------------------------------------------------------------
#include "trace/vector.h"
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// TC_SAMPLED_SPECTRUM_SAMPLE_COUNT
//------------------------------------------------------------------------------
enum
{
    /// \brief The number of samples stored in the SampledSpectrum class.
    TC_SAMPLED_SPECTRUM_SAMPLE_COUNT = 3
};

//------------------------------------------------------------------------------
// SampledSpectrum
//------------------------------------------------------------------------------
/// \brief Contains an array of electromagnetic wavelength intensity values.
///
/// This is the class we use for storing the 'color' of a point, albeit we don't
/// actually store a color. Instead we store an array of values, where each
/// entry represents the intensity of a particular wavelength. The
/// SampledSpectrum value can be converted to an RGB colour value for displaying
/// and for storing in RGB image formats.
//------------------------------------------------------------------------------
class SampledSpectrum
{
public:
    typedef Vector3<float, TC_SAMPLED_SPECTRUM_SAMPLE_COUNT> Wavelengths;

    /// \brief Initialises a SampledSpectrum class instance.
    inline SampledSpectrum();

    /// \brief Initialises a SampledSpectrum class instance.
    /// \param wavelengths a vector of wavelength values, to use to populate
    /// this SampledSpectrum instance.
    inline explicit SampledSpectrum(const Wavelengths& wavelengths);

    /// \brief Initialises a SampledSpectrum class instance.
    /// \param allWavelengths An intensity value to assign to every
    /// wavelength.
    inline SampledSpectrum(const float allWavelengths);

    /// \brief adds the wavelenth values in rhs to our wavelength values.
    inline SampledSpectrum operator+(const SampledSpectrum& rhs) const;
    /// \brief subtracts the wavelenth values in rhs from our wavelength
    /// values.
    inline SampledSpectrum operator-(const SampledSpectrum& rhs) const;
    /// \brief divides the wavelength values in this instance by the
    /// wavelength values in rhs.
    inline SampledSpectrum operator/(const SampledSpectrum& rhs) const;
    /// \brief multiplies the wavelength values in this instance by the
    /// wavelength values in rhs.
    inline SampledSpectrum operator*(const SampledSpectrum& rhs) const;

    /// \brief increments our wavelenth values by those in rhs.
    inline const SampledSpectrum& operator+=(const SampledSpectrum& rhs);
    /// \brief decrincrements our wavelenth values by those in rhs.
    inline const SampledSpectrum& operator-=(const SampledSpectrum& rhs);
    inline const SampledSpectrum& operator/=(const SampledSpectrum& rhs);
    inline const SampledSpectrum& operator*=(const SampledSpectrum& rhs);

    /// \return The intensity value for the given wavelength.
    inline float getWavelength(const size_t wavelength) const;

private:
    Wavelengths m_wavelengths;
};

//------------------------------------------------------------------------------
SampledSpectrum::SampledSpectrum() : m_wavelengths(0.0f)
{
}

//------------------------------------------------------------------------------
SampledSpectrum::SampledSpectrum(const float allWavelengths)
    : m_wavelengths(allWavelengths)
{
}

//------------------------------------------------------------------------------
SampledSpectrum::SampledSpectrum(const Wavelengths& wavelengths)
    : m_wavelengths(wavelengths)
{
}

//------------------------------------------------------------------------------
SampledSpectrum SampledSpectrum::operator+(const SampledSpectrum& rhs) const
{
    const SampledSpectrum result(m_wavelengths + rhs.m_wavelengths);
    return result;
}

//------------------------------------------------------------------------------
SampledSpectrum SampledSpectrum::operator-(const SampledSpectrum& rhs) const
{
    const SampledSpectrum result(m_wavelengths - rhs.m_wavelengths);
    return result;
}

//------------------------------------------------------------------------------
SampledSpectrum SampledSpectrum::operator/(const SampledSpectrum& rhs) const
{
    const SampledSpectrum result(m_wavelengths / rhs.m_wavelengths);
    return result;
}

//------------------------------------------------------------------------------
SampledSpectrum SampledSpectrum::operator*(const SampledSpectrum& rhs) const
{
    const SampledSpectrum result(m_wavelengths * rhs.m_wavelengths);
    return result;
}

//------------------------------------------------------------------------------
const SampledSpectrum& SampledSpectrum::operator+=(const SampledSpectrum& rhs)
{
    m_wavelengths += rhs.m_wavelengths;
    return *this;
}

//------------------------------------------------------------------------------
const SampledSpectrum& SampledSpectrum::operator-=(const SampledSpectrum& rhs)
{
    m_wavelengths -= rhs.m_wavelengths;
    return *this;
}

//------------------------------------------------------------------------------
const SampledSpectrum& SampledSpectrum::operator/=(const SampledSpectrum& rhs)
{
    m_wavelengths /= rhs.m_wavelengths;
    return *this;
}

//------------------------------------------------------------------------------
const SampledSpectrum& SampledSpectrum::operator*=(const SampledSpectrum& rhs)
{
    m_wavelengths *= rhs.m_wavelengths;
    return *this;
}

//------------------------------------------------------------------------------
float SampledSpectrum::getWavelength(const size_t wavelength) const
{
    return m_wavelengths[wavelength];
}

}  // namespace tc

#endif  // TC_SAMPLED_SPECTRUM
