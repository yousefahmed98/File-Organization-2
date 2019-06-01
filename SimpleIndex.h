#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class SimpleIndex
{
	int headerSize; int next;//next free slot in index
	int numofNames;
	struct course
	{
		char courseID[6]; 
		string courseName =""; 
		string instructor="";
		short weeks;
	};
	struct IndexRecord
	{
		char PK[6]; int offset;
	};
	struct SecRecord
	{
		char name[50]="!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
		int pKloc;
		bool operator<(const SecRecord &r) const { 
			return strcmp(name, r.name) < 0;
		}
	};

	struct CourseId {
		int loc;
		char id[6];
		int nextid = -1;
		bool operator<(const CourseId &r) const { 
			return strcmp(id, r.id) < 0;
		}
	};
	const static int indexSize = 100;
	IndexRecord index[indexSize];
	vector<CourseId> courseid;
	vector<SecRecord> secindex;
	char dataFilePath[20] = "dataFile.txt";
	char indexFilePath[20] = "indexFile.txt";
	char secindexFilePath[20] = "secindexFile.txt";
	char courseidFilePath[20] = "courseid.txt";

public:
	SimpleIndex();
	int  IndexBinarySearch(char key[]);
	int  SecIndexBinarySearch(char key[]);
	void printCoursebyPK(char pk[]);
	void printCoursebySK();			
	void insertCourse();
	void updateCourse();
	void updateCoursebyinstructor();
	void deleteCourse(char []);
	int  deleteCoursebyIstractor();
	void saveIndex();
	void saveSecIndex();
	void saveCourseid();
	void loadIndex();
	void loadcourseidfile();
	void loadSecIndex();
	void ReconstructIndex();
	void sortIndex();
	void ChangeIndexState(bool state);
	bool getIndexState();
	bool exists(char name[20]);


};

