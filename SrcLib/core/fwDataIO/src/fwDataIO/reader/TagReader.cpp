/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwDataIO/reader/TagReader.hpp"

#include "fwDataIO/reader/registry/macros.hpp"

#include <fwData/Point.hpp>
#include <fwData/PointList.hpp>
#include <fwData/Tag.hpp>
#include <fwData/location/SingleFile.hpp>

#include <fwMath/IntrasecTypes.hpp>

#include <boost/cstdint.hpp>

#include <fstream>
#include <iostream>

fwDataIOReaderRegisterMacro( ::fwDataIO::reader::TagReader );

namespace fwDataIO
{

namespace reader
{

//------------------------------------------------------------------------------

TagReader::TagReader(::fwDataIO::reader::IObjectReader::Key key) :
    ::fwData::location::enableSingleFile< IObjectReader >(this)
{
}

//------------------------------------------------------------------------------

TagReader::~TagReader()
{
}

//------------------------------------------------------------------------------

void TagReader::read()
{
    assert( ::std::dynamic_pointer_cast< ::fwData::location::SingleFile >(m_location) );
    ::boost::filesystem::path path =
        ::std::dynamic_pointer_cast< ::fwData::location::SingleFile >(m_location)->getPath();

    OSLM_INFO( "[TagReader::read] Tag file: " << path);
    assert( path.empty() == false );

    ::std::shared_ptr< ::fwData::Tag > tag = getConcreteObject();

    std::fstream file;
    file.open(path.string().c_str(), std::fstream::in);
    if (!file.is_open())
    {
        OSLM_ERROR( "Tag file loading error for " << path.string());
        std::string str = "Unable to open ";
        str += path.string();
        throw std::ios_base::failure(str);
    }

    /// Read content and update tag data structure
    std::string name, type;
    int n;
    file>>name;
    file>>n;
    if(n >= 1)
    {
        file>>type;

        if(type == "ARTAG" || type == "CHESSBOARD" || type == "ARToolKitPlus_MARKER_ID_BCH")
        {
            int nbPts;
            double x, y, z;
            double radius = 0.0;
            file>>x>>y>>z;
            file>>nbPts;
            tag->setType(type);
            for(int i = 0; i < nbPts; i++)
            {
                ::fwData::Point::sptr p;
                fwVec3d vPoint;
                file>>vPoint[0]>>vPoint[1]>>vPoint[2]>>radius;
                p->setCoord(vPoint);
                tag->getRefPointList()->getRefPoints().push_back(p);
            }
        }
        else
        {
            SLM_ERROR( "Tag file loading error for " + path.string() + " with type " + type);
            std::string str = "Unable to open ";
            str += path.string();
            throw std::ios_base::failure(str);
        }
    }

    file.close();

}

//------------------------------------------------------------------------------

std::string TagReader::extension()
{
    return (".tag");
}

//------------------------------------------------------------------------------

} // namespace reader

} // namespace fwDataIO
