class Dvd {
  public:
    char ID[5];
    char Title[50];
    char Production[50];
    char Genre[50];
    char Year[5];
    char Quantity[3];
    char Price[5];

    Dvd();
    void Clear();
    int Pack(DelimFieldBuffer & buffer) const;
    int Unpack(DelimFieldBuffer & buffer);
    void PrintHeadings();
    void PrintRecord();
    // void Dvd::Read(istream & stream);
    // void Dvd::Print(ostream & stream) const;
};

Dvd::Dvd() {
  Clear();
}

void Dvd::Clear() {
  ID[0] = 0;
  Title[0] = 0;
  Production[0] = 0;
  Genre[0] = 0;
  Year[0] = 0;
  Quantity[0] = 0;
  Price[0] = 0;
}

int Dvd::Pack(DelimFieldBuffer & buffer) const {
  int numBytes;
  buffer.Clear();
  numBytes = buffer.Pack(ID);
  if (numBytes == -1) return false;
  numBytes = buffer.Pack(Title);
  if (numBytes == -1) return false;
  numBytes = buffer.Pack(Production);
  if (numBytes == -1) return false;
  numBytes = buffer.Pack(Genre);
  if (numBytes == -1) return false;
  numBytes = buffer.Pack(Year);
  if (numBytes == -1) return false;
  numBytes = buffer.Pack(Quantity);
  if (numBytes == -1) return false;
  numBytes = buffer.Pack(Price);
  if (numBytes == -1) return false;
  return true;
}

int Dvd::Unpack(DelimFieldBuffer & buffer) {
  Clear();
  int numBytes;
  numBytes = buffer.Unpack(ID);
  if (numBytes == -1) return false;
  numBytes = buffer.Unpack(Title);
  if (numBytes == -1) return false;
  numBytes = buffer.Unpack(Production);
  if (numBytes == -1) return false;
  numBytes = buffer.Unpack(Genre);
  if (numBytes == -1) return false;
  numBytes = buffer.Unpack(Year);
  if (numBytes == -1) return false;
  numBytes = buffer.Unpack(Quantity);
  if (numBytes == -1) return false;
  numBytes = buffer.Unpack(Price);
  if (numBytes == -1) return false;
  return true;
}

void Dvd::PrintHeadings() {
  cout << setw(5) << "ID" << setw(50) << "TITLE" << setw(20)
    << "PRODUCTION" << setw(30) << "GENRE" << setw(6) << "YEAR"
    << setw(10) << "QUANTITY" << setw(10) << "PRICE" << "\n";
}

void Dvd::PrintRecord() {
  cout << setw(5) << ID << setw(50) << Title << setw(20)
    << Production << setw(30) << Genre << setw(6) << Year
    << setw(10) << Quantity << setw(10) << Price << "\n";
}

istream & operator >> (istream & stream, Dvd & d) {
  cout << "Enter the movie ID: ";
  stream >> d.ID;

  cout << "Enter the movie title: ";
  stream >> ws;
  stream.getline(d.Title, 50);

  cout << "Enter the production company: ";
  stream >> ws;
  stream.getline(d.Production, 50);

  cout << "Enter the movie genre(s): ";
  stream >> ws;
  stream.getline(d.Genre, 50);

  cout << "Enter the year of release: ";
  stream >> d.Year;

  cout << "Enter the quantity: ";
  stream >> d.Quantity;

  cout << "Enter the price: ";
  stream >> d.Price;

  return stream;
}

ostream & operator << (ostream & stream, Dvd d) {
  stream << "\nID: " << d.ID << "\nTitle: " << d.Title
    << "\nProduction: " << d.Production << "\nGenre: "
    << d.Genre << "\nYear: " << d.Year << "\nQuantity: "
    << d.Quantity << "\nPrice: " << d.Price << "\n";
  return stream;
}