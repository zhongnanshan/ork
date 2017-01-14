/*
 * Ork: a small object-oriented OpenGL Rendering Kernel.
 * Website : http://ork.gforge.inria.fr/
 * Copyright (c) 2008-2015 INRIA - LJK (CNRS - Grenoble University)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 * may be used to endorse or promote products derived from this software without 
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * Ork is distributed under the BSD3 Licence. 
 * For any assistance, feedback and remarks, you can check out the 
 * mailing list on the project page : 
 * http://ork.gforge.inria.fr/
 */
/*
 * Main authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#include "ork/ui/GlutWindow.h"

#include "ork/core/Logger.h"

#include <GL/glew.h>

#ifdef USEFREEGLUT
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif

#include <assert.h>

#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP_BUTTON 0x0003
#endif

#ifndef GLUT_WHEEL_DOWN
#define GLUT_WHEEL_DOWN_BUTTON 0x0004
#endif

#include "DebugCallback.h"

using namespace std;

namespace ork
{



map<int, GlutWindow*> GlutWindow::INSTANCES;

GlutWindow::GlutWindow(const Parameters &params) : Window(params)
{
    int argc = 1;
    char *argv[1] = { (char*) "dummy" };
	int x = INSTANCES.size();
    if (x == 0) {
        glutInit(&argc, argv);
    }
    glutInitDisplayMode(GLUT_DOUBLE |
        (params.alpha() ? GLUT_ALPHA : 0) |
        (params.depth() ? GLUT_DEPTH : 0) |
        (params.stencil() ? GLUT_STENCIL : 0) |
        (params.multiSample() ? GLUT_MULTISAMPLE : 0));
	
    glGetError();

#ifdef USEFREEGLUT
    //Init OpenGL context
    glutInitContextVersion(params.version().x, params.version().y);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | (params.debug() ? GLUT_DEBUG : 0));
#endif

    glutInitWindowSize(params.width(), params.height());
    windowId = glutCreateWindow(params.name().c_str());
    size = vec2i(params.width(), params.height());
    damaged = false;
    timer.start();
    t = 0.0;
    dt = 0.0;

    if (params.width() == 0 && params.height() == 0) {
        glutFullScreen();
    }
    if (false) {
        glutCreateMenu(NULL);//do nothing, only used to avoid a warning
    }
     
    glGetError();


    INSTANCES[windowId] = this;
    glutDisplayFunc(redisplayFunc);
    glutReshapeFunc(reshapeFunc);
    glutIdleFunc(idleFunc);
    glutMouseFunc(mouseClickFunc);
    glutMotionFunc(mouseMotionFunc);
    glutPassiveMotionFunc(mousePassiveMotionFunc);
    glutKeyboardFunc(keyboardFunc);
    glutKeyboardUpFunc(keyboardUpFunc);
    glutSpecialFunc(specialKeyFunc);
    glutSpecialUpFunc(specialKeyUpFunc);
    glutIgnoreKeyRepeat(1);

    // should be mouse enter/leave events,
    // but implemented in freeglut with get/loose focus
    glutEntryFunc(focusFunc);

    assert(glGetError() == 0);
    glewExperimental = GL_TRUE;
    glGetError();
    glewInit();
    glGetError();

#ifdef USEFREEGLUT
    if (params.debug()) {
        assert(glDebugMessageCallbackARB != NULL);
        glDebugMessageCallbackARB((GLDEBUGPROCARB)debugCallback, NULL);
    }
#endif
    // Lars F addtion 16.05.2016
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

GlutWindow::~GlutWindow()
{
    // Lars F addition 16.05.2016
    glDeleteVertexArrays(1, &vao);

#ifdef USEFREEGLUT
    // Commented out due to error message from glut
    // (complains glutInit has notbeen called...
    glutDestroyWindow(windowId);
    glutLeaveMainLoop();
#endif
    INSTANCES.erase(windowId);
}

void GlutWindow::shutDown()
{
	//glutDestroyWindow(windowId);
	glutLeaveMainLoop();

}

int GlutWindow::getWidth() const
{
    return size.x;
}

int GlutWindow::getHeight() const
{
    return size.y;
}

void GlutWindow::start()
{
    glutMainLoop();
}

void GlutWindow::redisplay(double t, double dt)
{
    glutSwapBuffers();
    double newT = timer.end();
    this->dt = newT - this->t;
    this->t = newT;
}

void GlutWindow::reshape(int x, int y)
{
    size = vec2i(x, y);
}

void GlutWindow::idle(bool damaged)
{
    glutPostRedisplay();
}

void GlutWindow::redisplayFunc()
{
    GlutWindow* window = INSTANCES[glutGetWindow()];
    window->redisplay(window->t, window->dt);
}

void GlutWindow::reshapeFunc(int x, int y)
{
    INSTANCES[glutGetWindow()]->reshape(x, y);
}

void GlutWindow::idleFunc()
{
    GlutWindow *window = INSTANCES[glutGetWindow()];
    // glutLayerGet(GLUT_NORMAL_DAMAGED) is not implemented in freeglut
    //window->idle(glutLayerGet(GLUT_NORMAL_DAMAGED) == 1);
    window->idle(window->damaged);
    window->damaged = false;
}

void GlutWindow::mouseClickFunc(int b, int s, int x, int y)
{
    int m = glutGetModifiers();

#ifdef GLUT_WHEEL_UP
    if (b == GLUT_WHEEL_UP) {
#else
    if (b == GLUT_WHEEL_UP_BUTTON) {
        if (s == 0)
#endif
        INSTANCES[glutGetWindow()]->mouseWheel(WHEEL_UP, (modifier) m, x, y);
        return;
    }

#ifdef GLUT_WHEEL_DOWN
    if (b == GLUT_WHEEL_DOWN) {
#else
    if (b == GLUT_WHEEL_DOWN_BUTTON) {
        if (s == 0)
#endif
        INSTANCES[glutGetWindow()]->mouseWheel(WHEEL_DOWN, (modifier) m, x, y);
        return;
    }

    INSTANCES[glutGetWindow()]->mouseClick((button) b, (state) s, (modifier) m, x, y);
}

void GlutWindow::mouseMotionFunc(int x, int y)
{
    INSTANCES[glutGetWindow()]->mouseMotion(x, y);
}

void GlutWindow::mousePassiveMotionFunc(int x, int y)
{
    INSTANCES[glutGetWindow()]->mousePassiveMotion(x, y);
}

void GlutWindow::keyboardFunc(unsigned char c, int x, int y)
{
    int m = glutGetModifiers();
    INSTANCES[glutGetWindow()]->keyTyped(c, (modifier) m, x, y);
}

void GlutWindow::keyboardUpFunc(unsigned char c, int x, int y)
{
    int m = glutGetModifiers();
    INSTANCES[glutGetWindow()]->keyReleased(c, (modifier) m, x, y);
}

void GlutWindow::specialKeyFunc(int k, int x, int y)
{
    int m = glutGetModifiers();
    INSTANCES[glutGetWindow()]->specialKey((key) k, (modifier) m, x, y);
}

void GlutWindow::specialKeyUpFunc(int k, int x, int y)
{
    int m = glutGetModifiers();
    INSTANCES[glutGetWindow()]->specialKeyReleased((key) k, (modifier) m, x, y);
}

void GlutWindow::focusFunc(int focus)
{
    INSTANCES[glutGetWindow()]->damaged = (focus != 0);
}

}
