/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWCOMED_HELPER_MEDICALIMAGEADAPTOR_HPP__
#define __FWCOMED_HELPER_MEDICALIMAGEADAPTOR_HPP__

#include "fwComEd/helper/ImageGetter.hpp"
#include "fwComEd/config.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slots.hpp>

#include <fwData/Composite.hpp>
#include <fwData/Integer.hpp>
#include <fwData/String.hpp>
#include <fwData/TransferFunction.hpp>

#include <fwServices/helper/SigSlotConnection.hpp>

#include <vector>


namespace fwComEd
{

namespace helper
{

/**
 * @brief   Helpers for medical image.
 */
class FWCOMED_CLASS_API MedicalImageAdaptor
{

public:
    fwCoreNonInstanciableClassDefinitionsMacro( (MedicalImageAdaptor) );


    /// Image orientation
    typedef enum
    {
        X_AXIS = 0,
        Y_AXIS,
        Z_AXIS
    } Orientation;

    /// Destructor. Do nothing.
    FWCOMED_API virtual ~MedicalImageAdaptor();

    /// Set the image orientation.
    FWCOMED_API virtual void setOrientation( Orientation orientation );

    /// Return the image orientation.
    Orientation getOrientation() const
    {
        return m_orientation;
    }

    /// Set TF Selection fwID
    FWCOMED_API void setTFSelectionFwID( const std::string & fwid );

    /// Get TF Selection fwID
    FWCOMED_API const std::string & getTFSelectionFwID() const;

    /// Set TF Selection fwID
    FWCOMED_API void setTransferFunctionSelection( ::fwData::Composite::wptr selection );

    /// Set selected TF Key
    FWCOMED_API void setSelectedTFKey( const std::string & key );

    /// Get selected TF Key
    FWCOMED_API const std::string & getSelectedTFKey() const;

    /// Set selected TF Key and TF Selection fwID from config
    FWCOMED_API void parseTFConfig( ::fwRuntime::ConfigurationElement::sptr configuration );

    /// Get the window of the selected tf
    FWCOMED_API double getWindow() const;

    /// Set the window of the selected tf
    FWCOMED_API void setWindow( double window );

    /// Set the window level of the selected tf from window min/max
    FWCOMED_API void setWindowLevel( double windowMin, double windowMax );

    /// Get the level of the selected tf
    FWCOMED_API double getLevel() const;

    /// Set the level of the selected tf
    FWCOMED_API void setLevel( double level );

protected:


    /// Constructor. Do nothing.
    FWCOMED_API MedicalImageAdaptor(); // this class VISUVTKADAPTOR_CLASS_API must be specialized

    /**
     * @brief Set the orientation of the image.
     * @param[in] orientation must be 0 (X_AXIS), 1 (Y_AXIS) or 2 (Z_AXIS).
     */
    FWCOMED_API void setOrientation( int orientation );

    /**
     * @brief Get the image spacing.
     * @param[out] spacing : the image spacing
     */
    FWCOMED_API void getImageSpacing(double spacing[3]);

    /**
     * @brief Get the image origin.
     * @param[out] origin : the image origin
     */
    FWCOMED_API void getImageOrigin(double origin[3]);

    /**
     * @brief Get the image data size (number of slices).
     * @param[out] size : the image size
     */
    FWCOMED_API void getImageDataSize(int size[3]);

    /**
     * @brief Get the image size ( = dataSize * spacing ).
     * @param[out] size : the image size
     */
    FWCOMED_API void getImageSize(double size[3]);

    /**
     * @brief Get the slice center
     * @param[out] center : the slice center
     */
    FWCOMED_API void getCurrentSliceCenter(double center[3]);

    // Retrieve the grey level from an image from physical world
    //float getPixelvalue( double worldPosition[3]);

    /**
     * @brief Convert world coordinates to slice index coordinates
     * @param[in] world : coordinate in the world
     * @param[out] index : coordinate in the slice index
     */
    FWCOMED_API void worldToSliceIndex(const double world[3],int index[3] );

    /**
     * @brief Convert coordinates in the world to coordinates in the image
     * @param[in] world : coordinate in the world
     * @param[out] index : coordinate in the image
     */
    FWCOMED_API void worldToImageSliceIndex(const double world[3], int index[3] );

