/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIVISU_ACTION_SSNAPSHOT_HPP__
#define __UIVISU_ACTION_SSNAPSHOT_HPP__

#include "uiVisu/config.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signals.hpp>

#include <fwGui/IActionSrv.hpp>

#include <fwTools/Failed.hpp>

#include <string>
#include <map>



namespace uiVisu
{
namespace action
{

/**
 * @brief   This action allows to snapshot a generic scene
 * @class   SSnapshot
 *
 * Send a 'snapped' signal containing the filename used to save the snapshot.
 * @note You need to connect the 'snapped' signal to one visuVTKAdaptor::Snapshot to save the file.
 */
class UIVISU_CLASS_API SSnapshot : public ::fwGui::IActionSrv
{

public:

    fwCoreServiceClassDefinitionsMacro ( (SSnapshot)(::fwGui::IActionSrv) );

    /// Constructor. Do nothing.
    UIVISU_API SSnapshot() throw();

    /// Destructor. Do nothing.
    UIVISU_API virtual ~SSnapshot() throw();

protected:

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    /// Starts action
    virtual void starting() throw(::fwTools::Failed);

    /// Stops action
    virtual void stopping() throw(::fwTools::Failed);

    /// Show a file dialog and notify the scene which must be printed.
    void updating() throw(::fwTools::Failed);

    /// Does nothing
    void swapping() throw(::fwTools::Failed);

    /**
     * @brief Configure action.
     * @verbatim
       <service type="::fwGui::IActionSrv" impl="::uiData::action::SSnapshot" autoConnect="no" />
       @endverbatim
     */
    void configuring() throw(fwTools::Failed);

    virtual void info( std::ostream &_sstream );


private:

    /**
     * @name Signals
     * @{
     */
    /// Type of signal to snap shot
    typedef ::fwCom::Signal< void (std::string) > SnappedSignalType;
    static const ::fwCom::Signals::SignalKeyType s_SNAPPED_SIG;

    SnappedSignalType::sptr m_sigSnapped; ///< snap shot signal
    /**
     * @}
     */

    std::string requestFileName();

};

} // action
} // uiVisu

#endif // __UIVISU_ACTION_SSNAPSHOT_HPP__


