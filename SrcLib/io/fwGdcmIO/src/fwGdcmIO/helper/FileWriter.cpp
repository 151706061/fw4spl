/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwGdcmIO/exception/Failed.hpp"
#include "fwGdcmIO/helper/FileWriter.hpp"

#include <fwCore/spyLog.hpp>

#include <gdcmFileMetaInformation.h>
#include <gdcmWriter.h>

namespace fwGdcmIO
{
namespace helper
{

void FileWriter::write(const std::string& filename, SPTR(::gdcm::Writer)writer)
{
    // Set file header
    ::gdcm::FileMetaInformation& metaInformation = writer->GetFile().GetHeader();

    // Transfer syntax - Type 1
    metaInformation.SetDataSetTransferSyntax(::gdcm::TransferSyntax::ExplicitVRLittleEndian);

    // Initialize the file
    writer->SetFileName(filename.c_str()); // NOTE: Must be called when file is ready to be written

    // Write data
    if (!writer->Write())
    {
        const std::string msg = "Unable to write the file " + filename;
        throw ::fwGdcmIO::exception::Failed(msg);
    }

}

} // namespace helper
} // namespace fwGdcmIO

