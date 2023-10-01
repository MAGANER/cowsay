#include"PipeReader.h"
#include<cstring>     //strlen
#include<list>        //list itself
#include <sstream>    //getline
#include<fstream>     //ifstream
#include<filesystem>  //is_regular_file

#ifdef __linux__
#include<unistd.h>
#endif

std::list<std::string> split_string_by_newline(const std::string& str)
{
	auto result = std::list<std::string>{};
	auto ss = std::stringstream{ str };

	for (std::string line; std::getline(ss, line, '\n');)
		result.push_back(line);

	return result;
}
size_t compute_horizontal_border_length(int argc, char** argv)
{
	auto max = argc < 10 ? argc : 10;

	size_t s = 0;
	for (size_t i = 0; i < max; i++)
	{
		s+=strlen(argv[i]); 
	}
	return s-1;
}
std::list<std::string> compute_lines(int argc, char** argv, size_t len)
{
	//split argv arguments into lines with special size
	std::list<std::string> lines;

	std::string line;
	for (size_t i = 2; i < argc; i++)
	{
		if (line.size() + strlen(argv[i]) >= len)
		{
			lines.push_back(line);
			line.clear();
			line += argv[i];
			line += " ";
		}
		else
		{
			line += argv[i];
			line += " ";
		}
	}
	lines.push_back(line);
	return lines;
}
std::list<std::string> read_lines(const std::string& file)
{
	//read file line by line, if it's only regular file

	std::list<std::string> lines;
	if (std::filesystem::is_regular_file(file))
	{
		std::ifstream input(file);
		for (std::string line; getline(input, line); )
		{
			lines.push_back(line);
		}
	}
	return lines;
}
inline void print_cow()
{
	//one system call is better then several
	std::string cow;
	cow += "\n          \\\n";
	cow += "           \\\n";
	cow += "               ^__^ \n";
	cow += "               (oo)_______ \n";
	cow += "               (__)\       )\\/\\ \n";
	cow += "                   ||----w |  \n";
	cow += "                   ||     || \n";
	
	printf(cow.c_str());
}

void print_message(size_t len,const std::list<std::string>& lines)
{
	//print upper delimiter
	for (size_t i = 0; i < len + 2; i++)printf("-");  //std::cout << "-";
	printf("\n");

	for (auto& l : lines)
	{
		printf("| %s",l.c_str());
		auto diff = abs((int)len - (int)l.size());
		for (auto i = 0; i < diff; i++)printf(" "); //std::cout << " ";

		printf("|\n");
	}


	//print bottom delimiter
	for (size_t i = 0; i < len + 2; i++)printf("="); //std::cout << "=";

	print_cow();
}
void print_empty_message()
{
	std::list<std::string> line;
	line.push_back("moo?");
	print_message(5, line);
}
size_t get_max_size(const std::list<std::string>& lines)
{
	//return the longest line in the list
	size_t max = (*lines.begin()).size();
	for (auto& l : lines)
	{
		if (l.size() > max)
			max = l.size();
	}
	return max;
	
}

int main(int argc, char** argv)
{

	char* src = nullptr;
	size_t   len;

#ifdef __linux__
  if(!isatty(0))
  {
#endif
	src = readall(stdin, &len);
	if (src)
	{
		auto lines = split_string_by_newline(std::string(src));
		auto len = get_max_size(lines);
		print_message(len, lines);

		free(src);
	}
#ifdef __linux__	
  }
#endif
	else
	{
		//read from cli arguments or from file
		//if there is another argument, except the cowsay itself, then 
		//we can check what is that option
		auto spec = argc > 1 ? std::string(argv[1]) : "";


		if (spec == "-t")
		{
			auto len = compute_horizontal_border_length(argc, argv);
			auto lines = compute_lines(argc, argv, len);
			print_message(len, lines);
		}
		else if (spec == "-f" && argc == 3)
		{
			auto lines = read_lines(argv[2]);
			if (lines.empty())
				print_empty_message();
			else
			{
				auto len = get_max_size(lines);
				print_message(len, lines);
			}
		}
		else
		{
			print_empty_message();
		}
	}

	
	return 0;
}
