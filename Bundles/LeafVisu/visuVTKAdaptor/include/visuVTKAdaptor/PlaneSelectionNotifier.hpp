/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */


#ifndef __VISUVTKADAPTOR_PLANESELECTIONNOTIFIER_HPP__
#define __VISUVTKADAPTOR_PLANESELECTIONNOTIFIER_HPP__

#ifndef ANDROID

#include "visuVTKAdaptor/config.hpp"

#include <fwCom/Connection.hpp>
#include <fwCom/Slot.hpp>
#include <fwCom/Slots.hpp>

#include <fwData/Composite.hpp>
#include <fwData/Object.hpp>
#include <fwData/PlaneList.hpp>

#include <fwRenderVTK/IVtkAdaptorService.hpp>

#include <fwServices/helper/SigSlotConnection.hpp>

namespace visuVTKAdaptor
{

/**
 * @brief Update selection plan in resection activity
 */
class VISUVTKADAPTOR_CLASS_API PlaneSelectionNotifier : public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (PlaneSelectionNotifier)(::fwRenderVTK::IVtkAdaptorService) );

    VISUVTKADAPTOR_API PlaneSelectionNotifier() throw();

    VISUVTKADAPTOR_API virtual ~PlaneSelectionNotifier() throw();

    void setPlaneListId(const std::string& id)
    {
        m_planeListId = id;
    }

    void setPlaneSelectionId(const std::string& id)
    {
        m_planeSelectionId = id;
    }

    /**
     * @brief Slot: adds/swaps the plane is the composite.
     *
     * @note It can be connected to ::visuVTKAdaptor::PlaneList's "selected" signal.
     */
    VISUVTKADAPTOR_API void selectPlane( ::fwData::Object::sptr plane );

    VISUVTKADAPTOR_API void deselectPlane();

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Composite::s_ADDED_OBJECTS_SIG to this::s_UPDATE_PLANELIST_SLOT
     * Connect Composite::s_CHANGED_OBJECTS_SIG to this::s_UPDATE_PLANELIST_SLOT
     * Connect Composite::s_REMOVED_OBJECTS_SIG to this::s_UPDATE_PLANELIST_SLOT
     */
    VISUVTKADAPTOR_API virtual KeyConnectionsType getObjSrvConnections() const;

protected:

    typedef std::map < std::string, ::fwCom::Connection > ConnetionMapType;

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);

    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);

    std::string m_planeListId;
    std::string m_planeSelectionId;

    ::fwData::PlaneList::wptr m_currentPlaneList;

    /// Register connection between planelist and this adaptor
    ::fwServices::helper::SigSlotConnection::sptr m_plConnection;

    /// Register connection between plane and this service
    ConnetionMapType m_planeConnections;

private:

    /**
     * @name Slots
     * @{
     */
    /// If selected==false, the planes are deselected.
    void updateSelection(bool selected);

    /// Adds plane
    void addPlane(::fwData::Plane::sptr plane);

    /// Removes plane
    void removePlane(::fwData::Plane::sptr plane);

    /// Show/hide planes
    void showPlanes(bool visible);

    /// Slot: update scene if planeList changed
    void updatePlaneList(::fwData::Composite::ContainerType objects);

    typedef fwCom::Slot <void (::fwData::Composite::ContainerType)> UpdatePlaneListSlotType;
    UpdatePlaneListSlotType::sptr m_slotUpdatePlaneList;
    /**
     * @}
     */

};


} //namespace visuVTKAdaptor

#endif // ANDROID

#endif // __VISUVTKADAPTOR_PLANESELECTIONNOTIFIER_HPP__
