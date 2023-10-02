#include"PipeReader.h"
#include<vector>
#include <sstream>    //getline
#include<fstream>     //ifstream
#include<filesystem>  //is_regular_file
#include <iomanip>

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
std::vector<std::string> split_string(const std::string& str)
{
	std::vector<std::string> strings;

	std::string buffer;
	for (auto& ch : str)
	{
		if (isnewline(ch))
		{
			strings.push_back("|"+buffer);
			buffer.clear();
		}
		else
			buffer += ch;
	}
	return strings;
}

std::vector<std::string> compute_lines(int argc, char** argv, size_t len)
{
	//split argv arguments into lines with special size
	std::vector<std::string> lines;

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
void print_message(std::vector<std::string>& lines)
{
	auto max_size = (*std::max_element(lines.begin(), lines.end(),
		[](auto& a, auto& b)
		{
			return a.size() < b.size();
		}
	)).size();
	auto pad = [&](auto& s)
	{
		s = "| " + s;
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
size_t get_max_size(const std::vector<std::string>& lines)
{
	return (*std::max_element(lines.begin(), lines.end(), [](auto& a, auto& b) { return a.size() < b.size(); })).size();
}
void pad(std::string& line, size_t max)
{
	while (line.size() != max)
		line += "#";
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
		for(auto& l:lines)
			l.erase(std::remove(l.begin(), l.end(), '\t'), l.end());

		auto max = get_max_size(lines);

		printf("%s\n", std::string(max + 2, '-').c_str()); //print high border
		for (auto& l : lines)
		{
			std::string s(max + 1, ' '); s += "|";
			for (size_t i = 0; i < l.size(); i++)
				s[i] = l[i];
			printf("%s\n", s.c_str());    //print message
		}
		printf("%s\n", std::string(max + 2, '=').c_str()); //print low border
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
			printf("Usage example #1: cat file | cowsay\nUsage example #2: cowsay bla bla bla bla");
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
