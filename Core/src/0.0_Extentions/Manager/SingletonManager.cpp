#include "SingletonManager.h"

#include "0.0_Extentions/IOSystem/EventSystem/EventManager.h"
#include "0.0_Extentions/Updater/MousePos.h"
#include "0.0_Extentions/Updater/MouseMode.h"
#include "0.0_Extentions/Updater/Time.h"
#include "4.3_Framebuffer/ShaderLibrary.h"
#include "4.3_Framebuffer/SpriteRenderer.h"
#include "4.4_Cube_Texture/SkyboxRenderer.h"
#include "4.8_Instancing/BindManager.h"

namespace Firefly
{
	void SingletonManager::Init()
	{
		BindManager::Init();
		EventManager::Init();

		ShaderLibrary::Init();
		SpriteRenderer::Init();
		SkyboxRenderer::Init();
		
		Time::Init();
		MousePos::Init();
		MouseMode::Init();
	}

	void SingletonManager::Terminate()
	{
		MouseMode::Terminate();
		MousePos::Terminate();
		Time::Terminate();

		SkyboxRenderer::Terminate();
		SpriteRenderer::Terminate();
		ShaderLibrary::Terminate();
		
		EventManager::Terminate();
		BindManager::Terminate();
	}
}