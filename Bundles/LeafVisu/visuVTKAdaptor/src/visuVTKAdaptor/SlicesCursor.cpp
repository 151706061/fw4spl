/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "visuVTKAdaptor/SlicesCursor.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwCore/base.hpp>

#include <fwData/Float.hpp>
#include <fwData/Image.hpp>
#include <fwData/String.hpp>

#include <fwDataTools/fieldHelper/Image.hpp>
#include <fwDataTools/fieldHelper/MedicalImageHelpers.hpp>

#include <fwServices/macros.hpp>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkLine.h> // CELL
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>

#include <boost/assign/list_of.hpp>


fwServicesRegisterMacro( ::fwRenderVTK::IVtkAdaptorService, ::visuVTKAdaptor::SlicesCursor, ::fwData::Image );

namespace visuVTKAdaptor
{

static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_INDEX_SLOT = "updateSliceIndex";
static const ::fwCom::Slots::SlotKeyType s_UPDATE_SLICE_TYPE_SLOT  = "updateSliceType";
static const ::fwCom::Slots::SlotKeyType s_UPDATE_IMAGE_SLOT       = "updateImage";

const ::fwCom::Slots::SlotKeyType SlicesCursor::s_SHOW_FULL_CROSS_SLOT   = "showFullCross";
const ::fwCom::Slots::SlotKeyType SlicesCursor::s_SHOW_NORMAL_CROSS_SLOT = "showNormalCross";
const ::fwCom::Slots::SlotKeyType SlicesCursor::s_SET_CROSS_SCALE_SLOT   = "setCrossScale";

//-----------------------------------------------------------------------------

SlicesCursor::SlicesCursor()  throw() :
    m_cursorPolyData( vtkPolyData::New() ),
    m_cursorMapper( vtkPolyDataMapper::New() ),
    m_cursorActor( vtkActor::New() ),
    m_scale(0.5f),
    m_isSelected(false)
{
    newSlot(s_UPDATE_SLICE_INDEX_SLOT, &SlicesCursor::updateSliceIndex, this);
    newSlot(s_UPDATE_SLICE_TYPE_SLOT, &SlicesCursor::updateSliceType, this);
    newSlot(s_UPDATE_IMAGE_SLOT, &SlicesCursor::updateImage, this);
    newSlot(s_SHOW_FULL_CROSS_SLOT, &SlicesCursor::showFullCross, this);
    newSlot(s_SHOW_NORMAL_CROSS_SLOT, &SlicesCursor::showNormalCross, this);
    newSlot(s_SET_CROSS_SCALE_SLOT, &SlicesCursor::setCrossScale, this);
}

//-----------------------------------------------------------------------------

SlicesCursor::~SlicesCursor()  throw()
{
    m_cursorActor->Delete();
    m_cursorActor = NULL;
    m_cursorMapper->Delete();
    m_cursorMapper = NULL;
    m_cursorPolyData->Delete();
}

//-----------------------------------------------------------------------------

void SlicesCursor::setCrossScale(double scale)
{
    m_scale = scale;
    this->updating();
}

//-----------------------------------------------------------------------------

void SlicesCursor::doConfigure() throw(fwTools::Failed)
{
    SLM_TRACE_FUNC();

    std::string scaleStr = m_configuration->getAttributeValue("scale");
    if ( !scaleStr.empty() )
    {
        SLM_ASSERT("scale attribute must be in a config", m_configuration->getName() == "config");
        m_scale = ::boost::lexical_cast<double>(scaleStr);
    }
}

//-----------------------------------------------------------------------------

void SlicesCursor::doStart() throw(fwTools::Failed)
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    this->buildPolyData();
    this->buildColorAttribute();
    this->updateImageInfos(image);
    this->updateColors();
    m_cursorMapper->SetInputData( m_cursorPolyData );
    m_cursorActor->SetMapper(m_cursorMapper);
    if(!this->getTransformId().empty())
    {
        m_cursorActor->SetUserTransform(this->getTransform());
    }
    this->addToRenderer(m_cursorActor);
    this->setVtkPipelineModified();
}

//-----------------------------------------------------------------------------

void SlicesCursor::doStop() throw(fwTools::Failed)
{
    m_isSelected = false;
    this->removeAllPropFromRenderer();
}

//////////////////////////////
// SHAPE
//    B---------BC----------C
//    |         |           |
//    |         |           |
//    |        BCM          |
//    |                     |
//    |                     |
//   AB----ABM      CDM-----CD
//    |                     |
//    |        ADM          |
//    |         |           |
//    |         |           |
//    A---------AD----------D
//
// no for id point A,B,C,D
// id point AB, BC, CD, AD  = 0,1,...,3
// id point ABM, BCM, CDM, ADM = 4,..,7