    /**
     * @brief Convert from world coordinates system to image coordinates system
     * @param[in] index : coordinate in the image
     * @param[out] world : coordinate in the world
     */
    FWCOMED_API void sliceIndexToWorld(const int index[3], double world[3] );

    /**
     * @brief Get the image spacing.
     * @param[out] spacing : the image spacing
     */
    template< typename FLOAT_ARRAY_3 >
    void getImageSpacing(FLOAT_ARRAY_3 spacing);

    /**
     * @brief Get the image data size (number of slices).
     * @param[out] size : the image size
     */
    template< typename INT_INDEX >
    void getImageDataSize(INT_INDEX size);

    /**
     * @brief Convert world coordinates to slice index coordinates
     * @param[in] world : coordinate in the world
     * @param[out] index : coordinate in the slice index
     */
    template< typename WORLD, typename INT_INDEX >
    void worldToSliceIndex(const WORLD world, INT_INDEX index );

    /**
     * @brief Convert coordinates in the world to coordinates in the image
     * @param[in] world : coordinate in the world
     * @param[out] index : coordinate in the image
     */
    template< typename WORLD, typename INT_INDEX >
    void worldToImageSliceIndex(const WORLD world, INT_INDEX index );




    /**
     * @brief Return the 4 points of the image plane
     *
     * - points are ordered in the following way : image origin is the first point
     * - points are inserted using the preference follow X axis if exists, if not exists follow Y axis
     * if Y axis is not present follow Z axis
     */
    FWCOMED_API void getPlane( double points[4][3], int sliceNumber );

    /// Set the slice index
    FWCOMED_API bool setSliceIndex(const int index[3]);
    /// Get the slice index
    FWCOMED_API void getSliceIndex(::fwData::Integer::sptr index[3]);

    /// Get the current transfer function
    FWCOMED_API ::fwData::TransferFunction::sptr getTransferFunction() const;

    /// Get the current transfer function selection
    FWCOMED_API ::fwData::Composite::sptr getTransferFunctionSelection() const;

    /// Update the image information (slice index, min/max,...)
    FWCOMED_API void updateImageInfos( ::fwData::Image::sptr image  );

    /// Update the transfer function information
    FWCOMED_API void updateTransferFunction( ::fwData::Image::sptr image );

    /// Return the image
    FWCOMED_API ::fwData::Image::sptr getImage();

    /// Image orientation
    Orientation m_orientation;

    /// Current image
    ::fwData::Image::wptr m_weakImage;

    /// Axial slice index
    ::fwData::Integer::sptr m_axialIndex;
    /// Frontal slice index
    ::fwData::Integer::sptr m_frontalIndex;
    /// Sagittal slice index
    ::fwData::Integer::sptr m_sagittalIndex;

    /**
     * @name Connections to transfer function
     * @{
     */

    /// Install connections to listen TF modifications
    FWCOMED_API void installTFConnections();

    /// Remove the TF connections
    FWCOMED_API void removeTFConnections();

    /**
     *  @brief Called when transfer function points are modified.
     *
     *  It must be reimplemented to upadte TF.
     */
    FWCOMED_API virtual void updatingTFPoints();

    /**
     *  @brief Called when transfer function windowing is modified.
     *
     *  It must be reimplemented to upadte TF.
     */
    FWCOMED_API virtual void updatingTFWindowing(double window, double level);

    /**
     * @brief Install the slots to managed TF modifications.
     *
     * Creates slots to listen TF selection Composite and TransferFunction signals.
     *
     * @warning It must be called in the service constructor
     */
    FWCOMED_API void installTFSlots(::fwCom::HasSlots* hasslots);

    /// Slot: add objects
    void addObjects(::fwData::Composite::ContainerType objects);

    /// Slot: change objects
    void changeObjects(::fwData::Composite::ContainerType newObjects, ::fwData::Composite::ContainerType oldObjects);

    /// Slot: remove objects
    void removeObjects(::fwData::Composite::ContainerType objects);

    /// Slot: called when transfer function points are modified
    void updateTFPoints();

    /// Slot: called when transfer function windowing is modified
    void updateTFWindowing(double window, double level);

