/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2016-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __ACTIVITIES_ACTION_SCREATEACTIVITY_HPP__
#define __ACTIVITIES_ACTION_SCREATEACTIVITY_HPP__

#include "activities/config.hpp"

#include <fwActivities/registry/Activities.hpp>
#include <fwActivities/registry/ActivityMsg.hpp>

#include <fwData/Vector.hpp>

#include <fwGui/IActionSrv.hpp>

namespace activities
{
namespace action
{

/**
 * @brief This action launchs an activity according to the given configuration
 *
 * This action works on a ::fwData::Vector. It proposes all the available activity according to the given configuration.
 * And then, it sends a signal with the activity identifier.
 *
 * It should work with the ::uiMedDataQt::editor::SActivityWizard that creates or updates the activitySeries.
 *
 * @section Signals Signals
 * - \b activityIDSelected(std::string) : This signal is emitted when the activity is selected, it
 *      contains all activity identifier. It should be connected to the slot 'createActivity' of the service
 *     'SActivityWizard'.
 * - \b activitySelected(::fwMedData::ActivitySeries::sptr) : This signal is emitted when the activity is selected in
 *      the current vector. It should be connected to the slot 'updateActivity' of the service 'SActivityWizard'.
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service uid="..." type="::activities::action::SCreateActivity" autoConnect="yes" >
       <!-- Filter mode 'include' allows all given activity id-s.
            Filter mode 'exclude' allows all activity id-s excepted given ones. -->
       <filter>
           <mode>include</mode>
           <id>2DVisualizationActivity</id>
           <id>3DVisualizationActivity</id>
           <id>VolumeRenderingActivity</id>
       </filter>
   </service>
   @endcode
 *
 * - \b filter (optional): it allows to filter the activity that can be proposed.
 *    - \b mode: 'include' or 'exclude'. Defines if the activities in the following list are proposed (include) or not
 *      (exclude).
 *    - \b id: id of the activity
 */
class ACTIVITIES_CLASS_API SCreateActivity : public ::fwGui::IActionSrv
{

public:

    fwCoreServiceClassDefinitionsMacro( (SCreateActivity)(::fwGui::IActionSrv) );

    /// Constructor. Do nothing.
    ACTIVITIES_API SCreateActivity() noexcept;

    /// Destructor. Do nothing.
    ACTIVITIES_API virtual ~SCreateActivity() noexcept;

    /**
     * @name Signals API
     * @{
     */
    ACTIVITIES_API static const ::fwCom::Signals::SignalKeyType s_ACTIVITY_SELECTED_SIG;
    typedef ::fwCom::Signal< void ( ::fwMedData::ActivitySeries::sptr ) > ActivitySelectedSignalType;

    ACTIVITIES_API static const ::fwCom::Signals::SignalKeyType s_ACTIVITY_ID_SELECTED_SIG;
    typedef ::fwCom::Signal< void (std::string) > ActivityIDSelectedSignalType;
    /// @}

protected:

    ///This method launches the IAction::starting method.
    virtual void starting();

    ///This method launches the IAction::stopping method.
    virtual void stopping();

    /**
     * @brief Show activity selector.
     */
    virtual void updating();

    /**
     * @brief Initialize the action.
     * @see fwGui::IActionSrv::initialize()
     */
    virtual void configuring();

    typedef std::vector< std::string > KeysType;

private:

    /**
     * @brief Slots to launch the given activitySeries.
     * @param activitySeries the activity is launched on this series.
     */
    void launchActivity(::fwMedData::ActivitySeries::sptr activitySeries);

    typedef ::fwActivities::registry::Activities::ActivitiesType ActivityInfoContainer;

    /// Show custom dialog box
    ::fwActivities::registry::ActivityInfo show( const ActivityInfoContainer& infos );

    /// Returns enabled activity infos according to activity filter.
    ActivityInfoContainer getEnabledActivities(const ActivityInfoContainer& infos);

    /**
     * @brief Filter mode : include or exclude activity configurations.
     * @note Allowed values : 'include' or 'exclude'
     */
    std::string m_filterMode;

    /// Id-s of activity configurations to be enabled or disabled, according to filter mode.
    KeysType m_keys;

    /// Signal emitted when activity is selected. Contains the activity identifier.
    ActivityIDSelectedSignalType::sptr m_sigActivityIDSelected;

    /// Signal emitted when activity is selected. Contains the activity.
    ActivitySelectedSignalType::sptr m_sigActivitySelected;
};

} // namespace action
} // namespace activities

#endif // __ACTIVITIES_ACTION_SCREATEACTIVITY_HPP__
