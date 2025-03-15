#pragma once

#include "core/core.h"
#include "core/stl/unordered_map.h"

namespace hdn
{
	class Image
	{
	public:
		void Load(const char* path);
		void Unload();
		virtual ~Image();

		u32 GetWidth() { return m_Width; }
		u32 GetHeight() { return m_Height; }
		u32 GetChannelCount() { return m_Channels; }
	private:
		void* m_Pixels = nullptr;
		u32 m_Width;
		u32 m_Height;
		u32 m_Channels;
	};

	class ImageRegistry
	{
	public:
		static ImageRegistry& Get();

		bool Contains(const char* name);
		void Register(const char* name, Ref<Image> image);
		Ref<Image> Get(const char* name);
	private:
		ImageRegistry() = default;
	private:
		unordered_map<u64, Ref<Image>> m_ImageRegistry{};
	};
}