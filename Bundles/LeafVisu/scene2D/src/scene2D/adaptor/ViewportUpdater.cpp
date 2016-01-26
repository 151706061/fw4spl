/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "scene2D/adaptor/ViewportUpdater.hpp"
#include "scene2D/data/Viewport.hpp"
#include "scene2D/Scene2DGraphicsView.hpp"

#include <fwServices/Base.hpp>


fwServicesRegisterMacro( ::scene2D::adaptor::IAdaptor, ::scene2D::adaptor::ViewportUpdater, ::scene2D::data::Viewport);

namespace scene2D
{
namespace adaptor
{


ViewportUpdater::ViewportUpdater() throw()
{
}

//-----------------------------------------------------------------------------

ViewportUpdater::~ViewportUpdater() throw()
{
}

//-----------------------------------------------------------------------------

void ViewportUpdater::configuring() throw( ::fwTools::Failed)
{
    this->IAdaptor::configuring();

    doUpdate();
}

//-----------------------------------------------------------------------------

void ViewportUpdater::doStart() throw( ::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void ViewportUpdater::doStop() throw( ::fwTools::Failed)
{
    SLM_TRACE_FUNC();
}

//-----------------------------------------------------------------------------

void ViewportUpdater::doUpdate() throw( ::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    ::scene2D::data::Viewport::sptr sceneViewport  = this->getScene2DRender()->getViewport();
    ::scene2D::data::Viewport::sptr viewportObject = this->getObject< ::scene2D::data::Viewport>();

    std::pair< double, double > pairCoord = this->mapAdaptorToScene(
        std::pair< double, double >( viewportObject->getX(), viewportObject->getY() ), m_xAxis, m_yAxis );

    std::pair< double, double > pairSize = this->mapAdaptorToScene(
        std::pair< double, double >( viewportObject->getWidth(), viewportObject->getHeight() ), m_xAxis, m_yAxis );

    sceneViewport->setX( pairCoord.first );
    sceneViewport->setY( pairCoord.second );
    sceneViewport->setWidth( pairSize.first );
    sceneViewport->setHeight( viewportObject->getHeight() );

    this->getScene2DRender()->getView()->updateFromViewport();
}

//-----------------------------------------------------------------------------

void ViewportUpdater::doSwap() throw( ::fwTools::Failed)
{
}

//-----------------------------------------------------------------------------

void ViewportUpdater::processInteraction( ::scene2D::data::Event::sptr _event )
{

}

}   // namespace adaptor
}   // namespace scene2D

