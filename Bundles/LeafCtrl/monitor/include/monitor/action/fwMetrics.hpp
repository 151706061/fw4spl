/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __MONITOR_ACTION_FWMETRICS_HPP__
#define __MONITOR_ACTION_FWMETRICS_HPP__

#include <fwGui/IActionSrv.hpp>

#include "monitor/config.hpp"

namespace monitor
{

namespace action
{
/// Implements an action for show metrics about f4s factories (number of data, message and service in factories).
class MONITOR_CLASS_API fwMetrics : public ::fwGui::IActionSrv
{

public:

    fwCoreServiceClassDefinitionsMacro ( (fwMetrics)( ::fwGui::IActionSrv ) );

    /// Does nothing
    MONITOR_API fwMetrics() noexcept;

    /// Does nothing
    MONITOR_API virtual ~fwMetrics() noexcept;

protected:

    /// Calls classic IAction methods to configure
    void configuring();

    /// Calls classic IAction methods to start
    void starting();

    /// Show metrics in a dialog message box
    void updating();

    /// Calls classic IAction methods to stop
    void stopping();
};

} // namespace action

} // namespace monitor

#endif //__MONITOR_ACTION_FWMETRICS_HPP__

