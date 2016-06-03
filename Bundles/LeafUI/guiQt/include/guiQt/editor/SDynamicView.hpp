/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GUIQT_EDITOR_SDYNAMICVIEW_HPP__
#define __GUIQT_EDITOR_SDYNAMICVIEW_HPP__

#include "guiQt/config.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signals.hpp>
#include <fwCom/Slot.hpp>
#include <fwCom/Slots.hpp>

#include <fwActivities/registry/Activities.hpp>
#include <fwActivities/registry/ActivityMsg.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include <fwMedData/ActivitySeries.hpp>

#include <fwServices/IAppConfigManager.hpp>

#include <fwTools/Failed.hpp>

#include <gui/view/IView.hpp>

#include <QObject>
#include <QPointer>

#include <map>
#include <set>


class QTabWidget;
namespace fwData
{
class Composite;
}
namespace guiQt
{
namespace editor
{


/**
 * @brief   This editor manages tabs containing activities.
 *
 * This service should received signals containing ActivitySeries connected to the slot \b launchActivity. It will
 * launch the activity in a new tab.
 *
 * @note The same activitySeries cannot be launch in two different tabs.
 *
 * @section XML XML Configuration
 * @code{.xml}
   <service type="::gui::view::IView" impl="::guiQt::editor::SDynamicView" autoConnect="yes" >
     <mainActivity id="SDBActivity" closable="false" />
     <parameters>
         <parameter replace="SERIESDB" by="medicalData"  />
         <parameter replace="ICON_PATH" by="Bundles/media_0-1/icons/app.ico"  />
     </parameters>
   </service>
   @endcode
 * - \b mainActivity (optional): information about the main activity (first tab). The activity series will be generated.
 *   This activity must not have requirement.
 *   - \b id : identifier of the activity
 *   - \b closable (optional, default 'no') : defines if the user can close this tab.
 * - \b parameters (optional) : additional parameters used to launch the activities
 *    - \b parameter: defines a parameter
 *        - \b replace: name of the parameter as defined in the AppConfig
 *        - \b by: defines the string that will replace the parameter name. It should be a simple string (ex.
 *          frontal) or define a camp path (ex. \@values.myImage). The root object of the sesh@ path if the
 *          composite contained in the ActivitySeries.
 *
 * @section Slots Slots
 * - \b launchActivity( ::fwMedData::ActivitySeries::sptr ): This slot allows to create a tab with the given activity
 *   series.
 * - \b launchActivitySeries( ::fwMedData::Series::sptr ): This slot allows to create a tab with the given activity
 *   series.
 * - \b createTab( ::fwActivities::registry::ActivityMsg ): This slot allows to create a tab with the given activity
 *   information.
 *
 * @section Signal Signal
 * - \b activitySelected( ::fwData::object::sptr ): this signal is emitted when the current tab selection
 *   changed, it contains the associated ActivitySeries. The activity series is send as a ::fwData::Object in order to
 *   connect this signal to slots receiving a ::fwData::Object.
 */
class GUIQT_CLASS_API SDynamicView : public QObject,
                                     public ::gui::view::IView
{
Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro ( (SDynamicView)(::gui::view::IView) );

    /// Constructor. Do nothing.
    GUIQT_API SDynamicView() throw();

    /// Destructor. Do nothing.

    GUIQT_API virtual ~SDynamicView() throw();

    typedef ::fwCom::Signal< void ( ::fwData::Object::sptr ) > ActivitySelectedSignalType;
    GUIQT_API static const ::fwCom::Signals::SignalKeyType s_ACTIVITY_SELECTED_SIG;
    typedef ::fwCom::Signal< void () > NothingSelectedSignalType;
    GUIQT_API static const ::fwCom::Signals::SignalKeyType s_NOTHING_SELECTED_SIG;
protected:

    /**
     * @brief Install the container.
     */
    virtual void starting() throw(::fwTools::Failed);

    /**
     * @brief Destroy the container.
     */
    virtual void stopping() throw(::fwTools::Failed);

    /**
     * @brief Update
     *
     */
    virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief Swap
     *
     * Update the editors.
     */
    virtual void swapping() throw(::fwTools::Failed);

    /**
     * @brief Configure the view
     * @see fwGui::IGuiContainerSrv::initialize()
     */
    virtual void configuring() throw(fwTools::Failed);

    virtual void info( std::ostream &_sstream );

private:


    typedef std::set< std::string > ActivityIdType;
    typedef ::fwActivities::registry::ActivityAppConfig::ActivityAppConfigParamsType ParametersType;
    typedef ::fwActivities::registry::ActivityAppConfigParam ParameterType;
    typedef std::map< std::string, std::string> ReplaceMapType;

    struct SDynamicViewInfo
    {
        ::fwGuiQt::container::QtContainer::sptr container;
        ::fwServices::IAppConfigManager::sptr helper;
        std::string wid;
        std::string title;
        bool closable;
        std::string icon;
        std::string tooltip;
        std::string tabID;
        std::string viewConfigID;
        ReplaceMapType replaceMap;
        ::fwMedData::ActivitySeries::sptr activitySeries;
    };

    typedef std::map< QWidget*, SDynamicViewInfo > SDynamicViewInfoMapType;

    /**
     * @brief Launch tab
     */
    void launchTab(SDynamicViewInfo& info);

    /**
     * @brief Slot: Launch the given activity in a new tab.
     * @note The same activity series cannot be launch in two different tabs.
     */
    void launchActivity(::fwMedData::ActivitySeries::sptr activitySeries);

    /**
     * @brief Slot: Launch the given activity in a new tab.
     * @note The same activity series cannot be launch in two different tabs.
     */
    void launchActivitySeries(::fwMedData::Series::sptr series);

    /**
     * @brief Translate parameters from source object.
     *
     * Replace camp path (ex.@values.myParam.image) by the uid of the corresponding object.
     *
     * @param[in] sourceObj source object used to find sub-object form camp path
     * @param[in] parameters list of parameters to translate
     * @param[out] replaceMap map containing the translated parameter
     */
    void translateParameters( ::fwData::Object::sptr sourceObj, const ParametersType& parameters,
                              ReplaceMapType & replaceMap );

    /// launch a new tab according to the receiving msg
    void createTab(::fwActivities::registry::ActivityMsg info);

    /// Create the main activitySeries and launch the activity
    void buildMainActivity();

    /// Create view info from activitySeries
    SDynamicViewInfo createViewInfo(::fwMedData::ActivitySeries::sptr activitySeries);


    std::map< std::string, unsigned int > m_titleToCount;
    std::set< std::string > m_tabIDList;

    ActivityIdType m_activityIds;

    SDynamicViewInfoMapType m_dynamicInfoMap;
    bool m_dynamicConfigStartStop;

    ParametersType m_parameters;
    QPointer<QTabWidget> m_tabWidget;
    QPointer<QWidget> m_currentWidget;

    ActivitySelectedSignalType::sptr m_sigActivitySelected;
    NothingSelectedSignalType::sptr m_sigNothingSelected;

    typedef std::pair< std::string, bool > MainActivityInfo;
    MainActivityInfo m_mainActivityInfo; ///< Store main activityInformation <id, isClosable>

protected Q_SLOTS:

    void closeTab( int index, bool forceClose );

    void closeTabSignal(int index);

    void changedTab(int index);

};

} //namespace editor
} // guiQt

#endif /*__GUIQT_EDITOR_SDYNAMICVIEW_HPP__*/
