//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
// This header file is for doxygen documentaton only.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \mainpage libtrace.
/// \section intro Introduction
/// libtrace is Luke Titley's hobby ray tracer.
/// \image html frontpage_image.jpg
/// \section lookingAtCode What to look at?
/// A good starting point for understanding the 20% most used part of the source
/// code is to look at the implementation of tc::shade::Integrator next in
/// shade.cpp. This manages a stack of shaders for computing the final colour
/// value of a pixel fragment.
///
/// After that it is worth looking at tc::RenderThreads run in
/// renderThreads.cpp and then the implementation of tc::KDTree.
//------------------------------------------------------------------------------
/// \namespace tc
/// \brief The all inclusive namespace for everything inside of trace.
//------------------------------------------------------------------------------
/// \namespace tc::shaders
/// \brief All shaders are defined inside of this namespace.
//------------------------------------------------------------------------------
/// \namespace tc::shade
/// \brief Not to be confused with tc::shaders. This namespace is for the core
/// shading functionality of trace. This is where the monte carlo estimator is
/// implemented.
