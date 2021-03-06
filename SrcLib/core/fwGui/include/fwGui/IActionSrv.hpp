/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUI_IACTIONSRV_HPP__
#define __FWGUI_IACTIONSRV_HPP__

#include "fwGui/config.hpp"
#include "fwGui/registrar/ActionRegistrar.hpp"

#include <fwCom/Slots.hpp>

#include <fwServices/IService.hpp>

namespace fwGui
{

/**
 * @brief   Defines the service interface managing the menu items.
 * @class   IActionSrv
 *
 */
class FWGUI_CLASS_API IActionSrv : public ::fwServices::IService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (IActionSrv)(::fwServices::IService) );
    fwCoreAllowSharedFromThis();

    /**
     * @name Slots Keys
     * @{
     */
    /// Slot to activate/deactivate the action
    static const ::fwCom::Slots::SlotKeyType s_SET_IS_ACTIVE_SLOT;

    /// Slot to activate the action
    static const ::fwCom::Slots::SlotKeyType s_ACTIVATE_SLOT;

    /// Slot to desactivate the action
    static const ::fwCom::Slots::SlotKeyType s_DEACTIVATE_SLOT;

    /// Slot to enable/disable the action
    static const ::fwCom::Slots::SlotKeyType s_SET_IS_EXECUTABLE_SLOT;

    /// Slot to enable the action
    static const ::fwCom::Slots::SlotKeyType s_SET_EXECUTABLE_SLOT;

    /// Slot to disable the action
    static const ::fwCom::Slots::SlotKeyType s_SET_INEXECUTABLE_SLOT;
    /**
     * @}
     */

    /// Slot to show/hide the action
    typedef ::fwCom::Slot< void (bool) >  SetVisibleSlotType;
    static const ::fwCom::Slots::SlotKeyType s_SET_VISIBLE_SLOT;

    /// Slot to disable the action
    typedef ::fwCom::Slot< void () >  ShowSlotType;
    static const ::fwCom::Slots::SlotKeyType s_SHOW_SLOT;

    /// Slot to disable the action
    typedef ::fwCom::Slot< void () >  HideSlotType;
    static const ::fwCom::Slots::SlotKeyType s_HIDE_SLOT;

    /**
     * @}
     */

    /// Method called when the action service is stopping
    FWGUI_API void actionServiceStopping();

    /// Method called when the action service is starting
    FWGUI_API void actionServiceStarting();

    /// Set the action service is active/inactive.
    FWGUI_API virtual void setIsActive(bool isActive);

    /// Set the action service is active.
    FWGUI_API virtual void activate();

    /// Set the action service is not active.
    FWGUI_API virtual void deactivate();

    /// Return true if action service is active.
    FWGUI_API bool getIsActive();

    /// Return true if action 'check' feedback is inverted.
    bool getActiveStateValue()
    {
        return m_activeStateValue;
    }

    /// Set the action service executable or not.
    FWGUI_API virtual void setIsExecutable(bool isExecutable);

    /// Set the action service executable.
    FWGUI_API virtual void setExecutable();

    /// Set the action service not executable.
    FWGUI_API virtual void setInexecutable();

    /// Return action service is executable.
    FWGUI_API bool getIsExecutable();

    /// Show or hide the action.
    FWGUI_API void setVisible(bool isVisible);

    /// Show the action.
    FWGUI_API void show();

    /// Hide the action.
    FWGUI_API void hide();

    FWGUI_API bool isVisible();

    /**
     * @brief Confirm that the action must be executed.
     *
     * If action is configured to be confirmed : show a dialog box to confirm execution.
     * Else return true
     *
     * @return true if user click on 'yes' button.
     */
    FWGUI_API bool confirmAction();

protected:

    FWGUI_API IActionSrv();

    FWGUI_API virtual ~IActionSrv();

    typedef ::fwRuntime::ConfigurationElement::sptr ConfigurationType;

    /**
     * @brief Initialize the action.
     *
     * Example of configuration
     * @code{.xml}
           <service uid="item" type="::fwGui::IActionSrv" impl="::gui::action::SDoNothing" autoConnect="no" >
              <state active="false" executable="false" inverse="true" visible="true" />
              <confirmation value="yes" message="..." />
           </service>
       @endcode
     * - \<state active="false" executable="false" /\> : fix the state of the action in the menu and Toolbar.
     *   - \b executable not mandatory (Default value true ) : allows to enable/disable the execution of the action.
     *     If the action appears in the interface it will be enable/disable.
     *   - \b active not mandatory (Default value false ):
     *     If the action appears in the interface it will be check/uncheck.
     *   - \b visible not mandatory (Default value true ):
     *     If true, the action is visible in the interface (and if the action is associated to a menu and/or a toolbar).
     *   - \b inverse not mandatory (Default value true) : allow to invert the state of the action (if "check")
     * - \<confirmation value="yes" message="..." /\> : configure if the action must be confirm by user before to execute it.
     *   - \b value : if true the action will show a confirmation message before to be executed.
     *   - \b message not mandatory : if not empty the message is shown in dialog box.
     *   - \b defaultbutton not mandatory (default defined by underlying gui backend) :
     *                      specify the dialog default button (yes or no)
     */
    FWGUI_API void initialize();


private:

    ::fwGui::registrar::ActionRegistrar::sptr m_registrar;

    /// Value for which the action will be considered 'active'
    bool m_activeStateValue;
    bool m_isActive;
    bool m_isExecutable;
    bool m_isVisible;
    bool m_confirmAction;
    std::string m_confirmMessage;
    std::string m_defaultButton;
};

//-----------------------------------------------------------------------------

class LockAction
{
public:
    LockAction(IActionSrv::wptr action) : m_action(action)
    {
        m_action.lock()->setIsExecutable(false);
    }
    ~LockAction()
    {
        m_action.lock()->setIsExecutable(true);
    }
private:
    IActionSrv::wptr m_action;
};

//-----------------------------------------------------------------------------

} // namespace fwGui

#endif /*__FWGUI_IACTIONSRV_HPP__*/