    typedef ::fwCom::Slot<void (::fwData::Composite::ContainerType)> AddedObjectsSlotType;
    typedef ::fwCom::Slot<void (::fwData::Composite::ContainerType,
                                ::fwData::Composite::ContainerType)> ChangedObjectsSlotType;
    typedef ::fwCom::Slot<void (::fwData::Composite::ContainerType)> RemovedObjectsSlotType;
    typedef ::fwCom::Slot<void ()> UpdateTFPointsSlotType;
    typedef ::fwCom::Slot<void (double, double)> UpdateTFWindowingSlotType;

    /// Slot called when objects are added into the composite
    AddedObjectsSlotType::sptr m_slotAddedObjects;

    /// Slot called when objects are changed into the composite
    ChangedObjectsSlotType::sptr m_slotChangedObjects;

    /// Slot called when objects are removed from the composite
    RemovedObjectsSlotType::sptr m_slotRemovedObjects;

    /// Slot called when transfer function points are modified
    UpdateTFPointsSlotType::sptr m_slotUpdateTFPoints;

    /// Slot called when transfer function windowing is modified
    UpdateTFWindowingSlotType::sptr m_slotUpdateTFWindowing;
    /**
     * @}
     */

private:

    ::fwServices::helper::SigSlotConnection::sptr m_tfSelectionConnections;
    ::fwServices::helper::SigSlotConnection::sptr m_tfConnections;

    /// Transfer function selection
    ::fwData::Composite::wptr m_tfSelection;

    /// fwID of tf selections ( used during configuration )
    std::string m_tfSelectionFwID;

    /// Identifier of the key containing the current selection of TransferFunction in TFSelection
    std::string m_selectedTFKey;
};

struct Image0 {};
struct Image1 {};

template < typename IMAGEID >
class FWCOMED_CLASS_API MedicalImageAdaptorTpl : public MedicalImageAdaptor
{
};

typedef MedicalImageAdaptorTpl<Image0> MedicalImageAdaptorImg0;
typedef MedicalImageAdaptorTpl<Image1> MedicalImageAdaptorImg1;


//------------------------------------------------------------------------------
template< typename FLOAT_ARRAY_3 >
void MedicalImageAdaptor::getImageSpacing(FLOAT_ARRAY_3 spacing)
{
    ::fwData::Image::sptr image = this->getImage();

    const ::fwData::Image::SpacingType imSpacing = image->getSpacing();
    std::copy(imSpacing.begin(), imSpacing.end(), spacing);
}

//------------------------------------------------------------------------------
template< typename INT_INDEX >
void MedicalImageAdaptor::getImageDataSize(INT_INDEX size)
{
    ::fwData::Image::sptr image = this->getImage();

    const ::fwData::Image::SizeType imSize = image->getSize();
    std::copy(imSize.begin(), imSize.end(), size);
}

//------------------------------------------------------------------------------

template< typename WORLD, typename INT_INDEX >
void MedicalImageAdaptor::worldToSliceIndex(const WORLD world, INT_INDEX index )
{
    double spacing[3];
    this->getImageSpacing(spacing);
    double origin[3];
    this->getImageOrigin(origin);
    for ( int i = 0; i<3; ++i )
    {
        index[i] =
            static_cast< int >( ( (world[i] - origin[i])/spacing[i] ) +
                                ( ( (world[i] - origin[i])/spacing[i] ) >= 0 ? 0.5 : -0.5 ) );
    }
}

//------------------------------------------------------------------------------

template< typename WORLD, typename INT_INDEX >
void MedicalImageAdaptor::worldToImageSliceIndex(const WORLD world, INT_INDEX index )
{
    int imageSize[3];
    this->getImageDataSize(imageSize);
    this->worldToSliceIndex(world, index);

    int idval;
    for (int i = 0; i < 3; i++)
    {
        int max = imageSize[i]-1;
        idval = index[i];
        if (idval < 0)
        {
            index[i] = 0;
        }
        else if (idval > max)
        {
            index[i] = max;
        }
    }
}

} //namespace helper

} //namespace fwComEd

#endif // __FWCOMED_HELPER_MEDICALIMAGEADAPTOR_HPP__

