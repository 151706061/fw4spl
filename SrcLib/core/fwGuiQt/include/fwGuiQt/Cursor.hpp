/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQT_CURSOR_HPP__
#define __FWGUIQT_CURSOR_HPP__

#include <fwCore/base.hpp>

#include <fwGui/ICursor.hpp>

#include "fwGuiQt/config.hpp"

namespace fwGuiQt
{

/**
 * @brief   Defines the generic cursor for IHM.
 * @class   Cursor
 *
 * @date    2009-2010.
 *
 */
class FWGUIQT_CLASS_API Cursor : public ::fwGui::ICursor
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (Cursor)(::fwGui::ICursor),
                                            (()),
                                            ::fwGui::factory::New< Cursor > );

    FWGUIQT_API Cursor(::fwGui::GuiBaseObject::Key key);

    FWGUIQT_API virtual ~Cursor();

    /// Set the cursor
    FWGUIQT_API virtual void setCursor( ::fwGui::ICursor::CursorType cursor);

    /// Set the default cursor
    FWGUIQT_API virtual void setDefaultCursor();

};

} // namespace fwGuiQt

#endif /*__FWGUIQT_CURSOR_HPP__*/


