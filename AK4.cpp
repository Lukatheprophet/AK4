#include <iostream>
#include <fstream>
#include <filesystem>
#include <windows.h>
using namespace std;
using namespace filesystem;
void invalid() {
	cout << "Invalid arguments. Use '-h' or '--help' for usage instructions." << endl;
}

void help() {
	cout << "Usage: [file_pattern] <directory path>, replacing_files" << endl;
	cout << "Options:" << endl;
	cout << "directory_path    Path to the target directory" << endl;
	cout << "file_pattern      Pattern for replacing files (optional)" << endl;
	cout << "replacing_files   Path to files that will replaced";
}
void replace(string extension, path file, path dest) {

	if (file.extension().string() == extension) {
		try {
			rename(file, dest);
		}
		catch (filesystem_error& e) {
			cout << e.what() << endl;
			cout << "Check file's security settings" << endl;
		}
	}
}
void replace(path file, path dest) {
	try {
		rename(file, dest);
	}
	catch (filesystem_error& e) {
		cout << e.what() << endl;
		cout << "Check file's security settings" << endl;
	}
}
int main(int argc, char* input[])
{
	setlocale(LC_ALL, "Rus");
	string extension = "";
	if (argc == 2) {
		if (string(input[1]) == "--help" or string(input[1]) == "-h") {
			help();
			return 0;
		}
	}
	if (input[1] == "--help" or input[1] == "-h") {
		help();
		return 0;
	}
	if (argc == 3 and input[1][0] == '.') {
		invalid();
		cout << "Maybe you forgot input path to files" << endl;
		return 1;
	}
	int a = 1; ///текущий аргумент
	if (input[1][0] == '.') {
		extension = input[a];
		a++;
	}
	const path directory = input[a];
	directory_entry is_directory(directory);
	if (!is_directory.is_directory()) {
		invalid();
		cout << "Maybe uncorrect path to directory"<<endl;
		return 1;
	}
	for (int i = a+1; i < argc; i++)
	{
		path file = string(input[i]);
		directory_entry is_file(file);
		if (!is_file.is_regular_file())
		{
				invalid();
				cout << "Maybe uncorrect path to file" << endl;
				return 1;
		}
		if (extension == "") {
			replace(file, directory);

			LPCWSTR fp = file.c_str();
			DWORD attributes = GetFileAttributes(fp);
			if (attributes != INVALID_FILE_ATTRIBUTES) {
				if (attributes & FILE_ATTRIBUTE_READONLY) cout << i + 1 << "file is read only" << endl;
				if (attributes & FILE_ATTRIBUTE_HIDDEN) cout << i + 1 << "file is hidden" << endl;
				if (attributes & FILE_ATTRIBUTE_ARCHIVE) cout << i + 1 << "file is archive" << endl;
			}
		} 
		else {		/// расширение файла задано
			replace(extension, file, directory);

			LPCWSTR fp = file.c_str();
			DWORD attributes = GetFileAttributes(fp);
			if (attributes != INVALID_FILE_ATTRIBUTES) {
				if (attributes & FILE_ATTRIBUTE_READONLY) cout << i + 1 << "file is read only" << endl;
				if (attributes & FILE_ATTRIBUTE_HIDDEN) cout << i + 1 << "file is hidden" << endl;
				if (attributes & FILE_ATTRIBUTE_ARCHIVE) cout << i + 1 << "file is archive" << endl;
			}
		}
	}
	cout << "Files replaced succesfuly";
	return 0;
}
