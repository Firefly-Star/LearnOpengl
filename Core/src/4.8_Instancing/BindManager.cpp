#pragma once

#include "BindManager.h"

namespace Firefly
{
	std::unique_ptr<BindManager> BindManager::s_Instance = nullptr;
	std::once_flag BindManager::s_OnceFlag;

	BindManager::BindManager()
	{
	}
	BindManager::~BindManager()
	{
	}
}