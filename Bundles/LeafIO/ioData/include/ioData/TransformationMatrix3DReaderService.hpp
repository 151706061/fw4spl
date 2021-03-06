/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IODATA_TRANSFORMATIONMATRIX3DREADERSERVICE_HPP__
#define __IODATA_TRANSFORMATIONMATRIX3DREADERSERVICE_HPP__

#include "ioData/config.hpp"

#include <io/IReader.hpp>

#include <boost/filesystem/path.hpp>

namespace ioData
{
/**
 * @brief   Transformation matrix 3D reader service.
 *
 * Service reading a TransformationMatrix3D object.
 * @li Use setConfiguration(cfg) and configure() methods to configure the matrix filename.
 * @li Use start() to initialize the service.
 * @li Use update() to read the transformation matrix.
 * @li Use stop() to stop service before to destroy it.
 *
 * Service registered details : \n
 * fwServicesRegisterMacro( ::io::IReader , ::ioData::TransformationMatrix3DReaderService ,
 *::fwData::TransformationMatrix3D )
 */
class IODATA_CLASS_API TransformationMatrix3DReaderService : public ::io::IReader
{

public:
    fwCoreServiceClassDefinitionsMacro( (TransformationMatrix3DReaderService)(::io::IReader) );

    /// Super class of reader services
    typedef ::io::IReader SuperClass;

    /** @name Specified reader service methods ( override from ::io::IReader )
     * @{
     */

    /**
     * @brief Configure the image path.
     *
     * This method is used to find
     * the file path  using a file selector.
     */
    IODATA_API virtual void configureWithIHM();

    /**
     * @brief   returns  (filename) extension
     */
    IODATA_API virtual std::vector< std::string > getSupportedExtensions();
    /// @}

    /// Return path type managed by the service, here FILE
    IODATA_API virtual ::io::IOPathType getIOPathType() const;

protected:

    /** @name Service methods ( override from ::fwServices::IService )
     * @{
     */

    /**
     * @brief Starting method. This method is called by start() from base service ( ::fwServices::IService )
     *
     * This method is used to initialize the service. The starting method is empty for this service.
     */
    IODATA_API virtual void starting( );

    /**
     * @brief Stopping method. This method is called by stop() from base service ( ::fwServices::IService )
     *
     * The stopping method is empty for this service.
     */
    IODATA_API virtual void stopping( );

    /**
     * @brief Configuring method : calls implementation from `io::IReader`
     */
    IODATA_API virtual void configuring();

    /**
     * @brief Updating method. This method is called by update() from base service ( ::fwServices::IService )
     *
     * This method is used to update the service.
     * The transformation matrix is read with the reader ::fwDataIO::reader::TransformationMatrix3DReader.
     * Notify reading.
     */
    IODATA_API void updating();

    /**
     * @brief Info method.
     *
     * This method is used to give
     * informations about the service.
     *
     * @param[out] _sstream output stream
     */
    IODATA_API virtual void info(std::ostream& _sstream );
    /// @}

};

} // namespace ioData

#endif // __IODATA_TRANSFORMATIONMATRIX3DREADERSERVICE_HPP__
