/*!*******************************************************************
\headerfile   Client.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2020/01/04
\brief
			  Header file for Client class which manages
			  whole framework.
********************************************************************/
#ifndef CLIENT_H
#define CLIENT_H

#include <SDL2/SDL.h>

#include "Render.h"
#include "World.h"

/*!*******************************************************************
\class Client
\brief
	   Having methods to handle input and control all features of
	   this demo.
********************************************************************/
class Client
{
public:
	/*!*******************************************************************
	\brief
		   Constructor that initializes all necessary variables.

	\param window
		   Window of this demo.

	\param width
		   Width of window.

	\param height
		   Height of window.

	\param start
		   Start position of path.

	\param end
		   End position of path.
	********************************************************************/
	Client(SDL_Window* window, int width, int height, const Point& start, const Point& end);
	/*!*******************************************************************
	\brief
		   Destructor that releases all allocated memory.
	********************************************************************/
	~Client(void);

	/*!*******************************************************************
	\brief
		   Abstracted update method.
		   Convert mouse position and call related update method of 
		   object under hierarchy.

	\param mouseX
		   X-coordinate of mouse in SDL space.

	\param mouseY
		   Y-coordinate of mouse in SDL space.
	********************************************************************/
	void Update(int mouseX, int mouseY);
	/*!*******************************************************************
	\brief
		   Abstracted draw method.
		   Show fps with title and call related draw method of object
		   under hierarchy.

	\param dt
		   Delta time.

	\param editMode
		   Passing value to the underlying object.

	\param showAllNodes
		   Passing value to the underlying object.

	\param smoothPath
		   Passing value to the underlying object.
	********************************************************************/
	void draw(double dt, bool editMode, bool showAllNodes, bool smoothPath);

	/*!*******************************************************************
	\brief
		   Abstracted handling mouse motion method.
		   If it is edit mode, than convert mouse position and call 
		   related method of object under hierarchy.

	\param event
		   Information about mouse position.

	\param editMode
		   If it is false, do nothing.
	********************************************************************/
	void MouseMotion(const SDL_Event& event, bool editMode);
	/*!*******************************************************************
	\brief
		   Abstracted method when pressed mouse is released.
		   Call related method of object under hierarchy.
	********************************************************************/
	void MouseUp();
	/*!*******************************************************************
	\brief
		   Abstracted method handling pressing mouse input.
		   Call related method of object under hierarchy depends on mode.

	\param event
		   Information about mouse.

	\param editMode
		   If it is in true, just call handling mouse down input method.
		   Otherwise, set new vertex of path depends on which mouse button
		   is down.
	********************************************************************/
	void MouseDown(const SDL_Event& event, bool editMode);

	/*!*******************************************************************
	\brief
		   When window resized, resize viewport as following that.

	\param width
		   New width of window.

	\param height
		   New height of window.
	********************************************************************/
	void resize(int width, int height);
	
	/*!*******************************************************************
	\brief
		   Abstracted method for handling editor input.
		   Call related method of object under hierarchy.
	********************************************************************/
	void AddHole();
	/*!*******************************************************************
	\brief
		   Abstracted method called when user escapes edit mode.
		   Call related method of object under hierarchy.
	********************************************************************/
	void GenerateNavMesh();

	/*!*******************************************************************
	\brief
		   Abstracted setter method for weight.

	\param weight
		   New weight.
	********************************************************************/
	void SetWeight(float weight);
	/*!*******************************************************************
	\brief
		   Abstracted setter method for color of considered nodes.

	\param color
		   New color.
	********************************************************************/
	void SetConsideredColor(const Color& color);
	/*!*******************************************************************
	\brief
		   Abstracted setter method for color of visited nodes.

	\param color
		   New color.
	********************************************************************/
	void SetVisitedColor(const Color& color);
	/*!*******************************************************************
	\brief
		   Abstracted setter method for color of path.

	\param color
		   New color.
	********************************************************************/
	void SetPathColor(const Color& color);
	/*!*******************************************************************
	\brief
		   Abstracted setter method for color of navigation meshes.

	\param color
		   New color.
	********************************************************************/
	void SetNavMeshColor(const Color& color);

private:
	/*!*******************************************************************
	\brief
		   Helper method that convert mouse position from SDL space to
		   viewport space.

	\param mouse
		   Mouse position in SDL space.

	\return Point
			Mouse position in viewport space.
	********************************************************************/
	Point ConvertMousePosition(const Point& mouse) const;

private:
	SDL_Window* m_window; //!< Pointer to SDL window.
	int m_frame_count = 0; //!< The number of frame passed from last update.
	double m_frame_time = 0;  //!< How much time passed from last update.

	int m_width; //!< Width of window and viewport.
	int m_height; //!< Height of window and viewport.

	Render* m_render; //!< Allocated render that has drawing methods.
	World m_world; //!< Object under hierarchy. Handles world space.
};

#endif // !CLIENT_H

