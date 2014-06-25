/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2013 Blender Foundation.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): Jason Wilkins
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file ghost/intern/GHOST_Context.h
 *  \ingroup GHOST
 * Declaration of GHOST_Context class.
 */

#ifndef __GHOST_CONTEXT_H__
#define __GHOST_CONTEXT_H__

#include "GHOST_Types.h"

#include "glew-mx.h"

#include <cstdlib> // for NULL



class GHOST_Context
{
public:
	/**
	 * Constructor.
	 * \param stereoVisual		Stereo visual for quad buffered stereo.
	 * \param numOfAASamples	Number of samples used for AA (zero if no AA)
	 */
	GHOST_Context(bool stereoVisual, GHOST_TUns16 numOfAASamples)
		: m_stereoVisual  (stereoVisual)
		, m_numOfAASamples(numOfAASamples)
		, m_glewContext(NULL)
	{}

	/**
	 * Destructor.
	 */
	virtual ~GHOST_Context() {
		glewDestroyContext(m_glewContext);
	}

	/**
	 * Swaps front and back buffers of a window.
	 * \return  A boolean success indicator.
	 */
	virtual GHOST_TSuccess swapBuffers() = 0;

	/**
	 * Activates the drawing context of this window.
	 * \return  A boolean success indicator.
	 */
	virtual GHOST_TSuccess activateDrawingContext() = 0;

	/**
	 * Call immediately after new to initialize.  If this fails then immediately delete the object.
	 * \return Indication as to whether initialization has succeeded.
	 */
	virtual GHOST_TSuccess initializeDrawingContext() = 0;

	/**
	 * Updates the drawing context of this window. Needed
	 * whenever the window is changed.
	 * \return Indication of success.
	 */
	virtual GHOST_TSuccess updateDrawingContext() {
		return GHOST_kFailure;
	}

	/**
	 * Checks if it is OK for a remove the native display
	 * \return Indication as to whether removal has succeeded.
	 */
	virtual GHOST_TSuccess releaseNativeHandles() = 0;

	/**
	 * Sets the swap interval for swapBuffers.
	 * \param interval The swap interval to use.
	 * \return A boolean success indicator.
	 */
	virtual GHOST_TSuccess setSwapInterval(int interval) {
		return GHOST_kFailure;
	}

	/**
	 * Gets the current swap interval for swapBuffers.
	 * \return An integer.
	 */
	virtual int getSwapInterval() {
		return 1;
	}

	/** Stereo visual created. Only necessary for 'real' stereo support,
	 *  ie quad buffered stereo. This is not always possible, depends on
	 *  the graphics h/w
	 */
	inline bool isStereoVisual() const {
		return m_stereoVisual;
	}

	/** Number of samples used in anti-aliasing, set to 0 if no AA **/
	inline GHOST_TUns16 getNumOfAASamples() const {
		return m_numOfAASamples;
	}

protected:
	void initContextGLEW();

	void activateGLEW() const {
		glewSetContext(m_glewContext);
	}

	bool m_stereoVisual;
	
	GHOST_TUns16 m_numOfAASamples;

private:
	GLEWContext* m_glewContext;

};



GLenum glew_chk(GLenum error, const char* file, int line, const char* text);

#ifndef NDEBUG
#define GLEW_CHK(x) glew_chk((x), __FILE__, __LINE__, #x)
#else
#define GLEW_CHK(x) x
#endif



#endif // __GHOST_CONTEXT_H__
