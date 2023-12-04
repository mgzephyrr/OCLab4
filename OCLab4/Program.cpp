#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <exception>

using namespace std;

#define SEH 0
#define DEBUG 0
#define APPEND 0  //���������� ��������� ��������� �� ���� 3 (1 - �� ����� �� ��������� ������ �������, 0 - ���������)

struct Point {
	string name;
	double x;
	double y;

	Point(string name, double x, double y) { this->name = name; this->x = x; this->y = y; }
};

fstream& operator << (fstream& file, const Point& p)
{
	file << p.name << " " << p.x << " " << p.y << endl;

	return file;
}

ostream& operator << (ostream& s, const Point& p)
{
	return s << p.name << ": " << p.x << " " << p.y << endl;
}

DWORD filter(_EXCEPTION_POINTERS* exception, fstream& stream, string& filename) {
	if (exception->ExceptionRecord->ExceptionCode == 3765269347) {
		ofstream{ filename };
		stream.open(filename);

		return EXCEPTION_CONTINUE_EXECUTION;
	}
	
	return EXCEPTION_EXECUTE_HANDLER;
}

void open_a_file(fstream& stream, string& filename) {
	stream.exceptions(ios::failbit);

#if SEH
	__try {
		stream.open(filename);
		stream << "� ���� �������� ����������" << endl;
	}
	__except (filter(GetExceptionInformation(), stream, filename)) {
		stream << "� ���� �������� ����������" << endl;
	}
#else
	try {
		stream.open(filename);
		stream << "� ���� �������� ����������" << endl;
	}
	catch (ios_base::failure) {
		ofstream{ filename };
		stream << "����� �� ���� � ���������� ��������� �� ������ �������" << endl;
	}
#endif
}

void read_lines_from_file(vector<string>& vec, fstream& stream, string& temp) {
	__try {
		while (getline(stream, temp)) {
			vec.push_back(temp);
		}
	}
	__except (GetExceptionInformation()->ExceptionRecord->ExceptionCode == 3765269347 ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {

	}
}

int main() {
	system("chcp 1251 >> null");
	string temp;

	// ��� 1

	string filename = "file.txt"; fstream txtfile;
	open_a_file(txtfile, filename);
	
	// thousands lines of code here...

	if (txtfile)
		txtfile.close();

	// ��� 2

	txtfile.open(filename);
	vector<string> line;
	read_lines_from_file(line, txtfile, temp);
	cout << line[0] << endl;
	txtfile.close();

	// ��� 3

#if APPEND
	txtfile.open(filename, ios::app);
#else
	txtfile.open(filename, ios::out | ios::trunc);
#endif

	txtfile.seekg(0, ios::end);
	auto position_offset = txtfile.tellg();

	string input;
	cout << "������� ���������� � ���� (0 - ����� �����): ";
	cin >> input;

	while (input != "0") {
		txtfile << input << endl;

		cout << "������� ���������� � ���� (0 - ����� �����): ";
		cin >> input;
	}

	txtfile.close();

#if DEBUG
	txtfile.open(filename);
	txtfile.seekg(position_offset, ios::beg);

	while (getline(txtfile, line))
		cout << line << endl;

	txtfile.close();
#endif

	// ��� 4

	vector<string> values;

	txtfile.open(filename);
	txtfile.seekg(position_offset, ios::beg);

	read_lines_from_file(values, txtfile, temp);

	txtfile.close();

	for (int i = 0; i < values.size(); i++) {
		cout << values[i] << endl;
	}

	// ��� 6

	cout << endl << "Press <Enter> to continue...";
	cin.ignore(INT_MAX, '\n');
	cin.get();

	system("cls");

	filename = "Points.txt";
	ofstream{ filename };
	txtfile.open(filename, ios::out | ios::trunc);
	int size;

	cout << "������� ���-�� �����, ������� �� ������ �������� � ����: ";
	cin >> size;
	cout << endl;

	for (int i = 0; i < size; i++) {
		string name; double x, y;

		cout << "�������� ����� " << i + 1 << ": ";
		cin >> name;

		cout << "x: "; cin >> x;

		cout << "y: "; cin >> y; cout << endl;

		Point* p = new Point(name, x, y);

		txtfile << *p << endl;
	}

	txtfile.close();
	txtfile.open(filename);

	system("cls");

	for (int i = 0; i < size; i++) {
		string name; double x, y;
		txtfile >> name; txtfile >> x; txtfile >> y;

		Point* p = new Point(name, x, y);

		cout << *p << endl;
	}
	txtfile.close();
}