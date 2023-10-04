#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H
#include<string>
#include<cstdlib>
#include<unordered_map>
#include<regex>
#define STR_END(x) (*(--x.end()))

static std::unordered_map<std::string, std::string> parse_arguments(int argc, char** argv)
{
	std::unordered_map<std::string, std::string> args;
	const std::string arguments = "-s-w";

	std::string value,argument;
	bool get_val = false;
	bool start_read_multiline = false;
	for (int i = 1; i < argc; i++)
	{
		auto current_val = std::string(argv[i]);
		auto unknown_arg =	arguments.find(current_val) == std::string::npos;

		if (get_val)
		{
			if (current_val[0] == '"' and !start_read_multiline)
			{
				value += current_val.substr(1) + " ";
				start_read_multiline = true;
			}
			else if (current_val[0] != '"' and !start_read_multiline)
			{
				args[argument] = current_val;
				get_val = false;
			}
			else if (STR_END(current_val) == '"' and start_read_multiline)
			{
				value += current_val.substr(0, current_val.size() - 1);
				args[argument] = value;

				start_read_multiline = false;
				get_val = false;
			}
			else
			{
				printf("unexpected \" in:%s\n", current_val.c_str());
			}

		}
		else if (!unknown_arg)
		{
			if (auto n = args.count(current_val) != 0)
			{
				printf("argument %s is passed already\n", current_val.c_str());
				exit(0);
			}
			get_val = true;
			argument = current_val;
		}

		else if(unknown_arg and !get_val)
		{
			printf("unknown argument: %s\n", current_val.c_str());
			exit(-1);
		}
	}

	return args;
}
static bool is_str_number(const std::string& str)
{
	for (auto& c : str)
		if (!isdigit(c))
			return false;

	return true;
}
#endif