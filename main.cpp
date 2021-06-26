#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>


using namespace std;

#define DVD_FILENAME "dvd-list.txt"

fstream DVD_FILE;

class DVD {
  private: string id, title, genre, release_date, quantity, price;

  public:
    void open(ios_base::openmode mode);
    void read();
    void pack();
    void unpack();
    void display();
    void printRecord();
};

void DVD::open(ios_base::openmode mode) {
  DVD_FILE.open(DVD_FILENAME, mode);
  if (!DVD_FILE) {
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
  cout << "Enter release_date: ";
  cin >> release_date;
  cout << "Enter quantity: ";
  cin >> quantity;
  cout << "Enter price: ";
  cin >> price;
}

void DVD::pack(){
  read();
  string record = id + "|" + title + "|" + genre + "|" + release_date + "|" + quantity + "|" + price + "|";
  DVD_FILE << record <<"\n"; 
}

void DVD::unpack(){
  string extra;
  getline(DVD_FILE, id, '|');
  getline(DVD_FILE, title, '|');
  getline(DVD_FILE, genre, '|');
  getline(DVD_FILE, release_date, '|');
  getline(DVD_FILE, quantity, '|');
  getline(DVD_FILE, price, '|');
  getline(DVD_FILE, extra, '\n');
  
}

void DVD::printRecord() {
  cout << setw(5) << id << setw(30) << title << setw(20)
    << genre << setw(15) << release_date << setw(6) << quantity
    << setw(10) << price << "\n";
}

void DVD::display(){
  cout << setw(5) << "ID" << setw(30) << "TITLE" << setw(20)
    << "GENRE" << setw(15) << "RELEASE DATE" << setw(6) << "QUANTITY"
    << setw(10) << "PRICE" << "\n";

    while(true){
      unpack();
      if(DVD_FILE.eof())  break;
      printRecord();
    }
}

int main() {
  cout << setiosflags(ios::left);
  fstream DVD_FILE;
  DVD dvd;
  int choice;
  do {
    cout << "\n1. Insert\t2. Display\t3. Exit";
    cout << "\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
      case 1:
        dvd.open(ios::out | ios::app);
        dvd.pack();
        break;
      
      case 2:
        dvd.open(ios::in);
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