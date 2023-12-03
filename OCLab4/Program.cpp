#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <windows.h>

using namespace std;

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

int main() {

	// ��� 1
	setlocale(LC_ALL, "ru");

	string filename = "file.txt";
	fstream txtfile(filename);
	if (!txtfile) {
		ofstream{ filename };
		txtfile.open(filename);
	}
	txtfile << "� ���� �������� ����������" << endl;

	// thousands lines of code here...

	if (txtfile)
		txtfile.close();

	// ��� 2

	txtfile.open(filename);
	string line;
	getline(txtfile, line);
	cout << line << endl;
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

	while (getline(txtfile, line)) {
		values.push_back(line);
		cout << line << endl;
	}

	txtfile.close();

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