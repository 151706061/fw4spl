/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIMEDDATAQT_PCH_HPP__
#define __UIMEDDATAQT_PCH_HPP__

// Cause an internal compiler error on windows...
#ifndef _WIN32
#include <fwCom/HasSlots.hpp>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>
#endif // _WIN32

#include <fwData/Reconstruction.hpp>
#include <fwData/TransformationMatrix3D.hpp>

#include <fwGui/IActionSrv.hpp>

#include <fwMedData/Equipment.hpp>
#include <fwMedData/Patient.hpp>
#include <fwMedData/Series.hpp>
#include <fwMedData/Study.hpp>

#include <gui/editor/IEditor.hpp>

#include <QPointer>
#include <QWidget>

#endif // __UIMEDDATAQT_PCH_HPP__
