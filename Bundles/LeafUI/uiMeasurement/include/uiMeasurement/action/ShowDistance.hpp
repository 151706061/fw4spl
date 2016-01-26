/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIMEASUREMENT_ACTION_SHOWDISTANCE_HPP__
#define __UIMEASUREMENT_ACTION_SHOWDISTANCE_HPP__

#include <fwGui/IActionSrv.hpp>
#include <fwData/Image.hpp>
#include "uiMeasurement/config.hpp"

namespace uiMeasurement
{
namespace action
{

/**
 * @brief   This action allows to show/hide image distances.
 * @class   ShowDistance
 *
 * @date    2010.
 */
class UIMEASUREMENT_CLASS_API ShowDistance : public ::fwGui::IActionSrv
{

public:
    fwCoreServiceClassDefinitionsMacro ( (ShowDistance)( ::fwGui::IActionSrv) );

    UIMEASUREMENT_API ShowDistance() throw();

    UIMEASUREMENT_API virtual ~ShowDistance() throw();

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Image::s_DISTANCE_DISPLAYED_SIG to this::s_SHOW_DISTANCE_SLOT
     */
    UIMEASUREMENT_API virtual KeyConnectionsType getObjSrvConnections() const;

protected:


    void swapping() throw(::fwTools::Failed);

    void configuring() throw (::fwTools::Failed);

    void starting() throw (::fwTools::Failed);

    void updating() throw (::fwTools::Failed);

    void stopping() throw (::fwTools::Failed);

    void info(std::ostream &_sstream );


private:

    /// Slot: show/hide distance
    void showDistance(bool isShown);

};

} // namespace action

} // namespace uiMeasurement

#endif // __UIMEASUREMENT_ACTION_SHOWDISTANCE_HPP__
