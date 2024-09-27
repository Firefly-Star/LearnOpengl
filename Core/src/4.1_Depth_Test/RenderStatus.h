#pragma once
namespace Firefly
{
    namespace RenderStatus
    {
		typedef unsigned char Status;

		const Status None			= 0;
		const Status DepthTest		= 1 << 0;
		const Status StencilTest	= 1 << 1;
		const Status Blend			= 1 << 2;
		const Status CullFace		= 1 << 3;
    }
}