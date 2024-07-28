#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern enum Camera_Movement;

extern float cameraMove;
extern float cameraSpeed; // �������
extern const float cameraSpeed_std; // ��׼����
extern const float cameraSpeed_rush; // ����ʱ����

extern glm::vec3 cameraPos; // ���λ��
extern glm::vec3 cameraTarget; // ���Ŀ��
extern glm::vec3 cameraDirection; // ��˵�ġ���������/Direction Vector����ָ��z��������ģ��������������ע�ӵ��Ǹ�����
extern glm::vec3 cameraFront;
extern glm::vec3 cameraRight;
extern glm::vec3 cameraUp;
extern glm::vec3 up;

extern float pitch;
extern float yaw;
extern float roll;

extern float fov_y; // �ӽǴ�С
extern float sensitivity; // ���������

extern const float YAW;
extern const float PITCH;
extern const float SPEED;
extern const float SENSITIVTY;
extern const float ZOOM;

class Camera
{

};

























#endif // !CAMERA_H

