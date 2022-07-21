#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <stack>
#include <regex>
#include "../Log.h"
#include <map>
#include "../Macros.h"
enum class TokenType
{
	UNDEFINIED,
	SCOPE,
	ATTRIBUTE,
	FLOAT4,
	FLOAT3,
	FLOAT2,
	FLOAT,
	INT4,
	INT3,
	INT2,
	INT,
	BOOL,
	TEXTURE2D,
	COLOR,
	ENTRYPOINT
};


class Token
{
public:
	Token(enum TokenType t, std::string c,std::string s = "") : type(t), m_content(c),supplement(s) {};
	enum TokenType type;
	std::string m_content;
	std::string supplement;
};



class Tokenizer
{
public:
	Tokenizer() = default;
	std::vector<Token> Parse(const std::filesystem::path& file);
private:
	int m_LineOffset = 0;
	int m_Offset = 0 ;
	std::stack<char> m_stack;
	//Token m_PrevToken;
	std::map < std::string, enum TokenType> keyMap = { {"float4",TokenType::FLOAT4},{"float3",TokenType::FLOAT3},{"float2",TokenType::FLOAT2}
	,{"float",TokenType::FLOAT4},{"bool",TokenType::BOOL},{"Texture2D",TokenType::TEXTURE2D},{"int4",TokenType::INT4},
		{"int3",TokenType::INT3},{"int2",TokenType::INT2},{"int",TokenType::INT},{"main",TokenType::ENTRYPOINT},{"Color",TokenType::COLOR}};
};