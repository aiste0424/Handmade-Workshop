#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

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
| 'BoxCollider' source files last updated in February 2020								        |
#==============================================================================================*/

#include <glm.hpp>

//TODO - Add some kind of 'central system' to render all colliders in the scene

class SphereCollider;

class BoxCollider
{

public:

	BoxCollider();
	
public:

	const glm::vec3& GetScale()     const;
	const glm::vec3& GetPosition()  const;
	const glm::vec3& GetDimension() const;

public:

	void SetScale(float x, float y, float z);
	void SetPosition(float x, float y, float z);
	void SetDimension(float width, float height, float depth);

public:

	bool IsColliding(const BoxCollider& secondBox) const;
	bool IsColliding(const SphereCollider& secondSphere) const;
	const glm::vec3& PointOnBox(float x, float y, float z) const;

public:

	void Update();
	void Draw();

private:

	glm::vec3 m_min;
	glm::vec3 m_max;
    glm::vec4 m_color;
	glm::vec3 m_scale;
    glm::vec3 m_position;
	glm::vec3 m_dimension;
	glm::vec3 m_halfDimension;

};

#endif