void SlicesCursor::buildPolyData()
{
    int nbPoints      = 8;
    vtkPoints* points = vtkPoints::New(VTK_DOUBLE);
    points->SetNumberOfPoints(nbPoints);
    int i;
    for (i = 0; i < nbPoints; i++)
    {
        //points->SetPoint(i, 300*i +0.0, 50*i*i + 0.0 , 0.0);
        points->SetPoint(i, 0.0, 0.0, 0.0);
    }

    vtkCellArray* cells = vtkCellArray::New();
    cells->Allocate(cells->EstimateSize(nbPoints,2));

    vtkIdType pts[2];
    for ( int line = 0; line<4; ++line)
    {
        pts[0] = line; pts[1] = line+4;
        //cells->InsertNextCell(2,pts);
        vtkLine* lineCell = vtkLine::New();
        lineCell->GetPointIds()->SetId(0, line );
        lineCell->GetPointIds()->SetId(1, line+ 4 );
        cells->InsertNextCell(lineCell);
        lineCell->Delete();
    }

    m_cursorPolyData->SetPoints(points);
    points->Delete();
    m_cursorPolyData->SetLines(cells);
    cells->Delete();
    this->setVtkPipelineModified();
}

//-----------------------------------------------------------------------------

void SlicesCursor::barycenter( double ptA[3], double ptB[3], float scale, double result[3] )
{
    for (int i = 0; i<3; ++i )
    {
        result[i] = scale*ptA[i] + (1-scale)*ptB[i];
    }
}

//-----------------------------------------------------------------------------

void SlicesCursor::computeCrossPoints( double _ptA[3], double _ptB[3], double _ptP[3], double _scale,
                                       double _ptAprime[3], double _ptBprime[3] )
{
    double ptPBprime[3];
    double norm2PBprime = 0.0;
    double norm2BP      = 0;
    double norm2AP      = 0;

    // Compute AB x scale / 2 = |A'P| x AB = |PB'| x AB
    for (int i = 0; i<3; ++i )
    {
        ptPBprime[i]  = ( _ptB[i] - _ptA[i] ) * (1-_scale)/2.0;
        norm2PBprime += ptPBprime[i]*ptPBprime[i];
        norm2AP      += (_ptP[i] - _ptA[i])*(_ptP[i] - _ptA[i]);
        norm2BP      += (_ptP[i] - _ptB[i])*(_ptP[i] - _ptB[i]);
    }

    // Compute  APrime = P + PA' = P - |A'P| x AB
    //          BPrime = P + pB' = P + |PB'| x AB
    if ( norm2PBprime > norm2BP )
    {
        for (int i = 0; i<3; ++i )
        {
            _ptAprime[i] = _ptP[i] - ptPBprime[i];
            _ptBprime[i] = _ptB[i];
        }
    }
    else if ( norm2PBprime > norm2AP )
    {
        for (int i = 0; i<3; ++i )
        {
            _ptAprime[i] = _ptA[i];
            _ptBprime[i] = _ptP[i] + ptPBprime[i];
        }
    }
    else
    {
        for (int i = 0; i<3; ++i )
        {
            _ptAprime[i] = _ptP[i] - ptPBprime[i];
            _ptBprime[i] = _ptP[i] + ptPBprime[i];
        }
    }
}

//-----------------------------------------------------------------------------

void SlicesCursor::buildColorAttribute()
{
    unsigned char red[3]   = {255, 0, 0};
    unsigned char green[3] = {0, 255, 0};
    unsigned char blue[3]  = {0, 0, 255};

    typedef  unsigned char* RGBColor;
    typedef std::map< std::string, std::pair< RGBColor, RGBColor> >  DicoType;
    DicoType dict;
    dict["colorXAxis"] = std::make_pair(green,red);
    dict["colorYAxis"] = std::make_pair(red, blue);
    dict["colorZAxis"] = std::make_pair(blue, green );


    for ( DicoType::iterator i = dict.begin(); i!= dict.end(); ++i )
    {
        vtkUnsignedCharArray* colors = vtkUnsignedCharArray::New();
        colors->SetNumberOfComponents(3);
        colors->SetName(  i->first.c_str() );
        colors->InsertNextTupleValue(  i->second.first  );
        colors->InsertNextTupleValue(  i->second.second );
        colors->InsertNextTupleValue(  i->second.first  );
        colors->InsertNextTupleValue(  i->second.second );
        m_cursorPolyData->GetCellData()->AddArray(colors);
    }
    m_cursorMapper->SetScalarModeToUseCellFieldData();
    this->setVtkPipelineModified();
}

//-----------------------------------------------------------------------------

void SlicesCursor::updateColors()
{
    switch (m_orientation )
    {
        case 0: m_cursorMapper->SelectColorArray("colorXAxis"); break;
        case 1: m_cursorMapper->SelectColorArray("colorYAxis"); break;
        case 2: m_cursorMapper->SelectColorArray("colorZAxis"); break;

    }
    this->setVtkPipelineModified();
}

//-----------------------------------------------------------------------------

