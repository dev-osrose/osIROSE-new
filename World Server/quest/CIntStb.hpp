// Props to ExJam for this code :D

class CIntStb 
{
public:
	CIntStb() : rows(0) {}
	CIntStb(char* filename) : rows(0){ LoadSTB(filename); }
	~CIntStb(){ CleanUp(); }

	int LoadSTB( char* filename ){
		FILE* fh;
		char tmpfield[512];
		unsigned dataoffset = 0;
		unsigned short fieldlen = 0;

		if ( (fh = fopen( filename, "rb" )) != 0 ) {
			Log( MSG_ERROR, "Could not load STB '%s'", filename );
			return 1;
		}else{
			fseek( fh, 4, SEEK_SET );
			fread( &dataoffset, 4, 1, fh );
			fread( &rowcount, 4, 1, fh );
			fread( &fieldcount, 4, 1, fh );

			rowcount--;
			fieldcount--;
			
			fseek( fh, dataoffset, SEEK_SET );
			int* tmp = new int[rowcount*fieldcount];
			rows = new int*[rowcount];
			for(dword i = 0; i < rowcount; i++ ) rows[i] = &tmp[i*fieldcount];
			for(dword j = 0; j < rowcount*fieldcount; j++) {
				fread( &fieldlen, 2, 1, fh );
				fread( tmpfield, 1, fieldlen, fh );
				tmpfield[fieldlen]=0;
				tmp[j] = atoi( tmpfield ); 
			}
			fclose( fh );
		}

		return 0;
	}

	dword rowcount;
	dword fieldcount;
	int** rows;

private:
	void CleanUp(){
		if(rows != NULL){
			delete [] rows[0];
			delete [] rows;
		}
	}
};
