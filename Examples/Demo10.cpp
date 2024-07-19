#include <cmath>
#include <iostream>
#include <string>

#include <OpenXLSX.hpp>
#include <external/pugixml/pugixml.hpp>

bool nowide_status()
{
#ifdef ENABLE_NOWIDE
	return true;
#else
	return false;
#endif
}

using namespace std;
using namespace OpenXLSX;

int main( int argc, char *argv[] )
{
	std::cout << "nowide is " << ( nowide_status() ? "enabled" : "disabled" ) << std::endl;

	/* DONE: format support for XLCells (cellFormat / setCellFormat)
	 * TODO: update style elements "count" attribute for all arrays when saving
	 * TODO: format support for XLRows: <row> attributes s (same as cell attribute) and customFormat (=true/false)
	 *       it appears that XLRow style is used to overwrite existing cell formats, and then for whenever a new cell is created
	 *         -> could support the same in OpenXLSX - possibly also for columns
	 * TODO TBD: permit setting a format reference for shared strings
	 */
	
	// load example file
	XLDocument doc("./test.xlsx"s);

	std::cout << "doc.name() is " << doc.name() << std::endl;

	size_t nodeIndex;

	XLNumberFormats & numberFormats = doc.styles().numberFormats();
	XLFonts & fonts = doc.styles().fonts();
	XLFills & fills = doc.styles().fills();
	XLBorders & borders = doc.styles().borders();
	XLCellFormats & cellStyleFormats = doc.styles().cellStyleFormats();
	XLCellFormats & cellFormats = doc.styles().cellFormats();
	XLCellStyles & cellStyles = doc.styles().cellStyles();

	XLWorksheet wks = doc.workbook().worksheet("Containers");
	size_t cellFormatIndexA2 = wks.cell("A2").cellFormat();                 // get index of cell format
	size_t fontIndexA2 = cellFormats[ cellFormatIndexA2 ].fontIndex();      // get index of used font

	size_t newFontIndex = fonts.create( fonts[ fontIndexA2 ] );             // create a new font based on used font
	size_t newCellFormatIndex                                               // create a new cell format
		= cellFormats.create( cellFormats[ cellFormatIndexA2 ] );            //              based on used format

	cellFormats[ newCellFormatIndex ].setFontIndex( newFontIndex );         // assign the new font to the new cell format
	wks.cell("A2").setCellFormat( newCellFormatIndex );                     // assign the new cell format to the cell

	//                                                                      // change the new font style:
	fonts[ newFontIndex ].setBold();                                        // bold
	fonts[ newFontIndex ].setItalic();                                      // italic
	fonts[ newFontIndex ].setUnderline(XLUnderlineDouble);                  // underline: XLUnderlineSingle, XLUnderlineDouble, XLUnderlineNone
	fonts[ newFontIndex ].setUnderline(XLUnderlineNone);                    // TEST: un-set the underline status
	fonts[ newFontIndex ].setStrikethrough();                               // strikethrough
	XLColor red  ( "ffff0000" );
	XLColor green( "ff00ff00" );
	XLColor blue ( "ff0000ff" );
	fonts[ newFontIndex ].setFontColor( green );

	cellFormats[ newCellFormatIndex ].setApplyFont( true );                 // apply the font (seems to do nothing)


	bool testBasics = false;
	if( testBasics ) {
		std::cout << "   numberFormats.count() is " <<    numberFormats.count() << std::endl;
		std::cout << "           fonts.count() is " <<            fonts.count() << std::endl;
		std::cout << "           fills.count() is " <<            fills.count() << std::endl;
		std::cout << "         borders.count() is " <<          borders.count() << std::endl;
		std::cout << "cellStyleFormats.count() is " << cellStyleFormats.count() << std::endl;
		std::cout << "     cellFormats.count() is " <<      cellFormats.count() << std::endl;
		std::cout << "      cellStyles.count() is " <<       cellStyles.count() << std::endl;


		bool createAll = false;

		nodeIndex = ( createAll ? numberFormats.create() : numberFormats.count() - 1 );
		numberFormats[ nodeIndex ].setFormatId( 15 );
		numberFormats[ nodeIndex ].setFormatCode( "fifteen" );
		std::cout << "numberFormats[ " << nodeIndex << " ] summary: " << numberFormats[ nodeIndex ].summary() << std::endl;

		nodeIndex = ( createAll ? fonts.create() : fonts.count() - 1 );
		fonts[ nodeIndex ].setFontSize( 12 );
		fonts[ nodeIndex ].setFontColor( XLColor( XLDefaultFontColor ) );
		fonts[ nodeIndex ].setFontName( "Twelve" );
		fonts[ nodeIndex ].setFontFamily( 13 );
		fonts[ nodeIndex ].setFontCharset( 14 );
		std::cout << "fonts[ " << nodeIndex << " ] summary: " << fonts[ nodeIndex ].summary() << std::endl;

		nodeIndex = ( createAll ? fills.create() : fills.count() - 1 );
		fills[ nodeIndex ].setFillType( XLDefaultFillType );
		fills[ nodeIndex ].setPatternType( XLDefaultFillPatternType );
		fills[ nodeIndex ].setColor          ( XLColor( XLDefaultFillPatternFgColor ) );
		fills[ nodeIndex ].setBackgroundColor( XLColor( XLDefaultFillPatternBgColor ) );
		std::cout << "fills[ " << nodeIndex << " ] summary: " << fills[ nodeIndex ].summary() << std::endl;

		nodeIndex = ( createAll ? borders.create() : borders.count() - 1 );
		borders[ nodeIndex ].setDiagonalUp( false );
		borders[ nodeIndex ].setDiagonalDown( true );
		borders[ nodeIndex ].setLine    ( XLLineBottom, XLLineStyleThin, XLColor( XLDefaultFontColor ) );
		borders[ nodeIndex ].setLeft    (               XLLineStyleNone, XLColor( "ff112222" )         );
		borders[ nodeIndex ].setRight   (               XLLineStyleThin, XLColor( "ff113333" )         );
		borders[ nodeIndex ].setTop     (               XLLineStyleThin, XLColor( "ff114444" )         );
		borders[ nodeIndex ].setBottom  (               XLLineStyleNone, XLColor( "ff222222" )         );
		borders[ nodeIndex ].setDiagonal(               XLLineStyleThin, XLColor( "ff332222" )         );
		std::cout << "borders[ " << nodeIndex << " ] summary: " << borders[ nodeIndex ].summary() << std::endl;

		nodeIndex = ( createAll ? cellStyleFormats.create() : cellStyleFormats.count() - 1 );
		cellStyleFormats[ nodeIndex ].setNumberFormatId( 5 );
		cellStyleFormats[ nodeIndex ].setFontIndex( 6 );
		cellStyleFormats[ nodeIndex ].setFillIndex( 7 );
		cellStyleFormats[ nodeIndex ].setBorderIndex( 8 );
		cellStyleFormats[ nodeIndex ].setApplyFont(true);
		cellStyleFormats[ nodeIndex ].setApplyFill(false);
		cellStyleFormats[ nodeIndex ].setApplyBorder(false);
		cellStyleFormats[ nodeIndex ].setApplyAlignment(true);
		cellStyleFormats[ nodeIndex ].setApplyProtection(false);
		cellStyleFormats[ nodeIndex ].setLocked(true);
		cellStyleFormats[ nodeIndex ].setHidden(false);
		cellStyleFormats[ nodeIndex ].alignment(XLCreateIfMissing).setHorizontal(XLAlignRight);
		cellStyleFormats[ nodeIndex ].alignment(XLCreateIfMissing).setVertical(XLAlignCenter);
		cellStyleFormats[ nodeIndex ].alignment(XLCreateIfMissing).setTextRotation(255);
		cellStyleFormats[ nodeIndex ].alignment(XLCreateIfMissing).setWrapText(true);
		cellStyleFormats[ nodeIndex ].alignment(XLCreateIfMissing).setIndent(256);
		cellStyleFormats[ nodeIndex ].alignment(XLCreateIfMissing).setShrinkToFit(false);

		std::cout << "cellStyleFormats[ " << nodeIndex << " ] summary: " << cellStyleFormats[ nodeIndex ].summary() << std::endl;

		nodeIndex = ( createAll ? cellFormats.create() : cellFormats.count() - 1 );
		cellFormats[ nodeIndex ].setNumberFormatId( 22 );
		cellFormats[ nodeIndex ].setFontIndex( 23 );
		cellFormats[ nodeIndex ].setFillIndex( 24 );
		cellFormats[ nodeIndex ].setBorderIndex( 25 );
		cellFormats[ nodeIndex ].setApplyFont(false);
		cellFormats[ nodeIndex ].setApplyFill(true);
		cellFormats[ nodeIndex ].setApplyBorder(true);
		cellFormats[ nodeIndex ].setApplyAlignment(false);
		cellFormats[ nodeIndex ].setApplyProtection(true);
		cellFormats[ nodeIndex ].setLocked(false);
		cellFormats[ nodeIndex ].setHidden(true);
		cellFormats[ nodeIndex ].alignment(XLCreateIfMissing).setHorizontal(XLAlignGeneral);
		cellFormats[ nodeIndex ].alignment(XLCreateIfMissing).setVertical(XLAlignTop);
		cellFormats[ nodeIndex ].alignment(XLCreateIfMissing).setTextRotation(370);
		cellFormats[ nodeIndex ].alignment(XLCreateIfMissing).setWrapText(false);
		cellFormats[ nodeIndex ].alignment(XLCreateIfMissing).setIndent(37);
		cellFormats[ nodeIndex ].alignment(XLCreateIfMissing).setShrinkToFit(true);
		std::cout << "cellFormats[ " << nodeIndex << " ] summary: " << cellFormats[ nodeIndex ].summary() << std::endl;

		nodeIndex = ( createAll ? cellStyles.create() : cellStyles.count() - 1 );
		cellStyles[ nodeIndex ].setName( "test cellStyle" );
		cellStyles[ nodeIndex ].setXfId( 77 );
		cellStyles[ nodeIndex ].setBuiltinId( 7 );
		std::cout << "cellStyles[ " << nodeIndex << " ] summary: " << cellStyles[ nodeIndex ].summary() << std::endl;


		std::cout << "   numberFormats.count() is " <<    numberFormats.count() << std::endl;
		std::cout << "           fonts.count() is " <<            fonts.count() << std::endl;
		std::cout << "           fills.count() is " <<            fills.count() << std::endl;
		std::cout << "         borders.count() is " <<          borders.count() << std::endl;
		std::cout << "cellStyleFormats.count() is " << cellStyleFormats.count() << std::endl;
		std::cout << "     cellFormats.count() is " <<      cellFormats.count() << std::endl;
		std::cout << "      cellStyles.count() is " <<       cellStyles.count() << std::endl;
	} // end if( testBasics )

	
	doc.saveAs("./test-out.xlsx");
	doc.close();

	return 0;
}
