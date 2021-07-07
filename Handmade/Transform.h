#pragma once

/*===================================================================#
| 'Object' source files last updated on 23 June 2021                 |
#===================================================================*/

#include "glad.h"
#include <glm.hpp>
#include <gtc/quaternion.hpp>

//TODO - This class still needs to be fully tested

class Transform
{

public:

	enum class Space
	{
		LOCAL,
		GLOBAL
	};

	Transform(const glm::mat4& matrix = glm::mat4(1.0f));

	Transform operator*(const Transform& rhs);
	Transform& operator*=(const Transform& rhs);

	glm::vec3 GetEulerAngles();
	const glm::vec3& GetPosition();
	const glm::quat& GetRotation();
	const glm::vec3& GetScale();
	const glm::mat4& GetMatrix();

	void SetIdentity();

	void SetPosition(const glm::vec3& position);
	void SetPosition(GLfloat x, GLfloat y, GLfloat z);

	void SetRotation(const glm::quat& rotation);
	void SetRotation(const glm::vec3& eulerAngles);
	void SetRotation(GLfloat angle, const glm::vec3& axis);
	void SetRotation(GLfloat pitch, GLfloat yaw, GLfloat roll);

	void SetScale(const glm::vec3& scale);
	void SetScale(GLfloat x, GLfloat y, GLfloat z);

	void Translate(const glm::vec3& translation, Space space);
	void Translate(GLfloat x, GLfloat y, GLfloat z, Space space);

	void Rotate(const glm::quat& rotation, Space space);
	void Rotate(const glm::vec3& eulerAngles, Space space);
	void Rotate(GLfloat angle, const glm::vec3& axis, Space space);
	void Rotate(GLfloat pitch, GLfloat yaw, GLfloat roll, Space space);

	void Scale(const glm::vec3& scale);
	void Scale(GLfloat x, GLfloat y, GLfloat z);

	void Update();

private:

	bool m_isDirty;

	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale;
	glm::mat4 m_matrix;

};