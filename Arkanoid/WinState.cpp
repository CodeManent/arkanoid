#include "winState.h"

#include <list>

#include <cstdlib>
#include <windows.h>
#include <gl/glut.h>

typedef void ( * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void ( * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void ( * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
typedef void ( * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);

PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;

namespace Arkanoid
{
	const int WinState::maxBranches = 5;
	const int WinState::maxDepth = 5;

	WinState::WinState()
	{
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");

		GLuint buffer;
		glGenBuffersARB(1, &buffer);
		root.position = point2f(0.5f, 0.8f);
		root.angle = 90.0f;
	}

	WinState::~WinState()
	{
	}

	bool WinState::step()
	{

		addTreeNode();
		return true;
	}

	void WinState::draw()
	{
		glPushMatrix();
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT);
		std::list<TreeNode*> nodeStack;

		nodeStack.push_back(&root);

		//draw tree
		glColor3f(1.0f, 1.0f, 0.0f);
		glBegin(GL_LINES);
		float depth = 1.0f;

		while(!nodeStack.empty())
		{
			float color = 1.0f/depth;
//			glColor3f(1.0f-color, color, fabs(color-0.5f));
			glColor3f(0.8f-color*0.8f, 0.2f, 0.8f*color+0.2f);

			TreeNode &current = *nodeStack.front();
			nodeStack.pop_front();

			for(int i = 0; i < current.following.size(); ++i)
			{
				TreeNode &child = current.following[i];
				glVertex2f(current.position.x, current.position.y);
				glVertex2f(child.position.x, child.position.y);

				nodeStack.push_back(&child);
			}
			depth++;
		}
		glEnd();

		glPopMatrix();
	}

	void WinState::input(const Input &input)
	{
		switch(input.type)
		{
		case Input::Start:
		case Input::Back:
		case Input::A:
		case Input::B:
		case Input::X:
		case Input::Y:
			if(input.value.buttonValue == Input::Value::ButtonDown)
				finished = true;

			break;
		}
	}

	void WinState::addTreeNode()
	{
		//find first not full node
		int depth = 1;
		TreeNode *addTo = getLeaf(root, depth);
		if(addTo == NULL)
			return;

		TreeNode child;

		float length = 1.0f/float(depth);
		length /=6.0f;
		length += 0.03f;

		float angle = addTo->angle +(-90 + addTo->following.size()*(180.0f/(maxBranches-1)) )/(depth*1.2f);
		child.angle = angle;

		float theta = 3.14f*angle/180.0f;

		point2f direction(cos(theta), -sin(theta));
		//direction is normalized
		direction *= length;

		child.position = addTo->position + direction;

		addTo->following.push_back(child);
	}

	WinState::TreeNode* WinState::getLeaf(Arkanoid::WinState::TreeNode &current, int &depth)
	{
		if(depth == maxDepth)
			return NULL;

		TreeNode *retVal = NULL;


		//check branches
		for(int i = 0; i < current.following.size(); ++i)
		{
			++depth;
			retVal = getLeaf(current.following[i], depth);
			if(retVal != NULL)
				return retVal;
			--depth;
		}

		//all branches full, current node not full
		//return current node
		if(current.following.size() == maxBranches)
			return NULL;
		else
			return &current;
	}
}
