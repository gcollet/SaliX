#include <iostream>
#include <fstream>

int main(int argc, const char * argv[])
{
	const clock_t begin_time = clock();
	
	std::string app_name = argv[0];
	app_name = app_name.substr(app_name.rfind("/") + 1);
	if (argc != 3) {
		std::cerr << "Usage: " << app_name << " <pdb_file1> <pdb_file2>\n";
		return 1;
	}
	std::cerr << app_name << " executed in " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " seconds \n";
	return 0;
}