#include<iostream>
#include<cstring>
#include<list>
#include <sstream>
#include<fstream>

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
	std::list<std::string> lines;
	std::ifstream input(file);
	if (input)
	{
		for (std::string line; getline(input, line); )
		{
			lines.push_back(line);
		}
	}

	return lines;
}
void print_cow()
{
	std::cout << std::endl;
	std::cout << "          \\"<<std::endl
			  << "           \\" << std::endl
			  << "               ^__^" << std::endl
			  << "               (oo)\_______" << std::endl
			  << "               (__)\\       )\\/\\" << std::endl
			  << "                   ||----w |" << std::endl
			  << "                   ||     ||" << std::endl;
}

void print_message(size_t len,const std::list<std::string>& lines)
{
	for (size_t i = 0; i < len + 2; i++)std::cout << "-";
	std::cout << std::endl;

	for (auto& l : lines)
	{
		std::cout << "| ";
		std::cout << l;
		auto diff = abs((int)len - (int)l.size());
		for (auto i = 0; i < diff; i++)std::cout << " ";
		std::cout << "|";
		std::cout << std::endl;
	}
	for (size_t i = 0; i < len + 2; i++)std::cout << "=";

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
	setlocale(LC_ALL, "");

	//read from cli arguments or from file
	auto spec = argc  > 1?std::string(argv[1]):"";
	if (spec == "-t")
	{
		auto len = compute_horizontal_border_length(argc, argv);
		auto lines = compute_lines(argc, argv, len);
		print_message(len, lines);
	}
	else if (spec == "-f" and argc == 3)
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

	return 0;
}