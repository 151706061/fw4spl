/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "ioVtkGdcm/SSeriesDBLazyReader.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>

#include <fwCore/base.hpp>

#include <fwGui/Cursor.hpp>
#include <fwGui/dialog/LocationDialog.hpp>
#include <fwGui/dialog/MessageDialog.hpp>

#include <fwJobs/IJob.hpp>
#include <fwJobs/Job.hpp>

#include <fwMedData/Series.hpp>
#include <fwMedData/SeriesDB.hpp>

#include <fwServices/macros.hpp>

#include <fwTools/ProgressToLogger.hpp>

#include <vtkGdcmIO/SeriesDBLazyReader.hpp>

#include <io/IReader.hpp>

#include <boost/bind.hpp>

namespace ioVtkGdcm
{

fwServicesRegisterMacro( ::io::IReader, ::ioVtkGdcm::SSeriesDBLazyReader, ::fwMedData::SeriesDB );
static const ::fwCom::Signals::SignalKeyType JOB_CREATED_SIGNAL = "jobCreated";

//------------------------------------------------------------------------------

SSeriesDBLazyReader::SSeriesDBLazyReader() noexcept
{
    m_sigJobCreated = newSignal< JobCreatedSignalType >( JOB_CREATED_SIGNAL );
}

//------------------------------------------------------------------------------

SSeriesDBLazyReader::~SSeriesDBLazyReader() noexcept
{
}

//------------------------------------------------------------------------------

void SSeriesDBLazyReader::configureWithIHM()
{
    static ::boost::filesystem::path _sDefaultPath;

    ::fwGui::dialog::LocationDialog dialogFile;
    dialogFile.setTitle(m_windowTitle.empty() ? this->getSelectorDialogTitle() : m_windowTitle);
    dialogFile.setDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    dialogFile.setOption(::fwGui::dialog::ILocationDialog::READ);
    dialogFile.setType(::fwGui::dialog::LocationDialog::FOLDER);

    ::fwData::location::Folder::sptr result;
    result = ::fwData::location::Folder::dynamicCast( dialogFile.show() );
    if (result)
    {
        _sDefaultPath = result->getFolder();
        this->setFolder( result->getFolder() );
        dialogFile.saveDefaultLocation( ::fwData::location::Folder::New(_sDefaultPath) );
    }
}

//------------------------------------------------------------------------------

void SSeriesDBLazyReader::starting()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SSeriesDBLazyReader::stopping()
{
    SLM_TRACE_FUNC();
}

//------------------------------------------------------------------------------

void SSeriesDBLazyReader::configuring()
{
    ::io::IReader::configuring();
}

//------------------------------------------------------------------------------

void SSeriesDBLazyReader::info(std::ostream& _sstream )
{
    _sstream << "SSeriesDBLazyReader::info";
}

//------------------------------------------------------------------------------

SSeriesDBLazyReader::ExtensionsType SSeriesDBLazyReader::getSupportedExtensions()
{
    ExtensionsType extensions;
    return extensions;
}

//------------------------------------------------------------------------------

std::string SSeriesDBLazyReader::getSelectorDialogTitle()
{
    return "Choose a directory with DICOM images";
}

//------------------------------------------------------------------------------

::fwMedData::SeriesDB::sptr SSeriesDBLazyReader::createSeriesDB(const ::boost::filesystem::path& dicomDir)
{
    SLM_TRACE_FUNC();
    ::vtkGdcmIO::SeriesDBLazyReader::sptr myLoader = ::vtkGdcmIO::SeriesDBLazyReader::New();
    ::fwMedData::SeriesDB::sptr dummy              = ::fwMedData::SeriesDB::New();
    myLoader->setObject(dummy);
    myLoader->setFolder(dicomDir);

    m_sigJobCreated->emit(myLoader->getJob());

    try
    {
        myLoader->read();
    }
    catch (const std::exception& e)
    {
        std::stringstream ss;
        ss << "Warning during loading : " << e.what();
        ::fwGui::dialog::MessageDialog::showMessageDialog(
            "Warning", ss.str(), ::fwGui::dialog::IMessageDialog::WARNING);
    }
    catch( ... )
    {
        ::fwGui::dialog::MessageDialog::showMessageDialog(
            "Warning", "Warning during loading", ::fwGui::dialog::IMessageDialog::WARNING);
    }

    return myLoader->getConcreteObject();
}

//------------------------------------------------------------------------------

void SSeriesDBLazyReader::updating()
{
    SLM_TRACE_FUNC();
    if( this->hasLocationDefined() )
    {
        ::fwMedData::SeriesDB::sptr seriesDB = createSeriesDB( this->getFolder() );

        if( seriesDB->size() > 0 )
        {
            // Retrieve dataStruct associated with this service
            ::fwMedData::SeriesDB::sptr associatedSeriesDB = this->getObject< ::fwMedData::SeriesDB >();
            SLM_ASSERT("associated SeriesDB not instanced", associatedSeriesDB);
            associatedSeriesDB->shallowCopy( seriesDB );

            ::fwGui::Cursor cursor;
            cursor.setCursor(::fwGui::ICursor::BUSY);
            this->notificationOfDBUpdate();
            cursor.setDefaultCursor();
        }
        else
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Image Reader", "This file can not be read. Retry with another file reader.",
                ::fwGui::dialog::IMessageDialog::WARNING);
        }
    }
}

//------------------------------------------------------------------------------

void SSeriesDBLazyReader::notificationOfDBUpdate()
{
    SLM_TRACE_FUNC();
    ::fwMedData::SeriesDB::sptr seriesDB = this->getObject< ::fwMedData::SeriesDB >();
    SLM_ASSERT("Unable to get seriesDB", seriesDB);

    ::fwMedData::SeriesDB::ContainerType addedSeries;
    for( ::fwMedData::Series::sptr s :  seriesDB->getContainer() )
    {
        addedSeries.push_back(s);
    }

    auto sig = seriesDB->signal< ::fwMedData::SeriesDB::AddedSeriesSignalType >(
        ::fwMedData::SeriesDB::s_ADDED_SERIES_SIG);
    sig->asyncEmit(addedSeries);
}

//-----------------------------------------------------------------------------

::io::IOPathType SSeriesDBLazyReader::getIOPathType() const
{
    return ::io::FOLDER;
}

//------------------------------------------------------------------------------

} // namespace ioInr
