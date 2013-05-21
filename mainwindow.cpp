#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "utf8.h"
#include "arabtype.h"

FT_Library MainWindow::ftlib__	= NULL;



uint32_t	get_arabic_form(const std::vector<uint32_t>& code_point, uint32_t idx)
{
	uint	ch	= code_point[idx];
	uint	prev	= 0;
	uint	next	= 0;

	if( idx )
		prev	= code_point[idx - 1];
	if( idx < code_point.size() - 1 )
		next	= code_point[idx + 1];

	return get_presentation_form_b(prev, next, ch);
}

std::vector<uint32_t> decode(const std::vector<uint8_t>& in)
{
	std::vector<uint>	ret;
	uint codep	= 0;
	uint state	= 0;

	for( size_t i = 0; i < in.size(); ++i )
	{
		if( decode(&state, &codep, in[i]) == UTF8_ACCEPT )
			ret.push_back(codep);
	}
	if( state != UTF8_ACCEPT )
		std::cout << "The string is not well-formed" << std::endl;

	return ret;
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui__(new Ui::MainWindow)
{
	ui__->setupUi(this);

	std::ifstream	ifs("arabic_test.txt");
	assert( ifs.is_open() );
	std::vector<uchar>	arabic_string;

	while( ifs.good() )
	{
		uchar ch = ifs.get();
		if( ifs.good() )
			arabic_string.push_back(ch);
	}

	std::cout << "size: " << arabic_string.size() << std::endl;
	ifs.close();

	std::vector<uint> arabic_cp	= decode(arabic_string);

	for( size_t i = 0; i < arabic_cp.size(); ++i )
		std::cout << "0x" << std::hex << arabic_cp[i] << std::endl;

	std::cout << std::dec;

	if( !ftlib__ )
	{
		FT_Error	error = FT_Init_FreeType(&ftlib__);
		assert( !error && "Error loading freetype" );
	}

	// load the font
	FT_Face		face;
	FT_Error	error;

	error		= FT_New_Face(ftlib__, "ae_Tholoth.ttf", 0, &face);
	assert( !error && "unable to load font file" );

	// set font size
	error		= FT_Set_Pixel_Sizes(face, 0, 24);
	assert( !error && "Error setting font size" );

	// setup glyph
	// render chars
	FT_GlyphSlot	slot		= face->glyph;

	FT_Int32	load_flags	= FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT;
	FT_Render_Mode	render_flags	= FT_RENDER_MODE_NORMAL;


	data__	= new uchar[1024 * 256 * 4];
	memset(data__, 0, 1024 * 256 * 4);
	int	col	= 1024 - 1;
	int	line	= 100;
	QImage	img(data__, 1024, 256, QImage::Format_RGB32);
	// render the arabic glyphs
	for( size_t idx = 0; idx < arabic_cp.size(); ++idx )
	{
		uint ch	= get_arabic_form(arabic_cp, idx);
		if( ch == 0xA || ch == 0xC )
			continue;

		int glyph_index	= FT_Get_Char_Index(face, ch);
		assert( glyph_index && "invalid character" );

		error	= FT_Load_Glyph(face, glyph_index, load_flags);
		assert( !error && "couldn't load glyph" );

		error = FT_Render_Glyph(slot, render_flags);
		assert( !error && "couldn't render glyph" );

		FT_Glyph glyph;
		FT_Get_Glyph(slot, &glyph);

		FT_BBox	box;
		FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &box);

		int	temp_col	= col - (slot->advance.x >> 6);//slot->bitmap.width;

		for( int y = 0; y < slot->bitmap.rows; y++ )
		{
			for( int x = 0; x < slot->bitmap.width; x++ )
			{
				uchar	co	= slot->bitmap.buffer[x + y * slot->bitmap.pitch];
				if( co && x + temp_col < 1024 && x + temp_col >= 0 )
				{
					uint rgb = img.pixel(x + temp_col + slot->bitmap_left, y - slot->bitmap_top + line);

					img.setPixel(x + temp_col + slot->bitmap_left, y - slot->bitmap_top + line, rgb | qRgb(co, co, co));
				}

				//std::cout << uint(co) << " ";
			}
			//std::cout << std::endl;
		}

		//std::cout << std::endl;

		col -= slot->advance.x >> 6;

		FT_Done_Glyph(glyph);
	}


	for( int i = 0; i < 1024; ++i )
		//data__[i + 128 * 512 * 4 + 1]	= 0xFF;
		img.setPixel(i, 128, 0xFF000000);
	QPixmap	pixmap = QPixmap::fromImage(img);
	ui__->label->setPixmap(pixmap);

	FT_Done_Face(face);
}

MainWindow::~MainWindow()
{
	FT_Done_FreeType(ftlib__);
	delete ui__;
	delete data__;
}

void MainWindow::paintEvent ( QPaintEvent * event )
{
}
