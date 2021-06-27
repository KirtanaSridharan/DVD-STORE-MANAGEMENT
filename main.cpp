#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "DVD.h"

using namespace std;

int main() {
  cout << setiosflags(ios::left);

  fstream DVD_FILE;
  const string DVD_FILENAME = "dvd-list.txt";
  DVD dvd;

  int choice;
  do {
    cout << "\n1. Insert\t2. Display\t3. Exit";
    cout << "\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
      case 1:
        dvd.opener(DVD_FILE, DVD_FILENAME, ios::out | ios::app);
        dvd.pack(DVD_FILE);
        break;
      
      case 2:
        dvd.opener(DVD_FILE, DVD_FILENAME, ios::in);
        dvd.display(DVD_FILE);
        break;
      
      case 3:
        cout << "\nProgram terminated.\n";
        break;
      
      default: cout << "\nInvalid choice. Please enter again.\n";
    }
    DVD_FILE.close();
  } while (choice != 3);
  return 0;
}