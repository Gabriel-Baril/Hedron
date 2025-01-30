#define archive(...)

// This define will be used by the archive parser
// Using a define make us able to put member names directly in the archive expression while still being able to catch name renaming of members
#define member(memberName) #memberName
#define member(typeName, memberName) "var" ## #typeName ## "->" ## #memberName