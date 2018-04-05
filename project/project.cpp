#include "stdafx.h"
#include "ResourseManager.h"

int main()
{
	ResourseManager * rm = new ResourseManager();

	std::string resoursePaths = { "C:\Projects\BootCamp\project\bmpimage.bmp", "C:\Projects\BootCamp\project\bmpimage.wav" };
	Resourse *res;

	std::string::iterator iter = resoursePaths.begin();
	while (iter != resoursePaths.end())
	{
		*res = rm->getResourse(iter);

		// show

		rm->ReleaseResourse();
	}




    return 0;
}	

