template <class RecType>
class RecordFile: public BufferFile {
  public:
    int Read(RecType & record, int recAddr = -1);
    int Write(const RecType & record, int recAddr = -1);
    int Append(const RecType & record);
    RecordFile(DelimFieldBuffer & buffer): BufferFile(buffer) {}
};

template <class RecType>
int RecordFile<RecType>::Read(RecType & record, int recAddr) {
  int readAddr, result;
  readAddr = BufferFile::Read(recAddr);
  if (readAddr == -1) return -1;
  result = record.Unpack(Buffer);
  if (!result) return -1;
  return readAddr;
}

template <class RecType>
int RecordFile<RecType>::Write(const RecType & record, int recAddr) {
  int result;
  result = record.Pack(Buffer);
  if (!result) return -1;
  return BufferFile::Write(recAddr);
}

template <class RecType>
int RecordFile<RecType>::Append(const RecType & record) {
  int result;
  result = record.Pack(Buffer);
  if (!result) return -1;
  return BufferFile::Append();
}