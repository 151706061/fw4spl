/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_SMODELSERIES_HPP__
#define __VISUVTKADAPTOR_SMODELSERIES_HPP__

#include "visuVTKAdaptor/config.hpp"

#include <fwRenderVTK/IAdaptor.hpp>

#include <vector>

namespace fwData
{
class Material;
}

namespace visuVTKAdaptor
{

/**
 * @brief This adaptor shows ModelSeries. Creates adaptors for each reconstruction in model.
 *
 * @section Signals Signals
 * - \b textureApplied(SPTR(::fwData::Material)) : emitted when a texture was applied.
 *
 * @section Slots Slots
 * - \b updateNormalMode(std::uint8_t) : updates the normal mode (0: hidden, 1: point normals, 2: cell normals)
 * - \b showReconstructions(bool) : shows/hides all the reconstructions
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service type="::visuVTKAdaptor::SModelSeries" autoConnect="yes">
       <in key="model" uid="..." />
       <config renderer="default" transform="..." picker="" />
   </service>
   @endcode
 * @subsection Input Input
 * - \b input [::fwMedData::ModelSeries]: model series to display.
 * @subsection Configuration Configuration
 * - \b config(mandatory) : contains the adaptor configuration
 *    - \b renderer(mandatory) : renderer where the reconstruction is displayed
 *    - \b picker(optional) : picker used to pick on the reconstruction
 *    - \b autoresetcamera(optional) : reset the camera point of view when the mesh is modified ("yes" or "no",
 *      default: "yes") .
 *    - \b clippingplanes(optional) : identifier of the vtk clipping plane object
 */
class VISUVTKADAPTOR_CLASS_API SModelSeries : public ::fwRenderVTK::IAdaptor
{

public:

    fwCoreServiceClassDefinitionsMacro( (SModelSeries)(::fwRenderVTK::IAdaptor) );

    /**
     * @name Signals API
     * @{
     */
    typedef ::fwCom::Signal< void ( SPTR(::fwData::Material) ) > TextureAppliedSignalType;
    VISUVTKADAPTOR_API static const ::fwCom::Signals::SignalKeyType s_TEXTURE_APPLIED_SIG;
    /** @} */

    /**
     * @name Slots API
     * @{
     */
    VISUVTKADAPTOR_API static const ::fwCom::Slots::SlotKeyType s_UPDATE_NORMAL_MODE_SLOT;
    VISUVTKADAPTOR_API static const ::fwCom::Slots::SlotKeyType s_SHOW_RECONSTRUCTIONS_SLOT;
    /**
     * @}
     */

    VISUVTKADAPTOR_API SModelSeries() noexcept;

    VISUVTKADAPTOR_API virtual ~SModelSeries() noexcept;

    //------------------------------------------------------------------------------

    void setClippingPlanes(::fwRenderVTK::SRender::VtkObjectIdType id)
    {
        m_clippingPlanes = id;
    }

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect ModelSeries::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     * Connect ModelSeries::s_RECONSTRUCTIONS_ADDED_SIG to this::s_UPDATE_SLOT
     * Connect ModelSeries::s_RECONSTRUCTIONS_REMOVED_SIG to this::s_UPDATE_SLOT
     */
    VISUVTKADAPTOR_API virtual KeyConnectionsMap getAutoConnections() const;

protected:

    VISUVTKADAPTOR_API void configuring();
    VISUVTKADAPTOR_API void starting();
    VISUVTKADAPTOR_API void updating();
    VISUVTKADAPTOR_API void stopping();

    /**
     * @name Slots methods
     * @{
     */

    /**
     * Slot: used to update normal display for the adaptor on object with givent uid 'recID'
     * (0: none, 1: point, 2: cell)
     */
    void updateNormalMode(std::uint8_t mode, std::string recID);

    /// Slot: show(or hide) reconstructions
    void showReconstructions(bool show);
    /**
     * @}
     */

private:

    ::fwRenderVTK::SRender::VtkObjectIdType m_clippingPlanes;
    bool m_autoResetCamera;

    /// Signal to emit when a texture must be applied on a material.
    TextureAppliedSignalType::sptr m_sigTextureApplied;
};

} //namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_SMODELSERIES_HPP__
