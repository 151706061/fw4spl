/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GUIQT_EDITOR_SSIGNALBUTTON_HPP__
#define __GUIQT_EDITOR_SSIGNALBUTTON_HPP__

#include "guiQt/config.hpp"

#include <fwTools/Failed.hpp>

#include <gui/editor/IEditor.hpp>

#include <QPointer>
#include <QPushButton>

namespace guiQt
{

namespace editor
{


/**
 * @brief   This editor shows a button and send a signal when it is clicked.
 * @class   SSignalButton
 *
 * @note If the mode is "checkable", it sends a signal with a boolean in parameter representing the "check" mode.
 */
class GUIQT_CLASS_API SSignalButton : public QObject,
                                      public ::gui::editor::IEditor
{
Q_OBJECT
public:


    fwCoreServiceClassDefinitionsMacro ( (SSignalButton)(::gui::editor::IEditor) );

    /// Constructor. Do nothing.
    GUIQT_API SSignalButton() throw();

    /// Destructor. Do nothing.
    GUIQT_API virtual ~SSignalButton() throw();

protected:

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */
    /**
     * @brief This method launches the IEditor::starting method.
     */
    GUIQT_API virtual void starting() throw( ::fwTools::Failed );

    /**
     * @brief This method launches the IEditor::stopping method.
     */
    GUIQT_API virtual void stopping() throw( ::fwTools::Failed );

    /**
     * @brief This method is used to update services. Do nothing.
     */
    GUIQT_API virtual void updating() throw(::fwTools::Failed);

    /**
     * @brief This method is used to configure the class parameters.
     * @verbatim
       <service uid="..." impl="::guiQt::editor::SSignalButton" >
        <config>
            <checkable>true|false</checkable>
            <text>...</text>
            <icon>...</icon>
            <text2>...</text2>
            <icon2>...</icon2>
            <checked>true|false</checked>
            <iconWidth>...</iconWidth>
            <iconHeight>...</iconHeight>
        </config>
       </service>
       @endverbatim
     * - \b text (optional): text displayed on the button
     * - \b icon (optional): icon displayed on the button
     * - \b checkable (optional, default: false): if true, the button is checkable
     *   - \b text2 (optional): text displayed if the button is checked
     *   - \b icon2 (optional): icon displayed if the button is checked
     *   - \b checked (optional, default: false): if true, the button is checked at start
     * - iconWidth (optional): icon width
     * - iconHeight (optional): icon height
     */
    GUIQT_API virtual void configuring() throw( ::fwTools::Failed );

    /**
     * @brief This method is used to give information about the service. Do nothing.
     */
    GUIQT_API virtual void info(std::ostream &_sstream );

    ///@}

private Q_SLOTS:
    void onClicked();
    void onToggled(bool);

private:

    /**
     * @name Signal
     * @{
     */
    /// Signal emitted when button is clicked
    typedef ::fwCom::Signal< void () > ClickedSignalType;
    ClickedSignalType::sptr m_sigClicked;

    /// Signal emitted when button is checked/unchecked
    typedef ::fwCom::Signal< void (bool) > ToggledSignalType;
    ToggledSignalType::sptr m_sigToggled;
    /**
     * @}
     */

    /**
     * @name Slots
     * @{
     */
    /// Slot: check/uncheck the button
    void setChecked(bool checked);
    /**
     * @}
     */

    QPointer <QPushButton> m_button; /// button

    std::string m_text; /// Button's text
    std::string m_text2; /// Button's text when it is checked
    ::boost::filesystem::path m_icon; /// Path of the button's icon
    ::boost::filesystem::path m_icon2; /// Path of the button's icon displayed when it is checked
    bool m_checkable; /// If true, the button is checkable
    bool m_checkAtStart; /// If true, the button is checked at start
    unsigned int m_iconWidth; /// icon width
    unsigned int m_iconHeight; /// icon height
};

}
}

#endif /*__GUIQT_EDITOR_SSIGNALBUTTON_HPP__*/


