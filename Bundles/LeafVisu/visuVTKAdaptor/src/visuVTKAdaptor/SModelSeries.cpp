/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SModelSeries.hpp"

#include "visuVTKAdaptor/SMesh.hpp"
#include "visuVTKAdaptor/SReconstruction.hpp"
#include "visuVTKAdaptor/STexture.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwData/Boolean.hpp>
#include <fwData/Material.hpp>
#include <fwData/Mesh.hpp>
#include <fwData/Reconstruction.hpp>

#include <fwMedData/ModelSeries.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/op/Add.hpp>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>

fwServicesRegisterMacro( ::fwRenderVTK::IAdaptor, ::visuVTKAdaptor::SModelSeries, ::fwMedData::ModelSeries );

namespace visuVTKAdaptor
{

const ::fwCom::Signals::SignalKeyType SModelSeries::s_TEXTURE_APPLIED_SIG = "textureApplied";

const ::fwCom::Slots::SlotKeyType SModelSeries::s_UPDATE_NORMAL_MODE_SLOT   = "updateNormalMode";
const ::fwCom::Slots::SlotKeyType SModelSeries::s_SHOW_RECONSTRUCTIONS_SLOT = "showReconstructions";

static const ::fwServices::IService::KeyType s_MODEL_INPUT = "model";

//------------------------------------------------------------------------------

SModelSeries::SModelSeries() noexcept :
    m_autoResetCamera(true)
{
    m_clippingPlanes = "";

    m_sigTextureApplied = newSignal<TextureAppliedSignalType>(s_TEXTURE_APPLIED_SIG);

    newSlot(s_UPDATE_NORMAL_MODE_SLOT, &SModelSeries::updateNormalMode, this);
    newSlot(s_SHOW_RECONSTRUCTIONS_SLOT, &SModelSeries::showReconstructions, this);
}

//------------------------------------------------------------------------------

SModelSeries::~SModelSeries() noexcept
{
}

//------------------------------------------------------------------------------

void SModelSeries::configuring()
{
    this->configureParams();

    const ConfigType config = this->getConfigTree().get_child("config.<xmlattr>");

    this->setClippingPlanes( config.get<std::string>("clippingplanes", "") );

    const std::string autoresetcamera = config.get<std::string>("autoresetcamera", "yes");
    SLM_ASSERT("'autoresetcamera' must be 'yes' or 'no'", autoresetcamera == "yes" || autoresetcamera == "no");
    m_autoResetCamera = (autoresetcamera == "yes");

    if (config.count("texture"))
    {
        SLM_FATAL("'texture' is deprecated, you need to connect manually the SModelSeries::textureApplied signal to "
                  "the STexture::applyTexture slot.");
    }
}

//------------------------------------------------------------------------------

void SModelSeries::starting()
{
    this->initialize();
    this->updating();
}

//------------------------------------------------------------------------------

void SModelSeries::updating()
{
    ::fwMedData::ModelSeries::csptr modelSeries = this->getInput< ::fwMedData::ModelSeries >(s_MODEL_INPUT);
    SLM_ASSERT("Missing ModelSeries '" + s_MODEL_INPUT + "'", modelSeries);

    this->stopping();

    ::fwData::Boolean::csptr showField = modelSeries->getField< ::fwData::Boolean >("ShowReconstructions");
    bool showRec = true;
    if (showField && showField->getValue() == false)
    {
        showRec = false;
    }

    for( ::fwData::Reconstruction::sptr reconstruction :  modelSeries->getReconstructionDB() )
    {
        // create the srv configuration for objects auto-connection
        auto adaptor = registerService< ::visuVTKAdaptor::SReconstruction >("::visuVTKAdaptor::SReconstruction");
        adaptor->registerInput(reconstruction, SReconstruction::s_RECONSTRUCTION_INPUT, true);

        adaptor->setTransformId( this->getTransformId() );
        adaptor->setRendererId( this->getRendererId() );
        adaptor->setPickerId( this->getPickerId() );
        adaptor->setRenderService(this->getRenderService());
        adaptor->setAutoRender( this->getAutoRender() );

        adaptor->setClippingPlanes( m_clippingPlanes );
        adaptor->setAutoResetCamera(m_autoResetCamera);
        adaptor->start();
        adaptor->setForceHide( !showRec );

        m_sigTextureApplied->asyncEmit(reconstruction->getMaterial());
    }
    this->requestRender();
}

//------------------------------------------------------------------------------

void SModelSeries::stopping()
{
    m_connections.disconnect();
    this->unregisterServices();
}

//------------------------------------------------------------------------------

void SModelSeries::updateNormalMode(std::uint8_t mode, std::string recID)
{
    auto subServices = this->getRegisteredServices();
    for( auto& wService :  subServices)
    {
        auto service = wService.lock();
        if(service)
        {
            auto reconstructionAdaptor = ::visuVTKAdaptor::SReconstruction::dynamicCast(service);
            if (reconstructionAdaptor && reconstructionAdaptor->getObject()->getID() == recID)
            {
                reconstructionAdaptor->updateNormalMode(mode);
                break;
            }
        }
    }
}

//------------------------------------------------------------------------------

void SModelSeries::showReconstructions(bool show)
{
    ::fwMedData::ModelSeries::csptr modelSeries = this->getInput< ::fwMedData::ModelSeries >(s_MODEL_INPUT);
    SLM_ASSERT("Missing ModelSeries '" + s_MODEL_INPUT + "'", modelSeries);

    auto subServices = this->getRegisteredServices();
    for( auto& wService :  subServices)
    {
        auto service = wService.lock();
        if(service)
        {
            auto reconstructionAdaptor = ::visuVTKAdaptor::SReconstruction::dynamicCast(service);
            if (reconstructionAdaptor)
            {
                reconstructionAdaptor->setForceHide( !show );
            }
        }
    }
    this->setVtkPipelineModified();
    this->requestRender();
}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsMap SModelSeries::getAutoConnections() const
{
    KeyConnectionsMap connections;
    connections.push(s_MODEL_INPUT, ::fwMedData::ModelSeries::s_MODIFIED_SIG, s_UPDATE_SLOT);
    connections.push(s_MODEL_INPUT, ::fwMedData::ModelSeries::s_RECONSTRUCTIONS_ADDED_SIG, s_UPDATE_SLOT);
    connections.push(s_MODEL_INPUT, ::fwMedData::ModelSeries::s_RECONSTRUCTIONS_REMOVED_SIG, s_UPDATE_SLOT);

    return connections;
}

//------------------------------------------------------------------------------

} //namespace visuVTKAdaptor
