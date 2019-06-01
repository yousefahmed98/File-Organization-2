#include "SimpleIndex.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include<algorithm>
using namespace std;



SimpleIndex::SimpleIndex()
{
	next = 0;
	headerSize = 1;
}




bool SimpleIndex::exists(char name[20])
{
	ifstream f(name);
	if (f.fail())
	{
		return false;
	}
	else
	{
		f.close();
		return true;
	}
}
//----------------------------load------------------------------
//----------------------------Index-----------------------------
void SimpleIndex::loadIndex()
{
	if (!exists(dataFilePath))
	{
		next = 0; ofstream fout;
		fout.open(dataFilePath, ios::app | ios::out | ios::binary);
		fout << 0;	fout.close();
	}
	else if (!getIndexState())
	{
		ifstream fin(indexFilePath);		next = 0;
		while (true)
		{
			IndexRecord temp;
			fin.read((char*)&temp, sizeof(temp)); if (fin.eof()) break;
			index[next] = temp;			next++;
		}
		fin.close();
	}
	else ReconstructIndex();
	
}		
//-----------------------------------------------------------------
//----------------------------secIndex-----------------------------
void SimpleIndex::loadSecIndex() {
	if (!exists(secindexFilePath))
	{
		numofNames = 0; ofstream fout;
		fout.open(secindexFilePath, ios::app | ios::out | ios::binary);
		fout << 0;	fout.close();
	}
	else {

		ifstream fout;
		fout.open(secindexFilePath, ios::app | ios::out | ios::binary);
		fout.read((char*)&numofNames, sizeof(numofNames));
		SecRecord temp;
		for (int i = 0; i < numofNames; i++) {

			fout.read((char*)&temp, sizeof(temp));
			secindex.push_back(temp);
		}
	}
}
//-----------------------------------------------------------------
//----------------------------courseid-----------------------------
void SimpleIndex::loadcourseidfile() {
	if (!exists(courseidFilePath))
	{
		next = 0; ofstream fout;
		fout.open(courseidFilePath, ios::app | ios::out | ios::binary);
		fout << 0;	fout.close();
	}
	else {
		ifstream fout;
		fout.open(courseidFilePath, ios::app | ios::out | ios::binary);
		
		CourseId temp;
		for (int i = 0; i < next; i++) {

			fout.read((char*)&temp, sizeof(temp));
			courseid.push_back(temp);
		}
	}
}
//-----------------------------------------------------------------
//_________________________________________________________________

void SimpleIndex::sortIndex() //bubble sort
{
	int len = next - 1;
	IndexRecord temp;
	for (int i = 0; i<len; i++)
		for (int j = 0; j<len - i; j++)
		{
			if (atoi(index[j].PK)>atoi(index[j + 1].PK))
			{
				temp = index[j];
				index[j] = index[j + 1];
				index[j + 1] = temp;
			}
		}
}
 //------------------search---------------------------------------
int SimpleIndex::IndexBinarySearch(char key[])
{

	int size = next;
	int low = 0, high = size - 1;
	while (low <= high)
	{
		int middle = (low + high) / 2;
		if (strcmp(index[middle].PK, key) == 0)
			return middle;
		else if (atoi(index[middle].PK)<atoi(key))
			low = middle + 1;
		else
			high = middle - 1;
	}
	return -1;
}
//--------------------------secsearch---------------------------------
int SimpleIndex::SecIndexBinarySearch(char key[])
{

	int size = numofNames;
	int low = 0, high = size - 1;
	while (low <= high)
	{
		int middle = (low + high) / 2;
		if (strcmp(secindex[middle].name, key)==0)
			return middle;
		else if (strcmp(secindex[middle].name, key)<0)
			low = middle + 1;
		else
			high = middle - 1;
	}
	return -1;
}


