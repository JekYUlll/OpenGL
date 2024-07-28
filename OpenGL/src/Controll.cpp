#include "Controll.h"
	
namespace ctrl
{
	bool enableMouse = false;
	bool isWireframe = false;
	static bool canShowCursor = true; // �Ƿ���ʾ���
	static bool keys[1024] = { false }; // ��¼�����Ƿ���
	static float lastX = (float)(1920 / 2), lastY = (float)(1080 / 2); // ����ʼλ��
	static bool firstMouse = true; // �Ƿ��ǵ�һ�ΰ������

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) // from LearnOpenGL �̳��к�������key_callback
	{
		if (action == GLFW_PRESS) {
			keys[key] = true;
			if (key == GLFW_KEY_ESCAPE) {
				// ����ESC��,����window���ڵ�WindowShouldClose����Ϊtrue
				std::cout << "[ESC pressed] window closed" << std::endl;
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
			if (key == GLFW_KEY_F11) {
				if (canShowCursor) {
					canShowCursor = false;
					GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
					std::cout << "[F11 pressed] hide cursor" << std::endl;
				}
				else {
					canShowCursor = true;
					GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL));
					std::cout << "[F11 pressed] show cursor" << std::endl;
				}
			}
			if (key == GLFW_KEY_F1) {
				if (!enableMouse) {
					enableMouse = true;
					std::cout << "[F1 pressed] enable mouse" << std::endl;
				}
				else {
					enableMouse = false;
					std::cout << "[F1 pressed] disable mouse" << std::endl;
				}
			}
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (enableMouse) {
			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = (xpos - lastX) * sensitivity;
			float yoffset = (lastY - ypos) * sensitivity; // �෴�ģ���Ϊy����ķ�Χ�Ǵ������ϵ�
			lastX = xpos;
			lastY = ypos;

			yaw += xoffset;
			pitch += yoffset;
			pitch = glm::clamp(pitch, -89.0f, 89.0f);

			glm::vec3 front{};
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraFront = glm::normalize(front);
		}
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		fov_y -= yoffset * 0.20f;
		fov_y = glm::clamp(fov_y, 1.0f, 45.0f);
		// �����޷Ŵ�ԭ��δ֪
	}

	void MoveCamera()
	{
		// �ƶ����ǰ��
		if (keys[GLFW_KEY_W])
			cameraPos += cameraMove * cameraFront;
		if (keys[GLFW_KEY_S])
			cameraPos -= cameraMove * cameraFront;
		// �ƶ��������
		if (keys[GLFW_KEY_A])
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraMove;
		if (keys[GLFW_KEY_D])
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraMove;
		// �������½�
		if (keys[GLFW_KEY_SPACE])
			cameraPos += cameraMove * cameraUp;
		if (keys[GLFW_KEY_C])
			cameraPos -= cameraMove * cameraUp;
		if (keys[GLFW_KEY_LEFT_SHIFT]) 
			cameraSpeed = cameraSpeed_rush;
		else
			cameraSpeed = cameraSpeed_std;
	}
	// �л�Ϊ�߿�/���ģʽ
	void SetFrameMode()
	{
		if (isWireframe) {
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));  // �л����߿�ģʽ
			std::cout << "[LINE mode]" << std::endl;
		}
		else {
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));  // �л������ģʽ
			std::cout << "[FILL mode]" << std::endl;
		}
	}
}