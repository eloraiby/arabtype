/*
** Arabtype Copyright 2012-2015(c) Wael El Oraiby. All Rights Reserved
**
** This file is in public domain
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "../arabtype.h"

FT_Library MainWindow::ftlib__	= NULL;


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui__(new Ui::MainWindow)
{
	ui__->setupUi(this);

	int	width	= 480;
	int	height	= 392;
	int	font_size	= 24;

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


	uint32_t*	arabic_cp	= new uint32_t[arabic_string.size()];
	uint32_t	acp_size	= get_presentation_form_b(arabic_string.size(), &arabic_string[0], arabic_string.size() * sizeof(uint32_t), arabic_cp);

	for( size_t i = 0; i < acp_size; ++i )
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
	error		= FT_Set_Pixel_Sizes(face, 0, font_size);
	assert( !error && "Error setting font size" );

	// setup glyph
	// render chars
	FT_GlyphSlot	slot		= face->glyph;

	FT_Int32	load_flags	= FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT;
	FT_Render_Mode	render_flags	= FT_RENDER_MODE_NORMAL;


	data__	= new uchar[width * height * 4];
	memset(data__, 0, width * height * 4);
	int	col	= width - font_size;
	int	line	= font_size;
	QImage	img(data__, width, height, QImage::Format_RGB32);
	// render the arabic glyphs
	for( size_t idx = 0; idx < acp_size; ++idx )
	{
		uint ch	= arabic_cp[idx];

		if( ch == 0xA ) {
			line	+= font_size + 5;
			col	= width - font_size;
			continue;
		}

		if ( ch == 0xC ) {
			continue;
		}

		if ( ch == -1 ) {
			continue;
		}

		assert(ch != 0);

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

		int	temp_col	= col - (slot->advance.x >> 6);

		for( int y = 0; y < slot->bitmap.rows; y++ )
		{
			for( int x = 0; x < slot->bitmap.width; x++ )
			{
				uchar	co	= slot->bitmap.buffer[x + y * slot->bitmap.pitch];
				if( co && x + temp_col < width && x + temp_col >= 0 )
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

	delete[] arabic_cp;

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
