/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_SDISTANCE_HPP__
#define __VISUVTKADAPTOR_SDISTANCE_HPP__

#ifndef ANDROID

#include "visuVTKAdaptor/config.hpp"

#include <fwData/Color.hpp>
#include <fwData/Point.hpp>

#include <fwRenderVTK/IAdaptor.hpp>

#include <vector>

class vtkDistanceRepresentation2D;
class vtkActor;
class vtkLineSource;

namespace visuVTKAdaptor
{

/**
 * @brief Display the distance between the two points of the point list
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service type="::namespace::classname">
       <in key="pointList" uid="..." />
       <config renderer="default" />
   </service>
   @endcode
 * @subsection Input Input
 * - \b pointList [::fwData::PointList]: point list containing two points.
 * @subsection Configuration Configuration
 * - \b config(mandatory) : contains the adaptor configuration
 *    - \b renderer (mandatory): defines the renderer to show the distance.
 */
class VISUVTKADAPTOR_CLASS_API SDistance : public ::fwRenderVTK::IAdaptor
{

public:

    fwCoreServiceClassDefinitionsMacro( (SDistance)(::fwRenderVTK::IAdaptor) );

    VISUVTKADAPTOR_API SDistance() noexcept;

    VISUVTKADAPTOR_API virtual ~SDistance() noexcept;

    static const ::fwServices::IService::KeyType s_POINTLIST_INPUT;

    /// set Distance Axis color AND alpha
    VISUVTKADAPTOR_API void setAxisColor( ::fwData::Color::sptr newColor) noexcept;

protected:

    VISUVTKADAPTOR_API void configuring();
    VISUVTKADAPTOR_API void starting();
    VISUVTKADAPTOR_API void updating();
    VISUVTKADAPTOR_API void stopping();

private:

    vtkDistanceRepresentation2D* m_distanceRepresentation;

    vtkActor* m_lineActor;
    vtkLineSource* m_lineSource;

    ::fwData::Point::cwptr m_point1;
    ::fwData::Point::cwptr m_point2;

    /// Connection between point 1 modified and this service reveive
    ::fwCom::Connection m_point1Connection;

    /// Connection between point 2 modified and this service reveive
    ::fwCom::Connection m_point2Connection;
};

} //namespace visuVTKAdaptor

#endif // ANDROID

#endif // __VISUVTKADAPTOR_SDISTANCE_HPP__
