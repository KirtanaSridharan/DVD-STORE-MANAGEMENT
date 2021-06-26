#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

#define DVD_FILENAME "dvd-list.txt"

fstream DVD_FILE;

class DVD {
  private: string id, title, genre, year, quantity, price;

  public:
    void opener(fstream& file, string file_name, ios_base::openmode mode);
    void read();
    void pack();
    void unpack();
    void display();
    void printRecord();
};

void DVD::opener(fstream& file, string file_name, ios_base::openmode mode) {
  file.open(file_name, mode);
  if (!file) {
    cout << "Error opening the file.\n";
    exit(1);
  }
}

void DVD::read() {
  cout << "Enter id: ";
  cin >> id;
  cout << "Enter title: ";
  getline(cin >> ws, title);
  cout << "Enter genre: ";
  getline(cin >> ws, genre);
  cout << "Enter year: ";
  cin >> year;
  cout << "Enter quantity: ";
  cin >> quantity;
  cout << "Enter price: ";
  cin >> price;
}

void DVD::pack() {
  read();
  string record = id + "|" + title + "|" + genre + "|" + year + "|" + quantity + "|" + price + "|";
  DVD_FILE << record << "\n"; 
}

void DVD::unpack() {
  string extra;
  getline(DVD_FILE, id, '|');
  getline(DVD_FILE, title, '|');
  getline(DVD_FILE, genre, '|');
  getline(DVD_FILE, year, '|');
  getline(DVD_FILE, quantity, '|');
  getline(DVD_FILE, price, '|');
  getline(DVD_FILE, extra, '\n');
  
}

void DVD::printRecord() {
  cout << setw(5) << id << setw(50) << title << setw(40)
    << genre << setw(8) << year << setw(10) << quantity
    << setw(10) << price << "\n";
}

void DVD::display() {
  cout << setw(5) << "ID" << setw(50) << "TITLE" << setw(40)
    << "GENRE" << setw(8) << "YEAR" << setw(10) << "QUANTITY"
    << setw(10) << "PRICE" << "\n";

    while (true) {
      unpack();
      if (DVD_FILE.eof()) break;
      printRecord();
    }
}

int main() {
  cout << setiosflags(ios::left);
  DVD dvd;
  int choice;
  do {
    cout << "\n1. Insert\t2. Display\t3. Exit";
    cout << "\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
      case 1:
        dvd.opener(DVD_FILE, DVD_FILENAME, ios::app | ios::out);
        dvd.pack();
        break;
      
      case 2:
        dvd.opener(DVD_FILE, DVD_FILENAME, ios::in);
        dvd.display();
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