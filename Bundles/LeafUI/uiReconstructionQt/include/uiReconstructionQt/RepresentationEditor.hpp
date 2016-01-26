/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIRECONSTRUCTIONQT_REPRESENTATIONEDITOR_HPP__
#define __UIRECONSTRUCTIONQT_REPRESENTATIONEDITOR_HPP__

#include "uiReconstructionQt/config.hpp"

#include <gui/editor/IEditor.hpp>

#include <fwCom/Signal.hpp>
#include <fwCom/Signals.hpp>

#include <fwData/Material.hpp>

#include <fwTools/Failed.hpp>

#include <QObject>
#include <QPointer>


class QRadioButton;
class QCheckBox;
class QButtonGroup;
class QAbstractButton;

namespace uiReconstruction
{

/**
 * @brief   RepresentationEditor service.
 * @class   RepresentationEditor
 *
 * @date    2010.
 */
class UIRECONSTRUCTIONQT_CLASS_API RepresentationEditor : public QObject,
                                                          public ::gui::editor::IEditor
{
Q_OBJECT

public:

    fwCoreServiceClassDefinitionsMacro ( (RepresentationEditor)(::gui::editor::IEditor) );

    /// Constructor. Do nothing.
    UIRECONSTRUCTIONQT_API RepresentationEditor() throw();

    /// Destructor. Do nothing.
    UIRECONSTRUCTIONQT_API virtual ~RepresentationEditor() throw();

    /**
     * @name Signals API
     * @{
     */
    /// normals mode (0: none, 1: point, 2: cell), reconstruction fwID
    typedef ::fwCom::Signal< void ( std::uint8_t, std::string ) > NormalsModeModifiedSignalType;
    UIRECONSTRUCTIONQT_API static const ::fwCom::Signals::SignalKeyType s_NORMALS_MODE_MODIFIED_SIG;
    /** @} */

protected:


    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    ///This method launches the IEditor::starting method.
    virtual void starting() throw(::fwTools::Failed);

    ///This method launches the IEditor::stopping method.
    virtual void stopping() throw(::fwTools::Failed);

    virtual void updating() throw(::fwTools::Failed);

    virtual void swapping() throw(::fwTools::Failed);

    virtual void configuring() throw(fwTools::Failed);

    /// Overrides
    virtual void info( std::ostream &_sstream );

    void notifyMaterial();

protected Q_SLOTS:

    void onChangeRepresentation( int id );
    void onChangeShading( int id );
    void onShowNormals(int state );


private:

    void refreshNormals();
    void refreshRepresentation();
    void refreshShading();

    QPointer<QButtonGroup> m_buttonGroup;
    QPointer<QButtonGroup> m_buttonGroupShading;
    QPointer<QButtonGroup> m_normalsRadioBox;

    ::fwData::Material::sptr m_material;

    /// Signal emitted when normals mode changed (0: none, 1: point, 2: cell)
    NormalsModeModifiedSignalType::sptr m_sigNormalsModeModified;

};

} // uiReconstruction

#endif /*__UIRECONSTRUCTIONQT_REPRESENTATIONEDITOR_HPP__*/


