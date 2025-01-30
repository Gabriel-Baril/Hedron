#pragma once
#include "core/core.h"
#include "core/stl/map.h"

namespace hdn
{
	enum class MemberAttributeFlag : u32
	{
		UniquePointer = BIT(0),
		SharedPointer = BIT(1),
		Array = BIT(2)
	};
	ENABLE_ENUM_CLASS_BITWISE_OPERATIONS(MemberAttributeFlag)

	class IMemberAttribute
	{
	public:
		virtual MemberAttributeFlag GetCompatibleAttributes() const = 0;
		virtual MemberAttributeFlag GetAttributeType() const = 0;
		virtual void Parse(const map<string /*arg name*/, string /*arg value*/>& attributeArguments)
		{
			// Default implementation for parameterless attribute
		}
	};

	class UniquePointerAttribute : public IMemberAttribute
	{
	public:
		virtual MemberAttributeFlag GetAttributeType() const override
		{
			return MemberAttributeFlag::UniquePointer;
		}

		virtual MemberAttributeFlag GetCompatibleAttributes() const override
		{
			return MemberAttributeFlag::Array;
		}
	};

	class SharedPointerAttribute : public IMemberAttribute
	{
	public:
		virtual MemberAttributeFlag GetAttributeType() const override
		{
			return MemberAttributeFlag::SharedPointer;
		}

		virtual MemberAttributeFlag GetCompatibleAttributes() const override
		{
			return MemberAttributeFlag::Array;
		}
	};

	class ArrayAttribute : public IMemberAttribute
	{
	public:
		static constexpr const char* EXPR_PARAMETER_NAME = "args";

		virtual MemberAttributeFlag GetAttributeType() const override
		{
			return MemberAttributeFlag::Array;
		}

		virtual MemberAttributeFlag GetCompatibleAttributes() const override
		{
			return MemberAttributeFlag::UniquePointer | MemberAttributeFlag::SharedPointer;
		}

		virtual void Parse(const map<string /*arg name*/, string /*arg value*/>& attributeArguments)
		{
			for (const auto& [key, value] : attributeArguments)
			{
				if (key == EXPR_PARAMETER_NAME)
				{
					countExpression = value;
				}
				else
				{
					HERR("Unknown parameter '{0}={1}' while parsing 'array' attribute", key.c_str(), value.c_str());
				}
			}
		}
	private:
		string countExpression; // For example, archive(array, Expr=2), archive(array, Expr=GetDefaultCount(vLightConfig->arrLength))
	};
}