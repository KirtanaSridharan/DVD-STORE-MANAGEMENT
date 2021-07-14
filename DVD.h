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
    void ReadFromStandardInput(BTree & tree);
    void Pack(IOBuffer & buffer) const;
    int Unpack(IOBuffer & buffer);
    void PrintHeadings();
    void PrintRecord();
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

void Dvd::ReadFromStandardInput(BTree & tree) {
  cout << "Enter the movie ID: ";
  cin >> ID;

  cout << "Enter the movie title: ";
  cin >> ws;
  getline(cin, Title);

  cout << "Enter the language: ";
  cin >> Lang;

  cout << "Enter the movie genre(s): ";
  cin >> ws;
  getline(cin, Genre);

  cout << "Enter the year of release: ";
  cin >> Year;

  cout << "Enter the price: ";
  cin >> Price;

  cout << "Enter the quantity: ";
  cin >> Quantity;
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
  cout << setw(5) << "ID" << setw(50) << "TITLE" << setw(20)
    << "LANGUAGE" << setw(30) << "GENRE" << setw(6) << "YEAR"
    << setw(10) << "QUANTITY" << setw(10) << "PRICE" << "\n";
}

void Dvd::PrintRecord() {
  cout << setw(5) << ID << setw(50) << Title << setw(20)
    << Lang << setw(30) << Genre << setw(6) << Year
    << setw(10) << Quantity << setw(10) << Price << "\n";
}

istream & operator >> (istream & stream, Dvd & d) {
  cout << "Enter the movie ID: ";
  stream >> d.ID;

  cout << "Enter the movie title: ";
  stream >> ws;
  getline(stream, d.Title);

  cout << "Enter the language: ";
  stream >> d.Lang;

  cout << "Enter the movie genre(s): ";
  stream >> ws;
  getline(stream, d.Genre);

  cout << "Enter the year of release: ";
  stream >> d.Year;

  cout << "Enter the price: ";
  stream >> d.Price;

  cout << "Enter the quantity: ";
  stream >> d.Quantity;

  return stream;
}

ostream & operator << (ostream & stream, Dvd d) {
  stream << "\nID: " << d.ID << "\nTitle: " << d.Title
    << "\nLanguage: " << d.Lang << "\nGenre: "
    << d.Genre << "\nYear: " << d.Year << "\nQuantity: "
    << d.Quantity << "\nPrice: " << d.Price << "\n";
  return stream;
}