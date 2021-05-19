#ifndef AXES_3D_H
#define AXES_3D_H

/*==============================================================================================#
|                                                                                               |
| All code has been written by Karsten Vermeulen as part of the 'Handmade' game engine, for the |
| purposes of educating other fellow programmers, programming students and anyone else wishing  |
| to learn about game development, C++ and OOP. The engine, class design and overall structure  |
| is by no means perfect and there is certainly room for improvement. Feel free to use, copy,   |
| break, update and do as you wish with this code - it is there, free, for all!                 |
|																							    |
| Designed to teach. Made from scratch. Built by hand.							                |
|																							    |
#===============================================================================================#
|																								|
| If you like 'Handmade', and wish to show your support, if you have any questions about the    |
| project, or if you just want to reach out, please find me on the following channels:          |
|																						        |
| Web: http://www.karstenvermeulen.com														    |
| Facebook: https://www.facebook.com/KarstensCorner								                |
| Twitter: https://twitter.com/KarstensCorner													|
| LinkedIn: https://www.linkedin.com/in/karstenvermeulen                                        |
| GitHub: https://github.com/djkarstenv									                        |
|                                                                                               |
#===============================================================================================#
| 'Axes3D' source files last updated in February 2020								            |
#==============================================================================================*/

#include "glad.h"
#include "Buffer.h"
#include "GameObject.h"

class Axes3D : public GameObject
{

public:

	Axes3D(GLint size = 15, GLfloat lineWidth = 5.0f);

public:

	void SetSize(GLint size);
	void SetLineWidth(GLfloat lineWidth);

public:

	virtual bool Create();
	virtual void Update(int deltaTime) {}
	virtual void Draw();
	virtual void Destroy();

private:

	GLint m_size;
	Buffer m_buffer;
	GLfloat m_lineWidth;

};

#endif