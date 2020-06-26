#pragma comment(lib, "shlwapi.lib")
#include <string>
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <vector>
#include <Shlwapi.h>
#include <fstream>

class SubsFixer
{
private:
	std::vector<std::string> SubsList, VidList;
	std::string subExtension, videoExtension;
public:
	void Init()
	{
		// Set the console title
		SetConsoleTitle("subfixer by Make");

		// Getting the files extensions
		std::cout << "SubFixer v1.0";
		std::cout << "Enter the subs extension:"; std::cin >> subExtension;
		std::cout << "Enter the vids extension:"; std::cin >> videoExtension;
	}

	void GetSubs(char pathFolder[]) noexcept
	{
		std::cout << "Gettings sub / video files . . ." << std::endl;

		// Removing the filename from the argv (getting the directory)
		if (::GetModuleFileName(NULL, pathFolder, MAX_PATH))
			::PathRemoveFileSpec(pathFolder);

		// Loop through all files in that directory
		for (const auto& entry : std::filesystem::directory_iterator(pathFolder))
		{
			// Save the files with the SubExtension
			if (strcmp(entry.path().extension().string().c_str(), subExtension.c_str()) == 0)
			{
				// Cout the file directory + save the path in a vector
				std::cout << entry.path() << std::endl;
				SubsList.push_back(entry.path().string().c_str());
			}

			// Save the files with the VidExtension
			if (strcmp(entry.path().extension().string().c_str(), videoExtension.c_str()) == 0)
			{
				// Cout the file directory + save the path in a vector
				std::cout << entry.path() << std::endl;
				VidList.push_back(entry.path().string().c_str());
			}
		}
		std::cout << "Videos found: " << VidList.size() << std::endl;
		std::cout << "Subs found: " << SubsList.size() << std::endl;
	}

	void EditSubs() noexcept
	{
		// Check if there are subs and videos
		if (SubsList.size() == 0 || VidList.size() == 0)
		{
			std::cout << "No sub / videos found . . ." << std::endl;
			return;
		}
		std::cout << "Renaming sub files . . ." << std::endl;
		int maxSize;
		// Saving the bigger vector size
		if (SubsList.size() < VidList.size())
			maxSize = SubsList.size();
		else
			maxSize = VidList.size();
		
		// Loop through all the videos / subtitles
		for (int j = 0; j < SubsList.size(); j++)
		{
			// Remove the extension from the video
			size_t lastindex = VidList[j].find_last_of(".");
			std::string rawname = VidList[j].substr(0, lastindex);

			// Add a srt extension to the video
			rawname.append(".srt");

			// Save all the text in the subtitle in a string
			std::ifstream t(SubsList[j].c_str());
			std::string subString((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());
			t.close();

			// Delete the subtitle
			_unlink(SubsList[j].c_str());

			// Create new subtitle with the video name
			std::ofstream out(rawname.c_str());
			for (int i = 0; i < subString.size(); i++)
			{
				// Write the old subtitle text into the new subtitle file
				if (int(subString[i]) >= 32 && int(subString[i]) <= 126)
					out << subString[i];
				else if (subString[i] == '\n')
					out << '\n';
				else if (int(subString[i]) == -2)
				{
					out << "\xc5\xa3"; // ţ
				}
				else if (int(subString[i]) == -70)
				{
					out << "\xc5\x9f"; // ş
				}
				else if (int(subString[i]) == -29)
				{
					out << "\xc4\x83"; // ă
				}
				else if (int(subString[i]) == -18)
				{
					out << "\xc3\xae"; // î
				}
				else if (int(subString[i]) == -30)
				{
					out << "\xc4\x83"; // ă
				}
				else if (int(subString[i]) == -61)
				{
					out << "\xc4\x82"; // Ă
				}
				else if (int(subString[i]) == -50)
				{
					out << "\xc3\x8e"; // Î
				}
				else if (int(subString[i]) == -86)
				{
					out << "\xc5\x9e"; // Ş
				}
			}
			out.close();
		}
		std::cout << "All the files were renamed and fixed!" << std::endl;
	}
};


int main(int argc, char* argv[])
{
	SubsFixer subs;
	subs.Init();
	subs.GetSubs(argv[0]);
	subs.EditSubs();
	system("pause");
	return 0;
}