/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwJobs/exception/Waiting.hpp"
#include "fwJobs/config.hpp"

#include <fwCore/Exception.hpp>

namespace fwJobs
{

namespace exception
{

Waiting::Waiting ( const std::string &err ) : ::fwCore::Exception(err)
{
}

} // namespace exception

} // namespace fwJobs


