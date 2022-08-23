#include "Tokenizer.h"

std::vector<Token> Tokenizer::Parse(const std::filesystem::path& file)
{
	std::vector<Token> output;

	std::ifstream filestream(file);
	std::string line;
	while (std::getline(filestream, line))
	{

			std::regex test("\\(");

			m_Offset = 0;

			if (std::regex_search(line,test)) //check if we have an header
			{
				if (m_stack.size() > 0)
				{
					//ERROR THIS SHOULD NOT HAPPEN IN THIS FILE NO NO NO !
				}
				std::string buffer = "";
				while (line[m_Offset] != '(')
				{
					buffer+= line[m_Offset];
					m_Offset++;
				}
				if (keyMap.count(buffer))
				{
					Armageddon::Log::GetLogger()->info("IN THE MAP");

					//TODO HANDLE MORE ERRORS BECAUSE OF IT 
					if (line[m_Offset] == '(')
					{
						m_stack.emplace('(');
						m_Offset++;
					}
					while (line[m_Offset] != '\"')
						m_Offset++;
					if (line[m_Offset] == '\"')
					{
						m_stack.emplace('\"');
						m_Offset++;
					}
					std::string name = "";
					
					while (line[m_Offset] != '\"')
					{
						name += line[m_Offset];
						m_Offset++;
					}
					if (line[m_Offset] == '\"')
					{
						char checker = m_stack.top();
						m_stack.pop();
						if (checker != '\"')
						{
							Armageddon::Log::GetLogger()->info("ERRRRRRRRRRRRRROR");
						}
						m_Offset++;
					}
					if (line[m_Offset] == ')')
					{
						char checker = m_stack.top();
						m_stack.pop();
						if (checker != '(')
						{
							Armageddon::Log::GetLogger()->info("ERRRRRRRRRRRRRROR");
						}
						m_Offset++;
					}
					//The stack should be empty and we get all informations
					
					Token tok (keyMap[buffer],name);
					//m_PrevToken = tok;
					output.push_back(tok);
				}
				else
				{
					//ERROR

				}
				//Parse the header
			}
			else if(std::regex_search(line,std::regex("\\{")))
			{
				Token tok(TokenType::SCOPE, "{");
				output.push_back(tok);
			}
			else if (std::regex_search(line, std::regex("\\}")))
			{
				Token tok(TokenType::SCOPE, "}");
				output.push_back(tok);
			}
			else if (std::regex_search(line, std::regex("\\="))) // check if we have opperation
			{


				//Skip all white spaces
				while (line[m_Offset] == ' ' || line[m_Offset] == '\t')
					m_Offset++;
				std::string var  =  "";
				while (line[m_Offset] != ' ' && line[m_Offset] != '=')
				{
					var += line[m_Offset];
					m_Offset++;
				}

				while (line[m_Offset] != '=')
					m_Offset++;
				while(line[m_Offset] != '<')
				{
					m_Offset++;
				}
				m_Offset++;
				std::string hlsl_var = "";
				while (line[m_Offset] != '>')
				{
					hlsl_var += line[m_Offset];
					m_Offset++;
				}
				if (line[m_Offset] == '>')
				{
					m_Offset++;
				}
				Token tok(TokenType::ATTRIBUTE,var,hlsl_var);
				output.push_back(tok);

			}
			else // this is whitespace ?
			{


			}
			m_LineOffset++;

	}
	return output;
}
