#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include <memory>

#include <type_traits>

template <typename Enum>
constexpr auto underlying(Enum e) noexcept -> typename std::underlying_type<Enum>::type
{
	static_assert(std::is_enum<Enum>::value, "Template parameter must be an enumeration type.");
	return static_cast<typename std::underlying_type<Enum>::type>(e);
}

enum IdeationNodeSource
{
	AI,
	User
};

enum IdeationNodeStatus
{
	InProgress,
	Closed
};

enum IdeationNodeType
{
	Text,
	Size,
	Invalid
};

struct IdeationNodeID
{
	uint64_t ID;
};

struct TextIdeationPayload
{
	std::string Semantic;
	std::string Text;
};

struct IdeationPayload
{
	union {
		TextIdeationPayload* TextPayload;
	};
};

struct IdeationNode
{
	IdeationNodeID ID;

	IdeationNodeType Type;
	IdeationNodeStatus Status;
	IdeationNodeSource Source;

	uint64_t CreatedDate;
	uint64_t ModifiedDate;

	int Version;
	int Weight;
	bool Inline;

	std::vector<IdeationNodeID> Dependencies;
	std::vector<std::string> ContentTags;

	// Ideation should instead be organized by module:namespace (for example, module0:/n0/n1)
	// std::string namespace
	// std::string module

	char Author[256];
	char Name[256];

	IdeationPayload Payload;
};