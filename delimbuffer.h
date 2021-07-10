class DelimFieldBuffer {
  public:
    DelimFieldBuffer(char delim = -1, int maxBytes = 1000);
    DelimFieldBuffer(const DelimFieldBuffer & buffer);
    DelimFieldBuffer & operator = (const DelimFieldBuffer & buffer);
    static void SetDefaultDelim(char delim);
    void Init(char delim, int maxBytes);
    void Clear();
    int Pack(const void * field, int size = -1);
    int Unpack(void * field, int maxBytes = -1);
    int Read(istream & stream);
    int Write(ostream & stream) const;
    int DRead(istream & stream, int recRef);
    int DWrite(ostream & stream, int recRef) const;
  
  protected:
    int Initialized;
    char * Buffer;
    int BufferSize;
    int MaxBytes;
    int NextByte;
    // int Packing;
    char Delim;
    static char DefaultDelim;
};

DelimFieldBuffer::DelimFieldBuffer(char delim, int maxBytes) {
  Init(delim, maxBytes);
}

DelimFieldBuffer::DelimFieldBuffer(const DelimFieldBuffer & buffer) {
  Init(buffer.Delim, buffer.MaxBytes);
}

DelimFieldBuffer & DelimFieldBuffer::operator = (const DelimFieldBuffer & buffer) {
  if (MaxBytes < buffer.BufferSize) return *this;
  Initialized = buffer.Initialized;
  BufferSize = buffer.BufferSize;
  memcpy(Buffer, buffer.Buffer, buffer.BufferSize);
  NextByte = buffer.NextByte;
  return *this;
}

// DelimFieldBuffer::DelimFieldBuffer(const DelimFieldBuffer & buffer) {

// }

void DelimFieldBuffer::SetDefaultDelim(char delim) {
  DefaultDelim = delim;
}

char DelimFieldBuffer::DefaultDelim = 0;

void DelimFieldBuffer::Clear() {
  NextByte = 0;
  // Packing = true;
}

void DelimFieldBuffer::Init(char delim, int maxBytes) {
  if (maxBytes < 0) maxBytes = 1000;
  MaxBytes = maxBytes;
  Buffer = new char[MaxBytes];
  BufferSize = 0;
  Initialized = true;
  if (delim == -1) Delim = DefaultDelim;
  else Delim = delim;
  Clear();
}

int DelimFieldBuffer::Pack(const void * field, int size) {
  int len;
  if (size >= 0) len = size;
  else len = strlen((char *)field);
  if (len > strlen((char *)field))
    return -1;
  
  int start = NextByte;
  NextByte += (len + 1);
  if (NextByte > MaxBytes) return -1;
  memcpy(&Buffer[start], field, len);
  Buffer[start + len] = Delim;
  BufferSize = NextByte;
  return len;
}

int DelimFieldBuffer::Unpack(void * field, int maxBytes) {
  int len;
  int start = NextByte;
  for (int i = start; i < BufferSize; i++)
    if (Buffer[i] == Delim) {
      len = i - start;
      break;
    }
  
  if (len == -1) return -1; // delimiter not found
  NextByte += (len + 1);
  if (NextByte > BufferSize) return -1;
  memcpy(field, &Buffer[start], len);
  if (maxBytes > len || maxBytes == -1)
    ((char *)field)[len] = 0;
  return len;
}

int DelimFieldBuffer::Read(istream & stream) {
  if (stream.eof()) return -1;
  int recAddr = stream.tellg();
  Clear();

  char bufferSize[3 + sizeof(char)];
  stream.read(bufferSize, sizeof(bufferSize));
  if (!stream.good()) {
    stream.clear();
    return -1;
  }

  from_chars(bufferSize, bufferSize + 3, BufferSize);
  if (BufferSize > MaxBytes) return -1; // buffer overflow

  stream.read(Buffer, BufferSize);
  if (!stream.good()) {
    stream.clear();
    return -1;
  }

  char ch;
  stream.get(ch);
  if (!stream.good()) {
    stream.clear();
    return -1;
  }

  return recAddr;
}

int DelimFieldBuffer::Write(ostream & stream) const {
  int recAddr = stream.tellp();

  char bufferSize[3 + sizeof(char)] = "";
  to_chars(bufferSize, bufferSize + 3, BufferSize);
  stream.write(bufferSize, sizeof(bufferSize));
  if (!stream.good()) return -1;

  stream.write(Buffer, BufferSize);
  if (!stream.good()) return -1;

  stream.put('\n');
  if (!stream.good()) return -1;

  return recAddr;
}

int DelimFieldBuffer::DRead(istream & stream, int recRef) {
  stream.seekg(recRef, ios::beg);
  if (stream.tellg() != recRef) return -1;
  return Read(stream);
}

int DelimFieldBuffer::DWrite(ostream & stream, int recRef) const {
  stream.seekp(recRef, ios::beg);
  if (stream.tellp() != recRef) return -1;
  return Write(stream);
}