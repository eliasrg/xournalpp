/*
 * Xournal++
 *
 * This file is part of the Xournal UnitTests
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#include "util/XojPreviewExtractor.h"

#include <ctime>
#include "stdlib.h"
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class XojPreviewExtractorTest: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(XojPreviewExtractorTest);

	CPPUNIT_TEST(testNonExistingFile);
	CPPUNIT_TEST(testLoadGzipped);
	CPPUNIT_TEST(testLoadGzipped2);
	CPPUNIT_TEST(testLoad1Unzipped);
	CPPUNIT_TEST(testNoPreview);
	CPPUNIT_TEST(testInvalidFile);

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
	}

	void tearDown()
	{
	}

	void testNonExistingFile()
	{
		XojPreviewExtractor extractor;
		PreviewExtractResult result = extractor.readFile("testfiles/THIS FILE DOES NOT EXIST.xoj");

		CPPUNIT_ASSERT_EQUAL(PREVIEW_RESULT_COULD_NOT_OPEN_FILE, result);
	}

	void testLoadGzipped()
	{
		XojPreviewExtractor extractor;
		PreviewExtractResult result = extractor.readFile("testfiles/preview-test.xoj");

		CPPUNIT_ASSERT_EQUAL(PREVIEW_RESULT_IMAGE_READ, result);

		CPPUNIT_ASSERT_EQUAL((gsize)17, extractor.getDataLength());
		CPPUNIT_ASSERT_EQUAL(string("CppUnitTestString"), string((const char*)extractor.getData()));
	}

	void testLoadGzipped2()
	{
		XojPreviewExtractor extractor;
		PreviewExtractResult result = extractor.readFile("testfiles/preview-test2.xoj");

		CPPUNIT_ASSERT_EQUAL(PREVIEW_RESULT_IMAGE_READ, result);

		CPPUNIT_ASSERT_EQUAL((gsize)2856, extractor.getDataLength());
	}

	void testLoad1Unzipped()
	{
		XojPreviewExtractor extractor;
		PreviewExtractResult result = extractor.readFile("testfiles/preview-test.unzipped.xoj");

		CPPUNIT_ASSERT_EQUAL(PREVIEW_RESULT_IMAGE_READ, result);

		CPPUNIT_ASSERT_EQUAL((gsize)17, extractor.getDataLength());
		CPPUNIT_ASSERT_EQUAL(string("CppUnitTestString"), string((const char*)extractor.getData()));
	}

	void testNoPreview()
	{
		XojPreviewExtractor extractor;
		PreviewExtractResult result = extractor.readFile("testfiles/preview-test-no-preview.unzipped.xoj");

		CPPUNIT_ASSERT_EQUAL(PREVIEW_RESULT_NO_PREVIEW, result);
	}

	void testInvalidFile()
	{
		XojPreviewExtractor extractor;
		PreviewExtractResult result = extractor.readFile("testfiles/preview-test-invalid.xoj");

		CPPUNIT_ASSERT_EQUAL(PREVIEW_RESULT_ERROR_READING_PREVIEW, result);
	}

};

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(XojPreviewExtractorTest);
