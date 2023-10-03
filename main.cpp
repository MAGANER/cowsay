#include"PipeReader.h"
#include"cxxopts.hpp"
#include<vector>
#include<string>
#include <sstream>    //getline
#include<fstream>     //ifstream
#include<filesystem>  //is_regular_file
#include<algorithm>   //max_element, remove

#include<functional> //function
#define TO_PRED(x) std::function<bool(char)>(x)

#ifdef __linux__
#include <unistd.h>
#endif


bool isnewline(char ch) 
{
	switch (ch) {
	case '\r':
	case '\n':
	case '\f':
		return true;
	}
	return false;
}
std::vector<std::string> split_string(const std::string& str,
									  const std::function<bool(char)>& pred)
{
	std::vector<std::string> strings;

	std::string buffer;
	for (auto& ch : str)
	{
		if (pred(ch))
		{
			strings.push_back("|"+buffer);
			buffer.clear();
		}
		else
			buffer += ch;
	}
	return strings;
}

void read_argv(const std::string& str, size_t len, std::vector<std::string>& buffer)
{
	//split argv  into lines with special size
	auto words = split_string(str+" ", TO_PRED(isspace));
	std::string line;

	for (auto& w : words)
	{
		if (w.size() + line.size() >= len)
		{
			buffer.push_back("|"+line);
			line.clear();
		}
		line += w.substr(1)+" ";//erase | at the beginning
	}
	buffer.push_back("|"+line);
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
	auto max = (*std::max_element(lines.begin(), lines.end(),
		[](auto& a, auto& b)
		{
			return a.size() < b.size();
		}
	)).size(); //get size of the highest element 

	printf("%s\n", std::string(max + 2, '-').c_str()); //print high border
	for (auto& l : lines)
	{
		std::string s(max , ' '); s += "|";
		for (size_t i = 0; i < l.size(); i++)
			s[i] = l[i];
		printf("%s\n", s.c_str());    //print message
	}
	printf("%s\n", std::string(max + 2, '=').c_str()); //print low border
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

	std::vector<std::string> buffer;
#ifdef __linux__
  if(!isatty(0))
#endif
#ifdef _WIN32
  if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0)
#endif
  {
	char* src = nullptr;
	size_t   len;
	src = readall(stdin, &len);
	if (src)
	{
		buffer = split_string(src,TO_PRED(isnewline));

		//get rid of tabs, because they will break cloud
		for(auto& l: buffer)
			l.erase(std::remove(l.begin(), l.end(), '\t'), l.end());

		free(src);
	}
  }

  if (argc == 1 and buffer.empty())
  {
	std::vector<std::string> l = {"|moo?"};
	print_message(l);
	print_cow();
  }
  else
  {
	cxxopts::Options options("cowsay", "cow to show textual data on terminal screen");
	options.add_options()("s", "print text", cxxopts::value<std::string>());

	auto result = options.parse(argc, argv);
	if (result.arguments_string().find("-s"))
	{
		auto text = result["s"].as<std::string>();
		read_argv(text,20, buffer);
	}

	print_message(buffer);
	print_cow();
   }
		


	
	return 0;
}
