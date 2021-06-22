//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#ifndef TC_ARGS
#define TC_ARGS
//------------------------------------------------------------------------------
#include <cstring>
#include <cstdlib>
//------------------------------------------------------------------------------
namespace tc
{

//------------------------------------------------------------------------------
// Args
//------------------------------------------------------------------------------
/// \brief given a string as a 'const char *', returns 'true' if the string
/// contains onl digits, and false if it contains characters other than digits.
//------------------------------------------------------------------------------
bool isNumber(const char* value)
{
    for (const char* i = value; *i != '\0'; ++i)
    {
        bool digit = *i >= '0' && *i <= '9';
        if (!digit)
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
// Args
//------------------------------------------------------------------------------
/// \brief Parses 'argv' and stores the result. Stores default values for
/// options omitted from 'argv'.
///
/// Arguments can be added with 'hasFlag' 'getArg' and 'getArgFloat'.
///
/// Example:
/// \code
///	tc::Arg arg(argc, argv);
///	if(arg.render)
///	{
///		// Render something
///	}
/// \endcode
//------------------------------------------------------------------------------
class Args
{
private:
    inline bool hasFlag(const char* flag, const int argc,
                        const char* argv[]) const
    {
        for (int i = 0; i != argc; ++i)
        {
            if (strcmp(argv[i], flag) == 0)
            {
                return true;
            }
        }
        return false;
    }
    inline size_t getArg(const char* flag, const size_t defaultValue,
                         const int argc, const char* argv[]) const
    {
        for (int i = 0; i != argc; ++i)
        {
            if (strcmp(argv[i], flag) == 0)
            {
                const int i_plus_one = i + 1;
                if (i_plus_one != argc)
                {
                    const char* value = argv[i_plus_one];
                    if (isNumber(value))
                    {
                        int intValue = atoi(value);
                        return static_cast<size_t>(intValue);
                    }
                }
            }
        }
        return defaultValue;
    }
    inline const char* getArg(const char* flag, const char* defaultValue,
                              const int argc, const char* argv[]) const
    {
        for (int i = 0; i != argc; ++i)
        {
            if (strcmp(argv[i], flag) == 0)
            {
                const int i_plus_one = i + 1;
                if (i_plus_one != argc)
                {
                    const char* value = argv[i_plus_one];
                    return value;
                }
            }
        }
        return defaultValue;
    }

    inline float getArgFloat(const char* flag, const float defaultValue,
                             const int argc, const char* argv[]) const
    {
        for (int i = 0; i != argc; ++i)
        {
            if (strcmp(argv[i], flag) == 0)
            {
                const int i_plus_one = i + 1;
                if (i_plus_one != argc)
                {
                    const char* value = argv[i_plus_one];
                    char* endptr = 0;
                    double valueD = strtod(value, &endptr);
                    if (value != endptr)  // This is how we check for errors
                    {
                        float valueF = static_cast<float>(valueD);
                        return valueF;
                    }
                }
            }
        }
        return defaultValue;
    }

public:
    /// Should the unit tests be run ?
    const bool runUnitTests;
    /// Should we render ?
    const bool render;
    /// Should rendering progress be written to standard output whilst
    /// rendering?
    const bool reportProgress;
    const size_t qualityLevel;
    /// The number of samples to use when computing the final colour value of a
    /// pixel. When supersampling, the values are averaged to produce a good
    /// result.
    const size_t samplesPerPixel;
    /// The upper limit for the amount of bounced rays to use.
    const size_t maxRayDepth;
    /// The number of threads to involve in the rendering operation.
    /// If this is set to 0, then the number of threads will be chosen by the
    /// computer.
    const size_t threadCount;
    /// The amount of time in seconds to wait before a progress report is given.
    const size_t secondsBetweenProgressReport;
    /// The width of the final rendered image.
    const size_t width;
    /// The height of the final rendered image.
    const size_t height;
    /// The filename of the image file to save to.
    const char* outputFilename;
    /// The filename of the scenee file to read from..
    const char* inputFilename;

    /// \brief Initialises the 'Args' class bry parsing argvh.
    inline Args(const int argc, const char* argv[])
        : runUnitTests(hasFlag("--runUnitTests", argc, argv)),
          render(hasFlag("--render", argc, argv)),
          reportProgress(hasFlag("--reportProgress", argc, argv)),
          qualityLevel(getArg("--qualityLevel", 1, argc, argv)),
          samplesPerPixel(getArg("--samplesPerPixel", 1, argc, argv)),
          maxRayDepth(getArg("--maxRayDepth", 2, argc, argv)),
          threadCount(getArg("--threadCount", (size_t)0, argc, argv)),
          secondsBetweenProgressReport(
              getArg("--secondsBetweenProgressReport", (size_t)0, argc, argv)),
          width(getArg("--width", 256, argc, argv)),
          height(getArg("--height", 256, argc, argv)),
          outputFilename(getArg("--outputFilename", "out.png", argc, argv)),
          inputFilename(getArg("--inputFilename", "in.lsd", argc, argv))
    {
    }
};

}  // namespace tc
#endif  // TC_ARGS
