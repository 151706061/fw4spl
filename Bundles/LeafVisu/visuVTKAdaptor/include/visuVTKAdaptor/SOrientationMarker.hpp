/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_SORIENTATIONMARKER_HPP__
#define __VISUVTKADAPTOR_SORIENTATIONMARKER_HPP__

#ifndef ANDROID

#include "visuVTKAdaptor/config.hpp"

#include <fwRenderVTK/IAdaptor.hpp>

namespace visuVTKAdaptor
{

/**
 * @brief Adaptor used to display an orientationMarker in a generic scene
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service type="::visuVTKAdaptor::SOrientationMarker">
       <config renderer="default" hAlign="left">
   </service>
   @endcode
 * @subsection Configuration Configuration
 * - \b config(mandatory) : contains the adaptor configuration
 *    - \b renderer(mandatory) : defines the renderer to show the point list.
 *    - \b hAlign(optional, default: 'left') : horizontal alignment (left or right)
 */
class VISUVTKADAPTOR_CLASS_API SOrientationMarker : public ::fwRenderVTK::IAdaptor
{

public:

    fwCoreServiceClassDefinitionsMacro( (SOrientationMarker)(::fwRenderVTK::IAdaptor) );
    VISUVTKADAPTOR_API SOrientationMarker();

protected:

    VISUVTKADAPTOR_API void configuring();
    VISUVTKADAPTOR_API void starting();

    //------------------------------------------------------------------------------

    void updating()
    {
    }

    void stopping();

    /// Horizontal alignment
    std::string m_hAlign;
};
} //namespace visuVTKAdaptor

#endif // ANDROID

#endif //__VISUVTKADAPTOR_SORIENTATIONMARKER_HPP__
