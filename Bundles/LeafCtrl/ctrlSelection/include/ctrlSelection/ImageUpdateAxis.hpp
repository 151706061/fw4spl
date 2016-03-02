/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __CTRLSELECTION_IMAGEUPDATEAXIS_HPP__
#define __CTRLSELECTION_IMAGEUPDATEAXIS_HPP__

#include <fwServices/IService.hpp>
#include <fwServices/IController.hpp>

#include <fwComEd/helper/MedicalImageAdaptor.hpp>

#include "ctrlSelection/config.hpp"

namespace ctrlSelection
{


/**
 * @class  ImageUpdateAxis
 * @brief  This service updates a fwData::Float given in configuration with the image axis.
 */
class CTRLSELECTION_CLASS_API ImageUpdateAxis : public ::fwServices::IController
{

public:

    fwCoreServiceClassDefinitionsMacro ( (ImageUpdateAxis)(::fwServices::IController) );

    CTRLSELECTION_API ImageUpdateAxis() throw();

    CTRLSELECTION_API virtual ~ImageUpdateAxis() throw();

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Image::s_SLICE_TYPE_MODIFIED_SIG to this::s_UPDATE_SLICE_TYPE_SLOT
     */
    CTRLSELECTION_API virtual KeyConnectionsType getObjSrvConnections() const;

protected:

    /// Calls updating().
    CTRLSELECTION_API virtual void starting()  throw ( ::fwTools::Failed );

    /// Does nothing.
    CTRLSELECTION_API virtual void stopping()  throw ( ::fwTools::Failed );

    /// Does nothing.
    CTRLSELECTION_API virtual void swapping()  throw ( ::fwTools::Failed );

    /**
     * @brief Configures the service.
     *
     * @code{.xml}
       <service type="::fwServices::IController" impl="::ctrlSelection::ImageUpdateAxis" autoConnect="yes">
           <axis uid="imageAxis" orientation="axial" />
       </service>
       @endcode
     * - \b uid : fwID of the ::fwData::Float to update
     * - \b orientation : image orientation (axial, frontal or sagittal)
     */
    CTRLSELECTION_API virtual void configuring()  throw ( ::fwTools::Failed );

    /// Does nothing.
    CTRLSELECTION_API virtual void reconfiguring()  throw ( ::fwTools::Failed );

    /// Updates the float from the image axis orientation.
    CTRLSELECTION_API virtual void updating() throw ( ::fwTools::Failed );

    /// Implements info method derived from IService. Print classname.
    CTRLSELECTION_API virtual void info( std::ostream &_sstream );

private:

    /**
     * @name Slots
     * @{
     */
    /// Slot: update image slice type
    void updateSliceType(int from, int to);
    /**
     * @}
     */

    /// fwID of the float to update.
    std::string m_floatID;

    /// Orientation of the image axis.
    ::fwComEd::helper::MedicalImageAdaptor::Orientation m_orientation;
};

} // ctrlSelection

#endif // __CTRLSELECTION_IMAGEUPDATEAXIS_HPP__
