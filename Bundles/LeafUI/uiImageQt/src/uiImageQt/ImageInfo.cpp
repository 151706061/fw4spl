/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "uiImageQt/ImageInfo.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwComEd/fieldHelper/MedicalImageHelpers.hpp>
#include <fwComEd/helper/Image.hpp>

#include <fwCore/base.hpp>

#include <fwData/Image.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include <fwMath/IntrasecTypes.hpp>

#include <fwServices/Base.hpp>
#include <fwServices/IService.hpp>
#include <fwServices/registry/ObjectService.hpp>

#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>

namespace uiImage
{

fwServicesRegisterMacro( ::gui::editor::IEditor, ::uiImage::ImageInfo, ::fwData::Image );

static const ::fwCom::Slots::SlotKeyType s_GET_INTERACTION_SLOT = "getInteraction";

ImageInfo::ImageInfo() throw()
{
    newSlot(s_GET_INTERACTION_SLOT, &ImageInfo::getInteraction, this);
}

//------------------------------------------------------------------------------

ImageInfo::~ImageInfo() throw()
{
}

//------------------------------------------------------------------------------

void ImageInfo::starting() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->::fwGui::IGuiContainerSrv::create();
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();

    ::fwGuiQt::container::QtContainer::sptr qtContainer = ::fwGuiQt::container::QtContainer::dynamicCast(
        this->getContainer() );
    QWidget* const container = qtContainer->getQtContainer();
    SLM_ASSERT("container not instanced", container);

    QHBoxLayout* hLayout = new QHBoxLayout();

    QLabel* staticText = new QLabel( QObject::tr("intensity:"), container);
    hLayout->addWidget( staticText, 0, Qt::AlignVCenter );

    m_valueText = new QLineEdit( container );
    m_valueText->setReadOnly(true);
    hLayout->addWidget( m_valueText, 1, Qt::AlignVCenter );

    container->setLayout( hLayout );
}

//------------------------------------------------------------------------------

void ImageInfo::stopping() throw(::fwTools::Failed)
{
    SLM_TRACE_FUNC();

    this->getContainer()->clean();
    this->::fwGui::IGuiContainerSrv::destroy();
}

//------------------------------------------------------------------------------

void ImageInfo::configuring() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();
    this->::fwGui::IGuiContainerSrv::initialize();
}

//------------------------------------------------------------------------------

void ImageInfo::updating() throw(::fwTools::Failed)
{
}

//------------------------------------------------------------------------------

void ImageInfo::swapping() throw(::fwTools::Failed)
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    bool imageIsValid = ::fwComEd::fieldHelper::MedicalImageHelpers::checkImageValidity( image );
    m_valueText->setEnabled(imageIsValid);
}

//------------------------------------------------------------------------------

void ImageInfo::getInteraction(::fwComEd::PickingInfo info)
{
    SLM_TRACE_FUNC();

    if (info.m_eventId == ::fwComEd::PickingInfo::Event::MOUSE_MOVE)
    {
        ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
        bool imageIsValid = ::fwComEd::fieldHelper::MedicalImageHelpers::checkImageValidity( image );
        m_valueText->setEnabled(imageIsValid);
        if (imageIsValid)
        {
            double *point = info.m_worldPos;

            ::fwComEd::helper::Image imageHelper(image);

            std::string intensity = imageHelper.getPixelAsString(point[0], point[1], point[2] );
            m_valueText->setText(QString::fromStdString(intensity));
        }
    }
}

//------------------------------------------------------------------------------

void ImageInfo::info( std::ostream &_sstream )
{
    _sstream << "Image Info Editor";
}

//------------------------------------------------------------------------------
}

