/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwRenderVTK/vtk/fwHandleRepresentation3D.hpp"

#include <vtkCamera.h>
#include <vtkCleanPolyData.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkFollower.h>
#include <vtkInteractorObserver.h>
#include <vtkMath.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

namespace fwRenderVTK
{

namespace vtk
{

vtkStandardNewMacro(fwHandleRepresentation3D);

//----------------------------------------------------------------------
fwHandleRepresentation3D::fwHandleRepresentation3D()
{
    // Instantiate a handle template shape as a cube
    this->CubeSource = vtkCubeSource::New();

    // Instantiate a handle template shape as a sphere
    this->SphereSource = vtkSphereSource::New();
    this->SphereSource->SetThetaResolution(20);
    this->SphereSource->SetPhiResolution(20);
    this->SphereSource->Update();
    this->SetHandle(this->SphereSource->GetOutput());

    this->GetSelectedProperty()->SetColor(0., 1., 0.);

    this->ShapeRepresentation = SPHERE;

    vtkSmartPointer<vtkCylinderSource> cynlinderSource = vtkCylinderSource::New();
    cynlinderSource->SetCenter(0., -1., 0.);
    cynlinderSource->SetResolution(64);
    cynlinderSource->SetHeight(0.);
    this->Marker = cynlinderSource;

    this->CleanPolyData = vtkCleanPolyData::New();
    this->CleanPolyData->PointMergingOn();
    this->CleanPolyData->CreateDefaultLocator();
    this->CleanPolyData->SetInputConnection(0, this->Marker->GetOutputPort(0));

    vtkPolyDataNormals* MarkerNormals = vtkPolyDataNormals::New();
    MarkerNormals->SetInputConnection( 0, this->CleanPolyData->GetOutputPort(0) );

    this->MarkerMapper = vtkPolyDataMapper::New();
    this->MarkerMapper->SetInputConnection( MarkerNormals->GetOutputPort() );
    MarkerNormals->Delete();

    this->Follower = vtkFollower::New();
    this->Follower->SetMapper(this->MarkerMapper);
    this->Follower->RotateX(90);

    // Set up the initial properties, parent's one is called in parent's constructor
    this->CreateDefaultProperties();

    this->MarkerRadiusFactor = 1.2;
    this->SetMarkerProperty(this->MarkerProperty);
    cynlinderSource->SetRadius(this->MarkerRadiusFactor * this->SphereSource->GetRadius());
}

//----------------------------------------------------------------------

fwHandleRepresentation3D::~fwHandleRepresentation3D()
{

}

//-----------------------------------------------------------------------------

void fwHandleRepresentation3D::PrintSelf(ostream& os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "HandleSize: " << this->HandleSize << endl;
    os << indent << "ShapeRepresentation: " << (this->ShapeRepresentation == SPHERE ? "SPHERE" : "CUBE") << endl;
    if (this->SphereSource)
    {
        this->SphereSource->PrintSelf(os, indent.GetNextIndent());
    }
    if (this->CubeSource)
    {
        this->CubeSource->PrintSelf(os, indent.GetNextIndent());
    }
}

//-------------------------------------------------------------------------

void fwHandleRepresentation3D::SetWorldPosition(double p[3])
{
    this->vtkPolygonalHandleRepresentation3D::SetWorldPosition(p);
    this->Follower->SetPosition(this->GetWorldPosition());// p may have been clamped

    this->UpdateLabel();
}

//----------------------------------------------------------------------

void fwHandleRepresentation3D::CreateDefaultProperties()
{
    this->MarkerProperty = vtkProperty::New();
    this->MarkerProperty->SetColor(1., 1., 0.);
    this->MarkerProperty->SetOpacity(1.);
}

//---------------------------------------------------------------------------

void fwHandleRepresentation3D::SetShapeRepresentation( Shape shape)
{
    this->ShapeRepresentation = shape;
    if (this->ShapeRepresentation == SPHERE)
    {
        vtkSmartPointer<vtkCylinderSource> cynlinderSource = vtkCylinderSource::SafeDownCast(this->Marker);
        if (!cynlinderSource)
        {
            cynlinderSource = vtkCylinderSource::New();
            cynlinderSource->SetCenter(0., -1., 0.);
            cynlinderSource->SetResolution(64);
            cynlinderSource->SetHeight(0.);
            cynlinderSource->SetRadius(this->MarkerRadiusFactor * this->SphereSource->GetRadius() );
            this->Marker = cynlinderSource;
            this->CleanPolyData->SetInputConnection(0, this->Marker->GetOutputPort(0));
        }

        this->SetHandle(this->SphereSource->GetOutput());
    }
    else
    {
        vtkSmartPointer<vtkCubeSource> cubeSource = vtkCubeSource::SafeDownCast(this->Marker);
        if (!cubeSource)
        {
            cubeSource = vtkCubeSource::New();
            cubeSource->SetCenter(0., -1., 0.);
            cubeSource->SetXLength(this->MarkerRadiusFactor * this->CubeSource->GetXLength() );
            cubeSource->SetYLength(0.);
            cubeSource->SetZLength(this->MarkerRadiusFactor * this->CubeSource->GetXLength() );
            this->Marker = cubeSource;
            this->CleanPolyData->SetInputConnection(0, this->Marker->GetOutputPort(0));
        }

        this->SetHandle(this->CubeSource->GetOutput());
    }
}

//---------------------------------------------------------------------------
void fwHandleRepresentation3D::BuildRepresentation()
{
    if (!this->GetRenderer() ||
        !this->GetRenderer()->GetActiveCamera())
    {
        return;
    }

    double currLength;
    if (this->ShapeRepresentation == SPHERE)
    {
        currLength = this->SphereSource->GetRadius()*2;
    }
    else
    {
        currLength = this->CubeSource->GetXLength();
    }

    if (currLength != this->HandleSize)
    {
        // Generate a handle with a radius of w_r in physical units
        if (this->ShapeRepresentation == SPHERE)
        {
            this->SphereSource->SetRadius(this->HandleSize / 2.);
            this->SphereSource->Update();

            vtkSmartPointer<vtkCylinderSource> cynlinderSource = vtkCylinderSource::SafeDownCast(this->Marker);
            if (cynlinderSource)
            {
                cynlinderSource->SetRadius(this->MarkerRadiusFactor * this->HandleSize / 2. );
            }

            this->SetHandle(this->SphereSource->GetOutput());
        }
        else
        {
            this->CubeSource->SetXLength(this->HandleSize);
            this->CubeSource->SetYLength(this->HandleSize);
            this->CubeSource->SetZLength(this->HandleSize);
            this->CubeSource->Update();

            vtkSmartPointer<vtkCubeSource> cubeSource = vtkCubeSource::SafeDownCast(this->Marker);
            if (cubeSource)
            {
                cubeSource->SetXLength(this->MarkerRadiusFactor * this->HandleSize );
                cubeSource->SetYLength(0.);
                cubeSource->SetZLength(this->MarkerRadiusFactor * this->HandleSize );
            }

            this->SetHandle(this->CubeSource->GetOutput());
        }

        this->Follower->SetCamera( this->GetRenderer()->GetActiveCamera() );
        this->Marker->Update();

        const double textScale = 10;
        this->SetLabelTextScale(textScale, textScale, textScale);

        // Update the label,
        this->UpdateLabel();

        this->BuildTime.Modified();
    }
}

//----------------------------------------------------------------------

void fwHandleRepresentation3D::GetActors(vtkPropCollection* pc)
{
    this->Actor->GetActors(pc);
    this->LabelTextActor->GetActors(pc);
    this->Follower->GetActors(pc);
}

//----------------------------------------------------------------------

void fwHandleRepresentation3D::ReleaseGraphicsResources(vtkWindow* win)
{
    this->Actor->ReleaseGraphicsResources(win);
    this->LabelTextActor->ReleaseGraphicsResources(win);
    this->Follower->ReleaseGraphicsResources(win);
}

//----------------------------------------------------------------------

int fwHandleRepresentation3D::RenderOpaqueGeometry(vtkViewport* viewport)
{
    this->BuildRepresentation();
    int ret = 0;
    if (this->GetRenderer()->GetActiveCamera()->GetParallelProjection())
    {
        ret = this->Follower->RenderOpaqueGeometry(viewport);
    }
    if (this->HandleVisibility)
    {
        ret += this->Actor->RenderOpaqueGeometry(viewport);
    }
    if (this->LabelVisibility)
    {
        ret += this->LabelTextActor->RenderOpaqueGeometry(viewport);
    }

    return ret;
}

//----------------------------------------------------------------------

int fwHandleRepresentation3D::RenderTranslucentPolygonalGeometry(vtkViewport* viewport)
{
    this->BuildRepresentation();
    int ret = 0;
    if (this->GetRenderer()->GetActiveCamera()->GetParallelProjection())
    {
        ret = this->Follower->RenderTranslucentPolygonalGeometry(viewport);
    }
    if (this->HandleVisibility)
    {
        ret += this->Actor->RenderTranslucentPolygonalGeometry(viewport);
    }
    if (this->LabelVisibility)
    {
        ret += this->LabelTextActor->RenderTranslucentPolygonalGeometry(viewport);
    }
    return this->Actor->RenderTranslucentPolygonalGeometry(viewport) + ret;
}

//----------------------------------------------------------------------

int fwHandleRepresentation3D::HasTranslucentPolygonalGeometry()
{
    return 1;
}

//----------------------------------------------------------------------

void fwHandleRepresentation3D::SetMarkerProperty(vtkProperty* p)
{
    vtkSetObjectBodyMacro(MarkerProperty, vtkProperty, p);
    if (p)
    {
        this->Follower->SetProperty( p );
    }
}

} // namespace vtk

} // namespace fwRenderVTK
