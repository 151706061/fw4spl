/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SImageText.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>
#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>
#include <fwDataTools/helper/Image.hpp>

#include <fwServices/macros.hpp>

#include <boost/format.hpp>

#include <vtkRenderer.h>
#include <vtkTextActor.h>

#include <sstream>

fwServicesRegisterMacro( ::fwRenderVTK::IAdaptor, ::visuVTKAdaptor::SImageText);

namespace visuVTKAdaptor
{

static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_INDEX_SLOT = "updateSliceIndex";

static const ::fwServices::IService::KeyType s_IMAGE_INOUT        = "image";
static const ::fwServices::IService::KeyType s_TF_SELECTION_INOUT = "tfSelection";

//-----------------------------------------------------------------------------

SImageText::SImageText() noexcept
{
    this->installTFSlots(this);
    newSlot(s_UPDATE_SLICE_INDEX_SLOT, &SImageText::updateSliceIndex, this);
}

//-----------------------------------------------------------------------------

SImageText::~SImageText() noexcept
{
}

//-----------------------------------------------------------------------------

void SImageText::starting()
{
    ::fwData::Composite::sptr tfSelection = this->getInOut< ::fwData::Composite>(s_TF_SELECTION_INOUT);
    this->setTransferFunctionSelection(tfSelection);

    this->SText::starting();

    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);

    this->updateImageInfos(image);
    this->updateTransferFunction(image);
    this->installTFConnections();
    this->updating();
}

//-----------------------------------------------------------------------------

void SImageText::stopping()
{
    this->removeTFConnections();
    this->SText::stopping();
}

//-----------------------------------------------------------------------------

void SImageText::configuring()
{
    this->SText::configuring();

    const ConfigType config = this->getConfigTree().get_child("config.<xmlattr>");

    this->setSelectedTFKey(config.get<std::string>("selectedTFKey", ""));
}

//-----------------------------------------------------------------------------

void SImageText::updating()
{
    std::stringstream ss;
    ::fwData::Image::sptr image = this->getInOut< ::fwData::Image >(s_IMAGE_INOUT);
    SLM_ASSERT("Missing image", image);

    if (::fwDataTools::fieldHelper::MedicalImageHelpers::checkImageValidity(image))
    {
        ::fwDataTools::helper::Image imageHelper(image);
        size_t axialIndex    = static_cast<size_t>(m_axialIndex->value());
        size_t frontalIndex  = static_cast<size_t>(m_frontalIndex->value());
        size_t sagittalIndex = static_cast<size_t>(m_sagittalIndex->value());

        double min = this->getLevel() - this->getWindow()/2.0;
        double max = this->getLevel() + this->getWindow()/2.0;

        double window = max - min;
        double level  = min + window*0.5;

        ss <<  ( ::boost::format("[% 3li,% 3li]") % min % max ) << std::endl;
        ss <<  ( ::boost::format("W:% 3lg L:% 3lg") % window % level ) << std::endl;
        ss <<  ( ::boost::format("(% 4li,% 4li,% 4li): %s") % sagittalIndex % frontalIndex % axialIndex %
                 imageHelper.getPixelAsString(sagittalIndex, frontalIndex, axialIndex ));
    }

    this->setText(ss.str());

    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

void SImageText::updateSliceIndex(int axial, int frontal, int sagittal)
{
    m_axialIndex->value()    = axial;
    m_frontalIndex->value()  = frontal;
    m_sagittalIndex->value() = sagittal;

    this->updating();
}

//------------------------------------------------------------------------------

void SImageText::updatingTFPoints()
{
    this->updating();
}

//------------------------------------------------------------------------------

void SImageText::updatingTFWindowing(double /*window*/, double /*level*/)
{
    this->updating();
}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsMap SImageText::getAutoConnections() const
{
    KeyConnectionsMap connections;
    connections.push(s_IMAGE_INOUT, ::fwData::Image::s_MODIFIED_SIG, s_UPDATE_SLOT);
    connections.push(s_IMAGE_INOUT, ::fwData::Image::s_SLICE_INDEX_MODIFIED_SIG, s_UPDATE_SLICE_INDEX_SLOT);
    connections.push(s_IMAGE_INOUT, ::fwData::Image::s_BUFFER_MODIFIED_SIG, s_UPDATE_SLOT);

    return connections;
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor
