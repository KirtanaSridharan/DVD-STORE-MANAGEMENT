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
    void ModifyRecord();
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

  cout << CYAN;
  int valid, temp;
  do {
    cout << "Enter the movie ID: ";
    cin >> ID;
    cin.sync();
    valid = validateID(tree, ID);
    if (!valid) cout << "\nInvalid ID. Please try again.\n";
  } while (!valid);

  cout << "Enter the movie title: ";
  cin >> ws;
  getline(cin, Title);

  cout << "Enter the language: ";
  cin >> Lang;
  cin.sync();

  cout << "Enter the movie genre(s): ";
  cin >> ws;
  getline(cin, Genre);

  do {
    cout << "Enter the year of release: ";
    cin >> Year;
    cin.sync();
    valid = validateField('Y', Year);
    if (!valid) cout << "\nPlease enter a valid year.\n";
  } while (!valid);

  do {
    cout << "Enter the price: ";
    cin >> Price;
    cin.sync();
    valid = validateField('P', Price);
    if (!valid) cout << "\nPlease enter a valid price.\n";
  } while (!valid);

  do {
    cout << "Enter the quantity: ";
    cin >> Quantity;
    cin.sync();
    valid = validateField('Q', Quantity);
    if (!valid) cout << "\nPlease enter a valid number.\n";
  } while (!valid);
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
  stream << "\nID: " << d.ID << "\nTitle: " << d.Title
    << "\nLanguage: " << d.Lang << "\nGenre: "
    << d.Genre << "\nYear: " << d.Year << "\nQuantity: "
    << d.Quantity << "\nPrice: " << d.Price << "\n";
  return stream;
}


void Dvd::ModifyRecord(){
  string temp;
  int choice;
  do{
    cout<<*this<<"\n";
    cout<< "Which field would you like to modify?\n";
    cout << "1. ID\t2. Title\t3. Language\t4. Genre\t5. Year\t6. Quantity\t7. Price\t8. Back to main menu";
    cout<< "\nEnter the field no. : ";
    cin >> choice;
    if(choice>0 && choice<8){
      cout<<"\nEnter the new value: ";
      cin >> ws;
      getline(cin, temp);
    }
    
    switch(choice){
      case 1:  ID = temp;  break;
      case 2:  Title = temp;  break;
      case 3:  Lang = temp;  break;
      case 4:  Genre = temp;  break;
      case 5:  Year = temp;  break;
      case 6:  Quantity = temp;  break;
      case 7:  Price = temp;  break;
      case 8: cout<<"\nApplying modifications..."; break;
      default: cout<<"Invalid choice. Please enter again." ;
    }

  }while(choice!=8);
}