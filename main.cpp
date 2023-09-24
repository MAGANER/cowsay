#include<iostream>
#include<cstring>
#include<list>


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

	if (argc == 1)
	{
		lines.push_back("moo?");
		return lines;
	}
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
	std::cout << std::endl;
	std::cout << "          \\"<<std::endl
			  << "           \\" << std::endl
			  << "               ^__^" << std::endl
			  << "               (oo)\_______" << std::endl
			  << "               (__)\\       )\\/\\" << std::endl
			  << "                   ||----w |" << std::endl
			  << "                   ||     ||" << std::endl;
}
int main(int argc, char** argv)
{
	auto len = compute_horizontal_border_length(argc, argv);

	auto lines = compute_lines(argc, argv, len);

	for (size_t i = 0; i < len+2;i++)std::cout << "-";
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
	for (size_t i = 0; i < len+2;i++)std::cout << "=";

	print_cow();
	return 0;
}