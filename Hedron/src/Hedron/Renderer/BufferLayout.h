#pragma once

#include "hdrpch.h"

namespace Hedron
{
	enum class ShaderDataType// : uint8_t
	{
		NONE = 0,
		FLOAT,
		FLOAT2, 
		FLOAT3, 
		FLOAT4,
		MAT3, 
		MAT4,
		INT, 
		INT2, 
		INT3, 
		INT4,
		BOOL
	};

	static uint32_t shader_data_type_size(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::FLOAT:  return     sizeof(float);
			case ShaderDataType::FLOAT2: return 2 * sizeof(float);
			case ShaderDataType::FLOAT3: return 3 * sizeof(float);
			case ShaderDataType::FLOAT4: return 4 * sizeof(float);
			case ShaderDataType::MAT3:   return 3 * 3 * sizeof(float);
			case ShaderDataType::MAT4:   return 4 * 4 * sizeof(float);
			case ShaderDataType::INT:    return     sizeof(int);
			case ShaderDataType::INT2:   return 2 * sizeof(int);
			case ShaderDataType::INT3:   return 3 * sizeof(int);
			case ShaderDataType::INT4:   return 4 * sizeof(int);
			case ShaderDataType::BOOL:   return     sizeof(bool);
		}

		HDR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	static uint32_t shader_data_type_component_count(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::FLOAT:  return 1;
			case ShaderDataType::FLOAT2: return 2;
			case ShaderDataType::FLOAT3: return 3;
			case ShaderDataType::FLOAT4: return 4;
			case ShaderDataType::MAT3:   return 3 * 3;
			case ShaderDataType::MAT4:   return 4 * 4;
			case ShaderDataType::INT:    return 1;
			case ShaderDataType::INT2:   return 2;
			case ShaderDataType::INT3:   return 3;
			case ShaderDataType::INT4:   return 4;
			case ShaderDataType::BOOL:   return 1;
		}

		HDR_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


	struct BufferElement
	{
		ShaderDataType type;
		std::string name;
		uint32_t offset;
		uint32_t size;
		bool normalized;

		BufferElement() 
			: type(ShaderDataType::NONE), name("UNKNOWN"), offset(0), size(0), normalized(false)
		{
		}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: name(name), type(type), size(shader_data_type_size(type)), offset(0), normalized(normalized)
		{
		}

		uint32_t get_component_count() const
		{
			return shader_data_type_component_count(type);
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout()
			: m_elements({})
		{
		}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_elements(elements)
		{
			calculate_offsets_and_stride();
		}

		inline const std::vector<BufferElement>& get_elements() const { return m_elements; };
		inline uint32_t get_stride() const { return m_stride; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	private:
		void calculate_offsets_and_stride()
		{
			uint32_t offset = 0;
			this->m_stride = 0;
			for (BufferElement& element : m_elements)
			{
				element.offset = offset;
				offset += element.size;
			}
			this->m_stride = offset;
		}

		uint32_t m_stride = 0;
		std::vector<BufferElement> m_elements;
	};
}
