#include"PipeReader.h"
#include<vector>
#include<string>
#include <sstream>    //getline
#include<fstream>     //ifstream
#include<filesystem>  //is_regular_file
#include<algorithm>   //max_element, remove

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

void read_argv(int argc, char** argv, size_t len, std::vector<std::string>& buffer)
{
	//split argv arguments into lines with special size

	std::string line;
	for (size_t i = 1; i < argc; i++)
	{
		if (line.size() + strlen(argv[i]) >= len)
		{
			buffer.push_back("|"+line);
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
  if((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0)
  {
	char* src = nullptr;
	size_t   len;
	src = readall(stdin, &len);
	if (src)
	{
		buffer = split_string(src);

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
	//every line contains up to 20 characters
	read_argv(argc, argv, 20,buffer);
	print_message(buffer);
	print_cow();
   }
		


	
	return 0;
}
