/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "scene2D/Scene2DGraphicsView.hpp"
#include "scene2D/adaptor/HistogramCursor.hpp"
#include "scene2D/data/InitQtPen.hpp"

#include <fwServices/macros.hpp>

#include <fwData/Histogram.hpp>
#include <fwData/Point.hpp>

#include <QGraphicsEllipseItem>
#include <QFont>


fwServicesRegisterMacro( ::scene2D::adaptor::IAdaptor, ::scene2D::adaptor::HistogramCursor, ::fwData::Histogram);

namespace scene2D
{
namespace adaptor
{

HistogramCursor::HistogramCursor() throw() :
    m_color("red"), m_borderColor(Qt::gray), m_opacity(0.8f), m_index(nullptr), m_pointSize(6.f), m_layer(nullptr)
{
}

//---------------------------------------------------------------------------------------------------------------

HistogramCursor::~HistogramCursor() throw()
{
}

//---------------------------------------------------------------------------------------------------------------

void HistogramCursor::configuring() throw( ::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    this->IAdaptor::configuring();

    if (!m_configuration->getAttributeValue("color").empty())
    {
        ::scene2D::data::InitQtPen::setPenColor(m_color, m_configuration->getAttributeValue("color"));
    }

    if (!m_configuration->getAttributeValue("opacity").empty())
    {
        m_opacity = ::boost::lexical_cast< float >( m_configuration->getAttributeValue("opacity") );
    }

    if (!m_configuration->getAttributeValue("borderColor").empty())
    {
        ::scene2D::data::InitQtPen::setPenColor(m_borderColor, m_configuration->getAttributeValue("borderColor"));
    }

    if (!m_configuration->getAttributeValue("pointSize").empty())
    {
        m_pointSize = ::boost::lexical_cast< float >( m_configuration->getAttributeValue("pointSize") );
    }

    SLM_ASSERT("A viewport attribute must be specified with 'viewportUID'.",
               !m_configuration->getAttributeValue("viewportUID").empty());

    if( !m_configuration->getAttributeValue("viewportUID").empty() )
    {
        m_viewportID = m_configuration->getAttributeValue("viewportUID");
    }

    SLM_ASSERT("'histogramPointUID' attribute is missing.", !m_configuration->getAttributeValue(
                   "histogramPointUID").empty());
    m_histogramPointUID = m_configuration->getAttributeValue("histogramPointUID");
}

//---------------------------------------------------------------------------------------------------------------

void HistogramCursor::doStart() throw( ::fwTools::Failed)
{
    m_index = new QGraphicsEllipseItem();
    m_index->setBrush( m_color.color() );
    m_index->setPen( m_borderColor );
    m_index->setZValue(m_zValue);
    m_index->setCacheMode( QGraphicsItem::DeviceCoordinateCache );
    m_color.setCosmetic(true);

    // Initialize the layer
    m_layer = new QGraphicsItemGroup();

    m_layer->addToGroup( m_index );

    // Set the layer position (according to the related axis) and zValue
    m_layer->setPos(m_xAxis->getOrigin(), m_yAxis->getOrigin());
    m_layer->setZValue(m_zValue);

    m_viewport = this->getSafeInOut< ::scene2D::data::Viewport>( m_viewportID );

    m_connection = m_viewport->signal(::fwData::Object::s_MODIFIED_SIG)->connect(
        this->slot(::fwServices::IService::s_UPDATE_SLOT));

    // Add the layer containing grid's lines to the scene
    this->getScene2DRender()->getScene()->addItem(m_layer);
}

//---------------------------------------------------------------------------------------------------------------

void HistogramCursor::doStop() throw( ::fwTools::Failed)
{
    m_connection.disconnect();
}

//---------------------------------------------------------------------------------------------------------------

void HistogramCursor::doUpdate() throw( ::fwTools::Failed)
{
    this->initializeViewSize();
    this->initializeViewportSize();

    ::fwData::Histogram::sptr histogram           = this->getObject< ::fwData::Histogram>();
    ::fwData::Histogram::fwHistogramValues values = histogram->getValues();
    const float histogramMinValue  = histogram->getMinValue();
    const float histogramBinsWidth = histogram->getBinsWidth();

    // Event coordinates in scene
    ::scene2D::data::Coord sceneCoord = this->getScene2DRender()->mapToScene( m_coord );

    int histIndex = (int) sceneCoord.getX();
    int index     = (histIndex - histogramMinValue) / histogramBinsWidth;

    if(index >= 0 && index < (int)values.size()) // avoid std out_of_range on MS Windows
    {
        ::scene2D::data::Viewport::sptr viewport = this->getScene2DRender()->getViewport();
        const double viewportHeight = viewport->getHeight();
        const double viewportWidth  = viewport->getWidth();

        const double viewportSizeRatio    = viewportHeight / viewportWidth;
        const double viewInitialSizeRatio = m_viewInitialSize.first / m_viewInitialSize.second;

        const Scene2DRatio ratio   = this->getRatio(); // Total ratio
        const double viewportRatio = this->getViewportSizeRatio().first;

        double diameterH = m_pointSize;
        double diameterV = m_pointSize * viewportSizeRatio;

        diameterV /= viewportRatio;
        diameterV *= viewInitialSizeRatio;

        // Apply the ratio of the scene 2D in order to keep the same size for the circles if viewport's size or
        // view's size change:
        diameterH *= ratio.first;
        diameterV *= ratio.second;

        ::fwData::Point::sptr point = this->getSafeInOut< ::fwData::Point>( m_histogramPointUID );

        double x = point->getRefCoord()[0] - diameterH / 2;
        double y = point->getRefCoord()[1] - diameterV / 2;

        m_index->setRect( x, y, diameterH, diameterV );
    }
}

//---------------------------------------------------------------------------------------------------------------

void HistogramCursor::doSwap() throw( ::fwTools::Failed)
{
}

//---------------------------------------------------------------------------------------------------------------

void HistogramCursor::processInteraction( ::scene2D::data::Event::sptr _event )
{
    this->initializeViewSize();
    this->initializeViewportSize();

    if( _event->getType() == ::scene2D::data::Event::MouseMove )
    {
        m_coord = _event->getCoord();
    }

    doUpdate();
}

//---------------------------------------------------------------------------------------------------------------

}   // namespace adaptor
}   // namespace scene2D
