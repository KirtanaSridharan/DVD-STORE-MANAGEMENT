class BufferFile {
  public:
    BufferFile(DelimFieldBuffer &);
    int Open(const char * filename, ios_base::openmode MODE);
    void Close();
    int Read(int recAddr = -1);
    int Write(int recAddr = -1);
    int Append();

  protected:
    DelimFieldBuffer & Buffer;
    fstream FILE;
};

BufferFile::BufferFile(DelimFieldBuffer & buffer): Buffer(buffer) {}

int BufferFile::Open(const char * filename, ios_base::openmode MODE) {
  FILE.open(filename, MODE);
  return FILE.good();
}

void BufferFile::Close() {
  FILE.close();
}

int BufferFile::Read(int recAddr) {
  if (recAddr == -1) return Buffer.Read(FILE);
  else return Buffer.DRead(FILE, recAddr);
}

int BufferFile::Write(int recAddr) {
  if (recAddr == -1) return Buffer.Write(FILE);
  else return Buffer.DWrite(FILE, recAddr);
}

int BufferFile::Append() {
  FILE.seekp(0, ios_base::end);
  return Buffer.Write(FILE);
}

