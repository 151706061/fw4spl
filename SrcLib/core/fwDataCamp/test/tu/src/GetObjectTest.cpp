/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <fwData/Composite.hpp>
#include <fwData/Float.hpp>
#include <fwData/Image.hpp>
#include <fwData/Integer.hpp>
#include <fwData/Reconstruction.hpp>
#include <fwData/String.hpp>
#include <fwData/Vector.hpp>

#include <fwDataCamp/getObject.hpp>
#include <fwDataCamp/exception/NullPointer.hpp>
#include <fwDataCamp/exception/ObjectNotFound.hpp>

#include <fwMedData/ImageSeries.hpp>
#include <fwMedData/Patient.hpp>

#include <fwTest/generator/Image.hpp>

#include "GetObjectTest.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION( ::fwDataCamp::ut::GetObjectTest );

namespace fwDataCamp
{
namespace ut
{

//-----------------------------------------------------------------------------

// Set up context before running a test.
void GetObjectTest::setUp()
{
}

//-----------------------------------------------------------------------------

void GetObjectTest::tearDown()
{
    // Clean up after the test run.
}

//-----------------------------------------------------------------------------

void GetObjectTest::getTest()
{
    // Visit 1
    ::fwData::Image::sptr img1 = ::fwData::Image::New();
    ::fwTest::generator::Image::generateRandomImage(img1, ::fwTools::Type::create("int16"));
    ::fwData::Image::sptr img2 = ::fwData::Image::New();
    ::fwTest::generator::Image::generateRandomImage(img2, ::fwTools::Type::create("uint8"));
    ::fwData::Composite::sptr composite = ::fwData::Composite::New();
    composite->getContainer()["img1"]   = img1;
    composite->getContainer()["img2"]   = img2;
    ::fwData::Object::sptr subObj1      = ::fwDataCamp::getObject( composite, "@values.img2" );
    CPPUNIT_ASSERT_MESSAGE("Image must be equal", subObj1 == img2);

    // Visit 2
    ::fwData::Float::sptr zspacing = ::fwDataCamp::getObject< ::fwData::Float >( composite, "@values.img2.spacing.2" );
    CPPUNIT_ASSERT_MESSAGE("spacing must be equal",
                           img2->getSpacing()[2] - 0.001 < zspacing->value() &&
                           zspacing->value() < img2->getSpacing()[2] + 0.001 );

    // Visit 3
    ::fwMedData::Patient::sptr patient1 = ::fwMedData::Patient::New();
    patient1->setName( "toto" );
    ::fwData::String::sptr str = ::fwDataCamp::getObject< ::fwData::String >( patient1, "@name" );
    CPPUNIT_ASSERT_MESSAGE("Name must be equal", patient1->getName() == str->value() );

    // Visit 4
    composite->setField("toto", img1);
    img1->setField("titi", img2);
    ::fwData::Object::sptr subObj2 = ::fwDataCamp::getObject( composite, "@fields.toto.fields.titi" );
    CPPUNIT_ASSERT_MESSAGE("Image must be equal", subObj2 == img2 );
}

//-----------------------------------------------------------------------------

void GetObjectTest::invalidPathTest()
{
    ::fwData::Composite::sptr composite = ::fwData::Composite::New();
    ::fwData::String::sptr text         = ::fwData::String::New("Text");
    (*composite)["string"]              = text;
    ::fwData::Integer::sptr intValue    = ::fwData::Integer::New(321);
    ::fwData::Float::sptr floatValue    = ::fwData::Float::New(1.234f);
    ::fwData::Vector::sptr vector       = ::fwData::Vector::New();
    ::fwData::Image::sptr img           = ::fwData::Image::New();
    ::fwTest::generator::Image::generateRandomImage(img, ::fwTools::Type::create("int16"));

    auto& internalVector = vector->getContainer();
    internalVector.push_back(intValue);
    internalVector.push_back(floatValue);
    internalVector.push_back(img);
    (*composite)["vector"] = vector;

    // no exception version
    ::fwData::Object::sptr obj = ::fwDataCamp::getObject( composite, "@values.string" );
    CPPUNIT_ASSERT_MESSAGE("fwData::String must be equal", obj ==  text );

    // with exception version
    obj = ::fwDataCamp::getObject( composite, "@values.string", true );
    CPPUNIT_ASSERT_MESSAGE("fwData::String must be equal", obj ==  text );

    // no exception version
    ::fwData::Object::sptr invalidObj = ::fwDataCamp::getObject( composite, "@values.invalidPath", false );
    CPPUNIT_ASSERT_MESSAGE("Object must not exist", !invalidObj );

    // exception version : path is invalid
    CPPUNIT_ASSERT_THROW(
        ::fwDataCamp::getObject( composite, "@values.invalidPath", true ),
        ::fwDataCamp::exception::ObjectNotFound
        );
    CPPUNIT_ASSERT_EQUAL(size_t(2), composite->size() );

    ::fwMedData::ImageSeries::sptr imgSeries = ::fwMedData::ImageSeries::New();
    // no exception version
    invalidObj = ::fwDataCamp::getObject( imgSeries, "@image.type", false );
    CPPUNIT_ASSERT_MESSAGE("Object must not exist", !invalidObj );

    // exception version : path exist, but image object is null
    CPPUNIT_ASSERT_THROW(
        ::fwDataCamp::getObject( imgSeries, "@image.type", true ),
        ::fwDataCamp::exception::NullPointer
        );

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Vector tests

    // no exception version
    obj = ::fwDataCamp::getObject( composite, "@values.vector.values.0" );
    CPPUNIT_ASSERT_MESSAGE("fwData::Integer must be equal", obj ==  intValue );

    // with exception version
    obj = ::fwDataCamp::getObject( composite, "@values.vector.values.0", true );
    CPPUNIT_ASSERT_MESSAGE("fwData::Integer must be equal", obj ==  intValue );

    /// This is important to test vectors subobjects properties to ensure the visitor path is correct

    // no exception version
    obj                            = ::fwDataCamp::getObject( composite, "@values.vector.values.2.spacing.2" );
    ::fwData::Float::sptr zspacing = ::std::dynamic_pointer_cast< ::fwData::Float >(obj);
    CPPUNIT_ASSERT_MESSAGE("spacing must be equal",
                           img->getSpacing()[2] - 0.001 < zspacing->value() &&
                           zspacing->value() < img->getSpacing()[2] + 0.001 );

    // with exception version
    obj      = ::fwDataCamp::getObject( composite, "@values.vector.values.2.spacing.2", true );
    zspacing = ::std::dynamic_pointer_cast< ::fwData::Float >(obj);
    CPPUNIT_ASSERT_MESSAGE("spacing must be equal",
                           img->getSpacing()[2] - 0.001 < zspacing->value() &&
                           zspacing->value() < img->getSpacing()[2] + 0.001 );

    // out of bounds, no exception version
    invalidObj = ::fwDataCamp::getObject( composite, "@values.vector.values.2.spacing.15", false );
    CPPUNIT_ASSERT_MESSAGE("Object must not exist", !invalidObj );

    // out of bounds, with exception version
    CPPUNIT_ASSERT_THROW( ::fwDataCamp::getObject( composite, "@values.vector.values.2.spacing.15", true ),
                          ::fwDataCamp::exception::NullPointer);

}

//-----------------------------------------------------------------------------

}  // namespace ut
}  // namespace fwDataCamp
