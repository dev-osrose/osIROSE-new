// Props to ExJam for this code :D

class CStrStb
{
public:
	CStrStb():_Data(0){}
	CStrStb(char* filename) : _Data(0){ LoadSTB(filename); }
	~CStrStb(){CleanUp();}

	bool LoadSTB(char* path)
    {
		CleanUp();

		CRoseFile* fh = new CRoseFile(path, FM_READ | FM_BINARY);
		if(!fh->IsOpen()){
			delete fh;
			return false;
		} // if is open

		fh->Seek(4, SEEK_CUR);
		dword offset = fh->Get<dword>();
		
		_Rows = fh->Get<dword>();
		_Columns = fh->Get<dword>();

		_Rows--;
		_Columns--;
		
		fh->SetAbsPos(offset);
		_Data = new char**[_Rows];
		for(dword i = 0; i < _Rows; i++)
			_Data[i] = new char*[_Columns];

		for(dword i = 0; i < _Rows; i++){
			for(dword j = 0; j < _Columns; j++){
				dword len = fh->Get<word>();
				if(len == 0){
					_Data[i][j] = NULL;
					continue;
				} // if len
				_Data[i][j] = new char[len+1];
				fh->Read(_Data[i][j], len, 1);
				_Data[i][j][len] = 0;
			} // for j
		} // for i

		fh->Close();
		delete fh;
		return true;
    } // bool

	char* Data(dword Row, dword Column){
		if(Row > _Rows) return NULL;
		if(Column > _Columns) return NULL;

		return _Data[Row][Column];
	}

	dword Rows(){ return _Rows; }
	dword Columns(){ return _Columns; }

private:
	void CleanUp(){
		if(_Data != NULL){
			for(dword i = 0; i < _Rows; i++){
				for(dword j = 0; j < _Columns; j++){
					delete [] _Data[i][j];
				}
				delete [] _Data[i];
			}
			delete [] _Data;
		}
		_Rows = 0;
		_Columns = 0;
	}

private:
	dword _Rows;
	dword _Columns;

	char*** _Data;
};
