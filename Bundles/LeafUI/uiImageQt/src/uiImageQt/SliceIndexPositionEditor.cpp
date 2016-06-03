/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "uiImageQt/SliceIndexPositionEditor.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>
#include <fwCom/Signals.hpp>
#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwComEd/Dictionary.hpp>
#include <fwComEd/fieldHelper/MedicalImageHelpers.hpp>

#include <fwCore/base.hpp>

#include <fwData/Composite.hpp>
#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>

#include <fwGuiQt/container/QtContainer.hpp>

#include <fwRuntime/ConfigurationElement.hpp>
#include <fwRuntime/operations.hpp>

#include <fwServices/macros.hpp>

#include <QWidget>
#include <QVBoxLayout>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <functional>

namespace uiImage
{

fwServicesRegisterMacro( ::gui::editor::IEditor, ::uiImage::SliceIndexPositionEditor, ::fwData::Image );

const std::string* SliceIndexPositionEditor::SLICE_INDEX_FIELDID[ 3 ] =
{
    &fwComEd::Dictionary::m_sagittalSliceIndexId,
    &fwComEd::Dictionary::m_frontalSliceIndexId,
    &fwComEd::Dictionary::m_axialSliceIndexId
};

static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_INDEX_SLOT = "updateSliceIndex";
static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_TYPE_SLOT  = "updateSliceType";

//------------------------------------------------------------------------------

SliceIndexPositionEditor::SliceIndexPositionEditor() throw()
{
    newSlot(s_UPDATE_SLICE_INDEX_SLOT, &SliceIndexPositionEditor::updateSliceIndex, this);
    newSlot(s_UPDATE_SLICE_TYPE_SLOT, &SliceIndexPositionEditor::updateSliceType, this);
}

//------------------------------------------------------------------------------

SliceIndexPositionEditor::~SliceIndexPositionEditor() throw()
{
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::starting() throw(::fwTools::Failed)
{
    this->create();

    ::fwGuiQt::container::QtContainer::sptr qtContainer = ::fwGuiQt::container::QtContainer::dynamicCast(
        this->getContainer() );
    QWidget * const container = qtContainer->getQtContainer();
    SLM_ASSERT("container not instanced", container);

    QVBoxLayout* layout = new QVBoxLayout( container );

    m_sliceSelectorPanel = new ::fwGuiQt::SliceSelector( container );
    m_sliceSelectorPanel->setEnable(false);

    ::fwGuiQt::SliceSelector::ChangeIndexCallback changeIndexCallback;
    changeIndexCallback = std::bind( &::uiImage::SliceIndexPositionEditor::sliceIndexNotification, this,
                                     std::placeholders::_1);
    m_sliceSelectorPanel->setChangeIndexCallback(changeIndexCallback);

    ::fwGuiQt::SliceSelector::ChangeIndexCallback changeTypeCallback;
    changeTypeCallback = std::bind( &::uiImage::SliceIndexPositionEditor::sliceTypeNotification, this,
                                    std::placeholders::_1);
    m_sliceSelectorPanel->setChangeTypeCallback(changeTypeCallback);
    layout->addWidget( m_sliceSelectorPanel );
    layout->setContentsMargins(0,0,0,0);

    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    this->updateImageInfos(image);
    this->updateSliceTypeFromImg(m_orientation);

    container->setLayout( layout );

    this->updating();
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::stopping() throw(::fwTools::Failed)
{
    if(m_sliceSelectorPanel)
    {
        delete m_sliceSelectorPanel;
        m_sliceSelectorPanel = 0;
    }

    this->getContainer()->clean();
    this->destroy();
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::configuring() throw(fwTools::Failed)
{
    this->initialize();

    if( this->m_configuration->size() > 0 )
    {
        std::vector< ::fwRuntime::ConfigurationElement::sptr > slideIndexCfg = m_configuration->find("sliceIndex");
        SLM_ASSERT("Only one xml element \"sliceIndex\" is accepted.", slideIndexCfg.size() == 1 );
        SLM_ASSERT("The xml element \"sliceIndex\" is empty.", !(*slideIndexCfg.begin())->getValue().empty() );
        std::string orientation = (*slideIndexCfg.begin())->getValue();
        ::boost::algorithm::trim(orientation);
        ::boost::algorithm::to_lower(orientation);

        if(orientation == "axial" )
        {
            m_orientation = Z_AXIS;
        }
        else if(orientation == "frontal" )
        {
            m_orientation = Y_AXIS;
        }
        else if(orientation == "sagittal" )
        {
            m_orientation = X_AXIS;
        }
        else
        {
            SLM_FATAL("The value for the xml element \"sliceIndex\" can only be axial, frontal or sagittal.");
        }
    }
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::updating() throw(::fwTools::Failed)
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    bool imageIsValid = ::fwComEd::fieldHelper::MedicalImageHelpers::checkImageValidity( image );
    m_sliceSelectorPanel->setEnable(imageIsValid);
    this->updateImageInfos(image);
    this->updateSliceIndexFromImg();
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::swapping() throw(::fwTools::Failed)
{
    this->updating();
}

//-----------------------------------------------------------------------------

void SliceIndexPositionEditor::updateSliceIndex(int axial, int frontal, int sagittal)
{
    m_axialIndex->value()    = axial;
    m_frontalIndex->value()  = frontal;
    m_sagittalIndex->value() = sagittal;

    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();

    image->setField( fwComEd::Dictionary::m_axialSliceIndexId, m_axialIndex);
    image->setField( fwComEd::Dictionary::m_frontalSliceIndexId, m_frontalIndex);
    image->setField( fwComEd::Dictionary::m_sagittalSliceIndexId, m_sagittalIndex);
    this->updateSliceIndexFromImg();
}

//-----------------------------------------------------------------------------

void SliceIndexPositionEditor::updateSliceType(int from, int to)
{
    if( to == static_cast< int > ( m_orientation ) )
    {
        m_orientation = static_cast< Orientation > ( from );
    }
    else if(from == static_cast<int>(m_orientation))
    {
        m_orientation = static_cast< Orientation >( to );
    }
    this->updateSliceTypeFromImg(m_orientation);
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::info( std::ostream &_sstream )
{
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::updateSliceIndexFromImg()
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();

    if (::fwComEd::fieldHelper::MedicalImageHelpers::checkImageValidity(image))
    {
        // Get Index
        std::string fieldID = *SLICE_INDEX_FIELDID[m_orientation];
        OSLM_ASSERT("Field "<<fieldID<<" is missing", image->getField( fieldID ) );
        unsigned int index = image->getField< ::fwData::Integer >( fieldID )->value();

        // Update QSlider
        int max = 0;
        if(image->getNumberOfDimensions() > m_orientation)
        {
            max = static_cast<int>(image->getSize()[m_orientation]-1);
        }
        m_sliceSelectorPanel->setSliceRange( 0, max );
        m_sliceSelectorPanel->setSliceValue( index );
    }
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::updateSliceTypeFromImg(Orientation type )
{
    // Update Type Choice
    m_sliceSelectorPanel->setTypeSelection( static_cast< int >( type ) );

    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    this->updateSliceIndexFromImg();
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::sliceIndexNotification( unsigned int index)
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();

    std::string fieldID = *SLICE_INDEX_FIELDID[m_orientation];
    OSLM_ASSERT("Field "<<fieldID<<" is missing", image->getField( fieldID ));
    image->getField< ::fwData::Integer >( fieldID )->value() = index;

    auto sig = image->signal< ::fwData::Image::SliceIndexModifiedSignalType >(
        ::fwData::Image::s_SLICE_INDEX_MODIFIED_SIG);
    ::fwCom::Connection::Blocker block(sig->getConnection(this->slot(s_UPDATE_SLICE_INDEX_SLOT)));
    sig->asyncEmit(m_axialIndex->value(), m_frontalIndex->value(), m_sagittalIndex->value());
}

//------------------------------------------------------------------------------

void SliceIndexPositionEditor::sliceTypeNotification( int _type )
{
    Orientation type = static_cast< Orientation >( _type );
    OSLM_ASSERT("Bad slice type "<<type, type == X_AXIS ||
                type == Y_AXIS ||
                type == Z_AXIS );

    int oldType = static_cast< int > ( m_orientation );
    // Change slice type
    m_orientation = type;

    // Fire the signal
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    auto sig = image->signal< ::fwData::Image::SliceTypeModifiedSignalType >(
        ::fwData::Image::s_SLICE_TYPE_MODIFIED_SIG);
    {
        ::fwCom::Connection::Blocker block(sig->getConnection(this->slot(s_UPDATE_SLICE_TYPE_SLOT)));
        sig->asyncEmit(oldType, _type);
    }
    this->updateSliceIndexFromImg();
}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsType SliceIndexPositionEditor::getObjSrvConnections() const
{
    KeyConnectionsType connections;
    connections.push_back( std::make_pair( ::fwData::Image::s_MODIFIED_SIG, s_UPDATE_SLOT ) );
    connections.push_back( std::make_pair( ::fwData::Image::s_SLICE_INDEX_MODIFIED_SIG, s_UPDATE_SLICE_INDEX_SLOT ) );
    connections.push_back( std::make_pair( ::fwData::Image::s_SLICE_TYPE_MODIFIED_SIG, s_UPDATE_SLICE_TYPE_SLOT ) );
    connections.push_back( std::make_pair( ::fwData::Image::s_BUFFER_MODIFIED_SIG, s_UPDATE_SLOT ) );

    return connections;
}

//------------------------------------------------------------------------------

}

