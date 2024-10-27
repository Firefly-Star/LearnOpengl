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
			case GL_INVALID_ENUM: error = "枚举参数不合法"; break;
			case GL_INVALID_VALUE: error = "值参数不合法"; break;
			case GL_INVALID_OPERATION: error = "一个指令的状态对指令的参数不合法"; break;
			case GL_STACK_OVERFLOW: error = "压栈操作造成栈上溢(Overflow)"; break;
			case GL_STACK_UNDERFLOW: error = "栈下溢"; break;
			case GL_OUT_OF_MEMORY: error = "内存调用操作无法调用（足够的）内存"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "读取或写入一个不完整的帧缓冲"; break;
			}
			std::cout << "GL_ERROR: " << error << ", at " << file << ", at line " << line << ".\n";
		}
	}
}

#define GET_ERROR GetError_(__FILE__, __LINE__) 
#else
#define GET_ERROR
#endif