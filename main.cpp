#include"PipeReader.h"
#include<cstring>     //strlen
#include<list>        //list itself
#include <sstream>    //getline
#include<fstream>     //ifstream
#include<filesystem>  //is_regular_file

#ifdef __linux__
#include<unistd.h>
#endif

bool isnewline(int ch) 
{
	switch (ch) {
	case '\r':
	case '\n':
	case '\f':
		return true;
	}
	return false;
}
std::list<std::string> split_string(const std::string& str)
{
	std::list<std::string> strings;

	std::string buffer;
	for (auto& ch : str)
	{
		if (isnewline(ch))
		{
			strings.push_back(buffer);
			buffer.clear();
		}
		else
			buffer += ch;
	}
	return strings;
}
std::list<std::string> compute_lines(int argc, char** argv, size_t len)
{
	//split argv arguments into lines with special size
	std::list<std::string> lines;

	std::string line;
	for (size_t i = 1; i < argc; i++)
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
void print_cow()
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
void print_message(std::list<std::string>& lines)
{
	auto max_size = (*std::max_element(lines.begin(), lines.end(),
		[](auto& a, auto& b)
		{
			return a.size() < b.size();
		}
	)).size();
	auto pad = [&](auto& s)
	{
		s = "| " +s;
		while (s.size() != max_size + 3)
			s += " ";
		s += "|";
	};

	printf("%s\n", std::string(max_size + 3, '-').c_str());
	for (auto& line : lines)
	{
		pad(line);
		printf("%s\n", line.c_str());
	}
	printf("%s\n", std::string(max_size + 3, '=').c_str());
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
		auto lines = split_string(src);
		print_message(lines);
		print_cow();
		free(src);
	}
#ifdef __linux__	
  }
#endif
	else
	{
		if (argc == 1)
		{
			printf("Usage example #1: cat file | cowsay\n \
				   Usage example #2: cowsay bla bla bla bla");
			return 0;
		}
		else
		{
			//every line contains up to 20 characters
			auto lines = compute_lines(argc, argv, 20);
			print_message(lines);
			print_cow();
		}
		
	}

	
	return 0;
}
