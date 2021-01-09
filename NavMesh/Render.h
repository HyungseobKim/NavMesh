/*!*******************************************************************
\headerfile   Render.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for Render class which methods for setup 
			  OpenGL and drawing.
********************************************************************/
#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "Mesh.h"
#include "Color.h"

/*!*******************************************************************
\class Astar
\brief
	   Renderer for 2D meshes and lines. Using OpenGL.
********************************************************************/
class Render
{
public:
	/*!*******************************************************************
	\brief
		   Constructor that initializes width and height of world space.

	\param width
		   Width of world space.

	\param height
		   Height of world space.
	********************************************************************/
	Render(float width, float height);
	/*!*******************************************************************
	\brief
		   Destructor that deletes buffers and programs used or OpenGL.
	********************************************************************/
	~Render();

	/*!*******************************************************************
	\brief
		   Clears buffer with default color.
	********************************************************************/
	void ClearBuffers();

	/*!*******************************************************************
	\brief
		   Draws given mesh.

	\param mesh
		   Mesh to draw.
	********************************************************************/
	void DrawMesh(const Mesh& mesh) const;
	/*!*******************************************************************
	\brief
		   Draws given line.

	\param start
		   One of vertices of the line.

	\param end
		   One of vertices of the line.

	\param color
		   Color of the line.

	\param width
		   Width of the line. Default value is 1.
	********************************************************************/
	void DrawLine(const Point& start, const Point& end, const Color& color, float width = 1.f);

private:
	/*!*******************************************************************
	\brief
		   Compile vertex shader and fragment shader, and attach them to
		   program. Stores locations of uniforms too.
	********************************************************************/
	void CompileShader();
	/*!*******************************************************************
	\brief
		   Allocates buffer and and set data for each shape, which are
		   triangle, box and line.
	********************************************************************/
	void Initialize();
	
	GLint m_program; //!< OpenGL program that will be used for this demo.
	GLint uColor; //!< Location of uniform color.
	GLint uWidthHeight; //!< Location of unifor array of two floats.

	const float m_WidthHeight[2]; // Transform from world space to OpenGL NDC space.
};

#endif // !RENDER_H
