#include "ResourseManager.h"

int main()
{
	ResourseManager *rm = new ResourseManager();
	int counter = 0;

	Resourse* asteroid = rm->GetResourse("smallasteroid");
	counter = asteroid->GetRc();
	rm->ReleaseResourse("smallasteroid");
	counter = asteroid->GetRc();

	counter = 0;	
	asteroid = rm->GetResourse("smallasteroid");
	asteroid = rm->GetResourse("smallasteroid");
	asteroid = rm->GetResourse("smallasteroid");
	counter = asteroid->GetRc();
	rm->ReleaseResourse("smallasteroid");
	rm->ReleaseResourse("smallasteroid");
	rm->ReleaseResourse("smallasteroid");
	rm->ReleaseResourse("smallasteroid");
	counter = asteroid->GetRc();


    return 0;
}

