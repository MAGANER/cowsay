#include"PipeReader.h"
#include"ArgumentParser.h"
#include<vector>
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
void print_cow(char cow_eye)
{
	//one system call is better then several
	std::string cow;
	cow += "\n          \\\n";
	cow += "           \\\n";
	cow += "               ^__^ \n";
	cow += "               (%c%c)_______ \n";
	cow += "               (__)\       )\\/\\ \n";
	cow += "                   ||----w |  \n";
	cow += "                   ||     || \n";
	
	printf(cow.c_str(),cow_eye,cow_eye);
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
int main(int argc, char** argv)
{
	//read from stdin pipe
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
	print_cow('o');
  }
  else
  {
	//parse arguments and apply them
	  auto arguments = parse_arguments(argc, argv);

	  std::string text_to_say;
	  char cow_eye = 'o';
	  int line_width = 20;
	  if (arguments.find("-s") != arguments.end())
	  {
		  text_to_say = arguments["-s"];
	  }
	  if (arguments.find("-w") != arguments.end())
	  {
		  auto width_str = arguments["-w"];
		  if (is_str_number(width_str))
		  {
			  line_width = atoi(width_str.c_str());
		  }
		  else
		  {
			  printf("%s must be digit value", width_str.c_str());
			  exit(-1);
		  }
	  }
	  if (arguments.find("-e") != arguments.end())
	  {
		  auto eye = arguments["-e"];
		  if (eye.size() != 1)
		  {
			  printf("eye must be one character or you want to get a mutant cow?\n Sounds good actually...\n");
			  srand(0);
			  exit(rand());
		  }
		  cow_eye = eye[0];
	  }

	  if (!text_to_say.empty())
	  {
		  read_argv(arguments["-s"], line_width, buffer);
	  }

	  if (buffer.empty())
	  {
		  printf("output buffer is empty\n");
		  exit(-1);
	  }
	  else
	  {
		  print_message(buffer);
		  print_cow(cow_eye);
	  }
   }
		


	
	return 0;
}