void SlicesCursor::doSwap() throw(fwTools::Failed)
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    this->updateImageInfos(image);
    this->updating();
}

//-----------------------------------------------------------------------------

void SlicesCursor::doUpdate() throw(fwTools::Failed)
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    bool imageIsValid = ::fwDataTools::fieldHelper::MedicalImageHelpers::checkImageValidity( image );

    if ( imageIsValid)
    {
        this->updateImageSliceIndex(image);
        this->updateColors();
    }
}

//-----------------------------------------------------------------------------

void SlicesCursor::updateImageSliceIndex( ::fwData::Image::sptr image )
{
    float scale = m_isSelected ? 1.0 : m_scale;
    if (scale <= 0)
    {
        m_cursorActor->VisibilityOff();
    }
    else
    {
        m_cursorActor->VisibilityOn();

        unsigned int pos[3];

        pos[2] = m_axialIndex->value();
        pos[1] = m_frontalIndex->value();
        pos[0] = m_sagittalIndex->value();

        double sliceWorld[3];
        for (int dim = 0; dim<3; ++dim )
        {
            sliceWorld[dim] = pos[dim]*image->getSpacing()[dim] + image->getOrigin().at(dim);
        }

        double cursorPoints[8][3]; // point AB,BC,CD,AD,ABM,BCM,CDM,ADM

        for ( int p = 0; p<2; ++p )
        {
            for (int dim = 0; dim<3; ++dim )
            {
                //cursorPoints[p][dim] =   ((m_orientation==dim   ||   ( (dim != m_orientation) && p )  )?sliceWorld[dim] : 0 );
                //cursorPoints[p+2][dim] = ((m_orientation==dim   ||   ( (dim != m_orientation) && p )  )?sliceWorld[dim] : (image->getSize()[dim]-1)*image->getSpacing()[dim] );
                cursorPoints[p][dim]   = sliceWorld[dim];
                cursorPoints[p+2][dim] = sliceWorld[dim];
                if ( (dim + p + 1)%3 == m_orientation )
                {
                    cursorPoints[p][dim]   = image->getOrigin().at(dim);
                    cursorPoints[p+2][dim] = (image->getSize()[dim]-1)*image->getSpacing()[dim] + image->getOrigin().at(
                        dim);
                }
            }
        }

        // Compute ABM & CDM
        computeCrossPoints( cursorPoints[0], cursorPoints[2], sliceWorld, scale, cursorPoints[4], cursorPoints[6] );
        // Compute BCM & ADM
        computeCrossPoints( cursorPoints[1], cursorPoints[3], sliceWorld, scale, cursorPoints[5], cursorPoints[7] );

        vtkPoints* points = m_cursorPolyData->GetPoints();

        for ( int i = 0; i < 8; ++i)
        {
            points->SetPoint(i,cursorPoints[i]);
        }

    }
    m_cursorPolyData->Modified();
    this->setVtkPipelineModified();
}

//-----------------------------------------------------------------------------

void SlicesCursor::updateSliceIndex(int axial, int frontal, int sagittal)
{
    m_axialIndex->value()    = axial;
    m_frontalIndex->value()  = frontal;
    m_sagittalIndex->value() = sagittal;

    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();

    this->updateImageSliceIndex(image);
    this->updating();
}
//-----------------------------------------------------------------------------

void SlicesCursor::showFullCross()
{
    m_isSelected = true;
}

//-----------------------------------------------------------------------------

void SlicesCursor::showNormalCross()
{
    m_isSelected = false;
}

//-----------------------------------------------------------------------------

void SlicesCursor::updateSliceType(int from, int to)
{
    if( to == static_cast<int>(m_orientation) )
    {
        setOrientation( static_cast< Orientation >( from ));
    }
    else if( from == static_cast<int>(m_orientation) )
    {
        setOrientation( static_cast< Orientation >( to ));
    }
    this->updating();
}

//-----------------------------------------------------------------------------

void SlicesCursor::updateImage()
{
    ::fwData::Image::sptr image = this->getObject< ::fwData::Image >();
    this->updateImageInfos(image);
    this->updating();
}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsType SlicesCursor::getObjSrvConnections() const
{
    KeyConnectionsType connections;
    connections.push_back( std::make_pair( ::fwData::Image::s_MODIFIED_SIG, s_UPDATE_IMAGE_SLOT ) );
    connections.push_back( std::make_pair( ::fwData::Image::s_SLICE_INDEX_MODIFIED_SIG, s_UPDATE_SLICE_INDEX_SLOT ) );
    connections.push_back( std::make_pair( ::fwData::Image::s_SLICE_TYPE_MODIFIED_SIG, s_UPDATE_SLICE_TYPE_SLOT ) );
    connections.push_back( std::make_pair( ::fwData::Image::s_BUFFER_MODIFIED_SIG, s_UPDATE_IMAGE_SLOT ) );

    return connections;
}

//-----------------------------------------------------------------------------


} //namespace visuVTKAdaptor
