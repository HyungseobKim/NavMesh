/*!*******************************************************************
\headerfile   Color.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for Color struct and some pre-defined colors.
********************************************************************/
#ifndef COLOR_H
#define COLOR_H

/*!*******************************************************************
\struct Color
\brief
	   Stores value of color as r, g, b channels.
********************************************************************/
struct Color
{
	/*!*******************************************************************
	\brief
		   Constructor that initializes r, g, b.

	\param r
		   Value of red channel.

	\param g
		   Value of green channel.

	\param b
		   Value of blue channel.
	********************************************************************/
	Color(float r = 0.f, float g = 0.f, float b = 0.f)
		: r(r), g(g), b(b) {}

	float r = 0.f; //!< Color channel red.
	float g = 0.f; //!< Color channel green.
	float b = 0.f; //!< Color channel blue.
};

const Color WHITE(1.f, 1.f, 1.f),
RED(1.f, 0.21f, 0.153f),
GREEN(0.f, 1.f, 0.f),
BLUE(0.12f, 0.65f, 0.9f),
BLACK(0.f, 0.f, 0.f),
YELLOW(1.f, 0.95f, 0.f),
PURPLE(0.72f, 0.24f, 0.73f),
ORANGE(1.f, 0.8f, 0.1f),
GRAY(0.86f, 0.86f, 0.86f);

#endif // !COLOR_H
