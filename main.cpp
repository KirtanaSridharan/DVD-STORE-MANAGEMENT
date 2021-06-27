#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

class DVD {
  private: string id, title, genre, year, quantity, price;

  public:
    void opener(fstream& file, string filename, ios_base::openmode mode);
    void read();
    void pack(fstream &file);
    void unpack(fstream &file);
    void display(fstream &file);
    void printRecord();
};

void DVD::opener(fstream& file, string filename, ios_base::openmode mode) {
  file.open(filename, mode);
  if (!file.is_open()) {
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

void DVD::pack(fstream &file) {
  read();
  string record = id + "|" + title + "|" + genre + "|" + year + "|" + quantity + "|" + price + "|";
  file << record << "\n"; 
}

void DVD::unpack(fstream &file) {
  string extra;
  getline(file, id, '|');
  getline(file, title, '|');
  getline(file, genre, '|');
  getline(file, year, '|');
  getline(file, quantity, '|');
  getline(file, price, '|');
  getline(file, extra, '\n');
  
}

void DVD::printRecord() {
  cout << setw(5) << id << setw(50) << title << setw(40)
    << genre << setw(8) << year << setw(10) << quantity
    << setw(10) << price << "\n";
}

void DVD::display(fstream &file) {
  cout << setw(5) << "ID" << setw(50) << "TITLE" << setw(40)
    << "GENRE" << setw(8) << "YEAR" << setw(10) << "QUANTITY"
    << setw(10) << "PRICE" << "\n";

    while (true) {
      unpack(file);
      if (file.eof()) break;
      printRecord();
    }
}

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