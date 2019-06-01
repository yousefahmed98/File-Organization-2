#include <iostream> 
#include "SimpleIndex.h"
using namespace std;



int main() {

	SimpleIndex i;
	i.loadIndex();
	i.loadSecIndex();
	i.loadcourseidfile();
	cout << "1) Add New course \n2) Delete course(ID) \n3) Delete course(instructor name) \n4) Print course(ID) \n5) Print course(instructor name) \n6) update course(ID) \n7) update course(instructor name) \n8) Exit \n \n ";
	int ch;
	while (true) {
		cout << "\n-----------------------------------------";
		cout << "\nPlease Enter Your Choice :";
		cin >> ch;
		cin.ignore();
		if (ch == 1) {
			i.insertCourse();
		}
		else if (ch == 2) {
			char PK[6];
			cout << "enter ID :";
			cin >> PK;
			i.deleteCourse(PK);
		}
		else if (ch == 3) {
			i.deleteCoursebyIstractor();
		}
		else if (ch == 4) {
			char PK[6];
			cout << "enter ID :";
			cin >> PK;
			cin.ignore();
			i.printCoursebyPK(PK);
		}
		else if (ch == 5) {
			i.printCoursebySK();
		}
		else if (ch == 6) {
			i.updateCourse();
		}
		else if (ch == 7) {
			i.updateCoursebyinstructor();
		}
		else if (ch == 8) {
			break;
		}
	}
	i.saveIndex();
	i.saveSecIndex();
	i.saveCourseid();


}