void SimpleIndex::insertCourse()
{
	ChangeIndexState(true);
	ofstream fout; fout.open(dataFilePath, ios::app | ios::out | ios::binary);

	course s;
	cout << "\nEnter couresID: ";	cin.getline(s.courseID, 5);
	cout << "\nEnter courseName: ";	cin >> s.courseName;
	cout << "\nEnter instractor: ";	cin >> s.instructor;
	cout << "\nEnter weeks: ";	cin >> s.weeks;
	cin.ignore();


	char buffer[200];
	strcpy_s(buffer, s.courseID);	strcat_s(buffer, "|");
	short len = strlen(buffer);	
	int j = 0;
	for (; j < s.courseName.length(); j++) {
		buffer[len++] = s.courseName[j];
	}
	buffer[len++] = '|';
	j = 0;
	for ( ; j < s.instructor.length(); j++) {
		buffer[len++] = s.instructor[j];
	}
	
	buffer[len++] = '|';

	fout.seekp(0, ios::end); 			int addess = fout.tellp();
	fout.write((char*)&len, sizeof(len)); 	fout.write(buffer, len);
	fout.write((char*)&s.weeks, sizeof(s.weeks));

	IndexRecord temp;  	strcpy_s(temp.PK, s.courseID);	temp.offset = addess;
	index[next] = temp;
	
	CourseId temp2;
	strcpy_s(temp2.id, s.courseID);
	temp2.loc = next;
	courseid.push_back( temp2);
	char key[50]= "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
	for (int z = 0; z < s.instructor.length(); z++) key[z] = s.instructor[z];
	int pos = SecIndexBinarySearch(key);
	if ( pos == -1) {
		SecRecord temp3;
		for (int i = 0; i < s.instructor.length();i++)  temp3.name[i] = s.instructor[i];
		temp3.pKloc = next;
		secindex.push_back(temp3);
		numofNames++;
		sort(secindex.begin(), secindex.end());

	}
	else {
		if (secindex[pos].pKloc == -1) {
			secindex[pos].pKloc = next;
		}
		else {
			int x = courseid[secindex[pos].pKloc].nextid;
			if (x == -1) { courseid[secindex[pos].pKloc].nextid = next; }
			else {
				int lastpos = -1;
				while (x != -1) {
					lastpos = x;
					x = courseid[x].nextid;
				}
				courseid[lastpos].nextid = next;
			}
		}
	}
	next++;
	
	sortIndex();
	fout.close();
}
//-----------------------------------------
//------------------------------------

void SimpleIndex::updateCourse() {
	char PK[6];
	cout << "enter ID :";
	cin >> PK;
	cin.ignore();
	deleteCourse(PK);
	insertCourse();
}
void SimpleIndex::updateCoursebyinstructor() {
	int x=deleteCoursebyIstractor();
		cout << "num of updated courses = " << x;
	while (x--) {
		insertCourse();
	}

}
 //-------------------------------------------
//----------------------------------------------

void SimpleIndex::deleteCourse(char PK[])
{
	ChangeIndexState(true);
	

	fstream fout(dataFilePath, ios::binary | ios::out | ios::in);
	int address = IndexBinarySearch(PK);
	if (address == -1)
	{
		cout << "\ncourse not found";  return;
	}
	fout.seekp(index[address].offset + 2, ios::beg);		fout << '*';


	for (int i = address; i<next - 1; i++) //shift up to delete from index
		index[i] = index[i + 1];
	

	next--;

	fout.close();

}

int  SimpleIndex::deleteCoursebyIstractor()
{
	int numofdeleted=0;
	ChangeIndexState(true);
	char SK[50] = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"; cout << "\nenter instractor to delete \n";
	string instractor;
	cin >> instractor;
	cin.ignore();
	for (int i = 0; i < instractor.length(); i++)SK[i] = instractor[i];
	int address = SecIndexBinarySearch(SK);
	if (address == -1)
	{
		cout << "\n instractor not found";  return 0;
	}
	if (secindex[address].pKloc ==-1) {
		cout << "\n instractor not found";  return 0;
	}
	else {
		int x = secindex[address].pKloc;
		while (x != -1) {
		
			deleteCourse(courseid[x].id);
			numofdeleted++;
			x = courseid[x].nextid;
		}
		secindex[address].pKloc = -1;
		for (int i = address; i<numofNames - 1; i++) //shift up to delete from secindex
			secindex[i] = secindex[i + 1];
	}
	return numofdeleted;
}



