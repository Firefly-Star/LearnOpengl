#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>
#define FF_DEBUG

#ifdef FF_DEBUG
namespace Firefly
{
	inline void GetError_(const char* file, unsigned int line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM: error = "ö�ٲ������Ϸ�"; break;
			case GL_INVALID_VALUE: error = "ֵ�������Ϸ�"; break;
			case GL_INVALID_OPERATION: error = "һ��ָ���״̬��ָ��Ĳ������Ϸ�"; break;
			case GL_STACK_OVERFLOW: error = "ѹջ�������ջ����(Overflow)"; break;
			case GL_STACK_UNDERFLOW: error = "ջ����"; break;
			case GL_OUT_OF_MEMORY: error = "�ڴ���ò����޷����ã��㹻�ģ��ڴ�"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "��ȡ��д��һ����������֡����"; break;
			}
			std::cout << "GL_ERROR: " << error << ", at " << file << ", at line " << line << ".\n";
		}
	}
}

#define GET_ERROR GetError_(__FILE__, __LINE__) 
#else
#define GET_ERROR
#endif