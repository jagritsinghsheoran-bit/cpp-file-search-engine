#include "Tokenizer.h"
#include <cctype>
#include <string>
#include <vector>



std::vector<std::string> tokenize(const std::string& text)
{
	std::vector<std::string>tokens{};

	std::string currToken;
	for (std::size_t i = 0; i < text.size(); ++i)
	{
		char ch{ text[i] };
		
		if (std::isalnum(static_cast<unsigned char>(ch)))
		{
			currToken.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
			
		}
		else if ((ch == '+' || ch == '#') && !currToken.empty())
		{
			if ((i != 0 && std::isalnum(static_cast<unsigned char>(text[i - 1])))||(i!=0&&(text[i-1]=='+'||text[i-1]=='#')))
			{
				currToken.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
			}
			else
			{
				continue;
			}
		}
		else
		{
			if (currToken.empty())
			{
				continue;
			}
			else
			{
				tokens.push_back(currToken);
				currToken.clear();

			}
			
		}

	}
	if (!currToken.empty())
	{
		tokens.push_back(currToken);
	}
	return tokens;

}