void SimpleIndex::printCoursebyPK(char PK[])
{
	 

	ifstream fin;	fin.open(dataFilePath, ios::binary | ios::in);

	int address = IndexBinarySearch(PK);
	if (address == -1)
	{
		cout << "\nthis course not exists";  return;
	}

	short len; course s;
	fin.seekg(index[address].offset, ios::beg);  fin.read((char*)&len, sizeof(len));

	char *buffer = new char[len];
	fin.read(buffer, len);			istringstream stream(buffer);

	stream.getline(s.courseID, 5, '|'); 
	int i = strlen(s.courseID)+1;
	while (true)
	{
		s.courseName += buffer[i++];
		if (buffer[i] == '|') break;

	}
	i++;
	while (true)
	{
		s.instructor += buffer[i++];
		if (buffer[i] == '|') break;

	}
	fin.read((char*)&s.weeks, sizeof(s.weeks));
	


	cout << endl << "ID: " << s.courseID << "  Name: " << s.courseName << "  instructor: " << s.instructor << "  weeks: " << s.weeks << endl;
	fin.close();  delete[]buffer;
}
//----------------------------------------
//-----------------------------------------
void SimpleIndex::printCoursebySK()
{
	char SK[50] = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"; cout << "\n enter instractor you want to display \n"; 
	string instractor;
	cin >> instractor;
	cin.ignore();
	for (int i = 0; i < instractor.length(); i++)SK[i] = instractor[i];

	int address = SecIndexBinarySearch(SK);
	if (address == -1)
	{
		cout << "\nthis course not exists";  return;
	}

	short len; course s;
	int x = (secindex[address].pKloc);
	if (x == -1) { cout << "\nthis course not exists";  return; }
	while (x!= -1) {
		printCoursebyPK(courseid[x].id);
		x = courseid[x].nextid;
	}
}
//__________________________saveindexes_____________________//

void SimpleIndex::saveIndex()
{
	ofstream fout(indexFilePath, ios::trunc);
	for (int i = 0; i<next; i++)
	{
		IndexRecord temp = index[i];
		fout.write((char*)&temp, sizeof(temp));
	}
	fout.close();
	ChangeIndexState(false);// index is up to date
}
void SimpleIndex::saveSecIndex() {
	ofstream fout(secindexFilePath, ios::trunc);
	fout.write((char*)&numofNames, sizeof(numofNames));
	
	for (int i = 0; i<numofNames; i++)
	{
		SecRecord temp = secindex[i];
		fout.write((char*)&temp, sizeof(temp));
	}
	fout.close();
}	
void SimpleIndex::saveCourseid() {
	ofstream fout(courseidFilePath, ios::trunc);

	for (int i = 0; i<next; i++)
	{
		CourseId temp = courseid[i];
		fout.write((char*)&temp, sizeof(temp));
	}
	fout.close();
}








void SimpleIndex::ReconstructIndex()
{
	ifstream fin(dataFilePath);  		next = 0;
	fin.seekg(headerSize, ios::beg); //skip header
	
	while (true)
	{
		IndexRecord temp;
		course c;
		int offset = fin.tellg();
		short len; fin.read((char*)&len, sizeof(len)); if (fin.eof()) break;
		char *buffer = new char[len];
		fin.read(buffer, len); if (buffer[0] == '*') continue;
		istringstream strbuf(buffer);
		strbuf.getline(temp.PK, 5, '|'); temp.offset = offset;
		int i = strlen(temp.PK) + 1;
		int v= strlen(temp.PK) ;
		while (true)
		{
			c.courseName += buffer[i++];
			if (buffer[i] == '|') break;

		}
		i++;
		while (true)
		{
			c.instructor += buffer[i++];
			if (buffer[i] == '|') break;

		}
		fin.read((char*)&c.weeks, sizeof(c.weeks));
		index[next] = temp;
		CourseId temp2;
		for (int c = 0; c < v; c++)
			temp2.id[c] = temp.PK[c];
		temp2.nextid = -1;
		temp2.loc = next;
		courseid.push_back(temp2);

		char key[50] = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
		for (int z = 0; z < c.instructor.length(); z++) key[z] = c.instructor[z];
		int pos = SecIndexBinarySearch(key);
		if (pos == -1) {
			SecRecord temp3;
			for (int i = 0; i < c.instructor.length(); i++)  temp3.name[i] = c.instructor[i];
			temp3.pKloc = next;
			secindex.push_back(temp3);
			numofNames++;
			sort(secindex.begin(), secindex.end());

		}
		else {
			if (secindex[pos].pKloc == -1) {
				secindex[pos].pKloc = next;
			}
			else {
				int x = courseid[secindex[pos].pKloc].nextid;
				if (x == -1) { courseid[secindex[pos].pKloc].nextid = next; }
				else {
					int lastpos = -1;
					while (x != -1) {
						lastpos = x;
						x = courseid[x].nextid;
					}
					courseid[lastpos].nextid = next;
				}
			}
		}
		next++;

	}
	fin.close(); 
	sortIndex();
}

void SimpleIndex::ChangeIndexState(bool state)
{
	fstream fout(dataFilePath, ios::out | ios::in);
	fout.seekp(0, ios::beg);
	fout << state;
	fout.close();
}

bool SimpleIndex::getIndexState()
{
	bool state;
	ifstream fout(dataFilePath);
	fout.seekg(0, ios::beg);
	fout >> state;
	fout.close();
	return state;
}



