#ifndef TIMER_H
#define TIMER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// ��ʱ
namespace timer
{
	extern float lastFrame;   // ��һ֡��ʱ��
	extern float currentFrame;
	extern float deltaTime;   // ��ǰ֡����һ֡��ʱ���

	void SyncTime(); // ͬ��ʱ��
	

}








#endif // !TIMER_H



