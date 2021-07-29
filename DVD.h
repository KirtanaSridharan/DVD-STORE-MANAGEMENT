class Dvd {
  public:
    string ID;
    string Title;
    string Lang;
    string Genre;
    string Year;
    string Price;
    string Quantity;

    Dvd();
    void Clear();
    int getID();
    void readID(BTree tree);
    void readTitle();
    void readLang();
    void readGenre();
    void readYear();
    void readPrice();
    void readQuantity();
    void ReadFromStandardInput(BTree tree);
    void Pack(IOBuffer & buffer) const;
    int Unpack(IOBuffer & buffer);
    void PrintHeadings();
    void PrintRecord();
    void ModifyRecord();
};

Dvd::Dvd() {
  Clear();
}

void Dvd::Clear() {
  ID.clear();
  Title.clear();
  Lang.clear();
  Genre.clear();
  Year.clear();
  Price.clear();
  Quantity.clear();
}

int Dvd::getID() {
  return stoi(ID);
}

void Dvd::readID(BTree tree) {
  int valid;
  do {
    cout << setw(35) << "Enter the movie ID: ";
    cin >> ID;
    cin.sync();
    valid = validateID(tree, ID);
  } while (!valid);
}

void Dvd::readTitle() {
  cout << setw(35) << "Enter the movie title: ";
  cin >> ws;
  getline(cin, Title);
}

void Dvd::readLang() {
  cout << setw(35) << "Enter the language: ";
  cin >> Lang;
  cin.sync();
}

void Dvd::readGenre() {
  cout << setw(35) << "Enter the genre: ";
  cin >> ws;
  getline(cin, Genre);
}

void Dvd::readYear() {
  int valid;
  do {
    cout << setw(35) << "Enter the year [1950 - 2021]: ";
    cin >> Year;
    cin.sync();
    valid = validateField('Y', Year);
    if (!valid) cout << RED << "\nPlease enter a valid year.\n\n" << CYAN;
  } while (!valid);
}

void Dvd::readPrice() {
  int valid;
  do {
    cout << setw(35) << "Enter the price [200 - 5000]: ";
    cin >> Price;
    cin.sync();
    valid = validateField('P', Price);
    if (!valid) cout << RED << "\nPlease enter a valid price.\n\n" << CYAN;
  } while (!valid);
}

void Dvd::readQuantity() {
  int valid;
  do {
    cout << setw(35) << "Enter the quantity [0 - 10]: ";
    cin >> Quantity;
    cin.sync();
    valid = validateField('Q', Quantity);
    if (!valid) cout << RED << "\nPlease enter a valid quantity.\n\n" << CYAN;
  } while (!valid);
}

void Dvd::ReadFromStandardInput(BTree tree) {
  cout << CYAN;
  readID(tree);
  readTitle();
  readLang();
  readGenre();
  readYear();
  readPrice();
  readQuantity();
  cout << RESET;
}

void Dvd::Pack(IOBuffer & buffer) const {
  buffer.Clear();
  buffer.Pack(ID);
  buffer.Pack(Title);
  buffer.Pack(Lang);
  buffer.Pack(Genre);
  buffer.Pack(Year);
  buffer.Pack(Price);
  buffer.Pack(Quantity);
}

int Dvd::Unpack(IOBuffer & buffer) {
  Clear();
  int numBytes;
  numBytes = buffer.Unpack(ID);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Title);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Lang);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Genre);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Year);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Price);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Quantity);
  if (numBytes == -1)
    return false;
  return true;
}

void Dvd::PrintHeadings() {
  cout << BLUE;
  cout << string(140, '-') << "\n";
  cout << setw(5) << "ID" << setw(50) << "TITLE" << setw(20)
    << "LANGUAGE" << setw(30) << "GENRE" << setw(6) << "YEAR"
    << setw(10) << "QUANTITY" << setw(10) << "PRICE" << "\n";
  cout << string(140, '-') << "\n"; 
  cout << RESET;
}

void Dvd::PrintRecord() {
  cout << setw(5) << ID << setw(50) << Title << setw(20)
    << Lang << setw(30) << Genre << setw(6) << Year
    << setw(10) << Quantity << setw(10) << Price << "\n";
}

ostream & operator << (ostream & stream, Dvd d) {
  stream << BOLD << setw(15) <<  "ID: " << RESET << setw(50) << d.ID << "\n";
  stream << BOLD << setw(15) << "Title: " << RESET << setw(50) << d.Title << "\n";
  stream << BOLD << setw(15) << "Language: " << RESET << setw(50) << d.Lang << "\n";
  stream << BOLD << setw(15) << "Genre: " << RESET << setw(50) << d.Genre << "\n";
  stream << BOLD << setw(15) << "Year: " << RESET << setw(50) << d.Year << "\n";
  stream << BOLD << setw(15) << "Price: " << RESET << setw(50) << d.Price << "\n";
  stream << BOLD << setw(15) << "Quantity: " << RESET << setw(50) << d.Quantity << "\n";
  return stream;
}

void Dvd::ModifyRecord() {
  int choice;
  do {
    cout << BLUE << "\nWhich field would you like to modify?\n" << RESET;
    cout << setw(25) << "1. Title" << setw(50) << Title << "\n";
    cout << setw(25) << "2. Language" << setw(50) << Lang << "\n";
    cout << setw(25) << "3. Genre" << setw(50) << Genre << "\n";
    cout << setw(25) << "4. Year" << setw(50) << Year << "\n";
    cout << setw(25) << "5. Price" << setw(50) << Price << "\n";
    cout << setw(25) << "6. Quantity" << setw(50) << Quantity << "\n";
    cout << "\n7. Apply Changes\n";
    cout << "\nEnter the choice : ";
    cin >> choice;
    cout << CYAN;
    switch(choice){
      case 1: readTitle();    break;
      case 2: readLang();     break;
      case 3: readGenre();    break;
      case 4: readYear();     break;
      case 5: readPrice();    break;
      case 6: readQuantity(); break;
      case 7: cout << BLUE << ITALIC << "\nApplying changes...\n"; break;
      default: cout << RED << "Invalid choice! Please enter again." ;
    }
    cout << RESET;
  } while(choice != 7);
  cout << RESET;
}