/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_TRANSFORMATIONMATRIX3D_HPP__
#define __FWDATA_TRANSFORMATIONMATRIX3D_HPP__

#include "fwData/Object.hpp"
#include "fwData/factory/new.hpp"

#include <array>
#include <iostream>

fwCampAutoDeclareDataMacro((fwData)(TransformationMatrix3D), FWDATA_API);

namespace fwData
{

/**
 * @brief This class represents a 3D transformation matrix (4x4).
 */
class FWDATA_CLASS_API TransformationMatrix3D : public Object
{

public:
    fwCoreClassDefinitionsWithFactoryMacro( (TransformationMatrix3D)(::fwData::Object),
                                            (()), ::fwData::factory::New< TransformationMatrix3D >);

    fwCampMakeFriendDataMacro((fwData)(TransformationMatrix3D));

    typedef double TM3DType;
    typedef std::array<TM3DType, 16> TMCoefArray;

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWDATA_API TransformationMatrix3D(::fwData::Object::Key key);

    //! @brief destructor
    FWDATA_API virtual ~TransformationMatrix3D();

    /// Defines shallow copy
    FWDATA_API void shallowCopy( const Object::csptr& _source );

    /// Defines deep copy
    FWDATA_API void cachedDeepCopy(const Object::csptr& _source, DeepCopyCacheType& cache);

    /// Getters/setters
    TMCoefArray& getRefCoefficients ();
    const TMCoefArray& getCoefficients () const;
    void setCoefficients (const TMCoefArray& _vCoefficients);

    /**
     * @{
     * @brief Get/Set value of the coefficient in the given position (matrix[l][c])
     */
    FWDATA_API TM3DType getCoefficient(size_t l, size_t c) const;
    FWDATA_API void setCoefficient(size_t l, size_t c, TM3DType val);
    /// @}

    /// maximum size of the matrix (MATRIX_SIZE x MATRIX_SIZE)
    static const size_t MATRIX_SIZE = 4;

    /// Print the coefficients of the matrix
    friend std::ostream& operator<<(std::ostream& s, const TransformationMatrix3D& mat)
    {
        for(size_t l = 0; l < MATRIX_SIZE; l++)
        {
            for(size_t c = 0; c < MATRIX_SIZE; c++)
            {
                s << mat.getCoefficient(l, c) << "\t";
            }
            s << std::endl;
        }
        return s;
    }

protected:

    //! Matrix coefficient number (4x4). m_vCoefficients[0] to m_vCoefficients[3] is the first row of the matrix
    TMCoefArray m_vCoefficients;
};

//-----------------------------------------------------------------------------

inline TransformationMatrix3D::TMCoefArray& TransformationMatrix3D::getRefCoefficients()
{
    return this->m_vCoefficients;
}

//-----------------------------------------------------------------------------

inline const TransformationMatrix3D::TMCoefArray& TransformationMatrix3D::getCoefficients() const
{
    return this->m_vCoefficients;
}

//-----------------------------------------------------------------------------

inline void TransformationMatrix3D::setCoefficients(const TransformationMatrix3D::TMCoefArray& _vCoefficients)
{
    this->m_vCoefficients = _vCoefficients;
}

//------------------------------------------------------------------------------

inline TransformationMatrix3D::TM3DType TransformationMatrix3D::getCoefficient(size_t l, size_t c) const
{
    size_t pos = l * MATRIX_SIZE + c;
    return m_vCoefficients.at(pos);
}

//------------------------------------------------------------------------------

inline void TransformationMatrix3D::setCoefficient(size_t l, size_t c, TransformationMatrix3D::TM3DType val)
{
    size_t pos = l * MATRIX_SIZE + c;
    m_vCoefficients.at(pos) = val;
}

//-----------------------------------------------------------------------------

} // namespace fwData

#endif // __FWDATA_TRANSFORMATIONMATRIX3D_HPP__

