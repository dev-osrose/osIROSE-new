// Props to ExJam for this code :D

class CStrStb
{
	public:
	CStrStb( )
	    : _Data( 0 ) {}
	CStrStb( char* filename )
	    : _Data( 0 ) { LoadSTB( filename ); }
	~CStrStb( ) { CleanUp( ); }

	bool LoadSTB( char* path )
	{
		CleanUp( );

		CRoseFile* fh = new CRoseFile( path, FM_READ | FM_BINARY );
		if ( !fh->IsOpen( ) )
		{
			delete fh;
			return false;
		} // if is open

		fh->Seek( 4, SEEK_CUR );
		uint32_t offset = fh->Get< uint32_t >( );

		_Rows    = fh->Get< uint32_t >( );
		_Columns = fh->Get< uint32_t >( );

		_Rows--;
		_Columns--;

		fh->SetAbsPos( offset );
		_Data = new char**[ _Rows ];
		for ( uint32_t i = 0; i < _Rows; i++ )
			_Data[ i ] = new char*[ _Columns ];

		for ( uint32_t i = 0; i < _Rows; i++ )
		{
			for ( uint32_t j = 0; j < _Columns; j++ )
			{
				uint32_t len = fh->Get< uint16_t >( );
				if ( len == 0 )
				{
					_Data[ i ][ j ] = NULL;
					continue;
				} // if len
				_Data[ i ][ j ] = new char[ len + 1 ];
				fh->Read( _Data[ i ][ j ], len, 1 );
				_Data[ i ][ j ][ len ] = 0;
			} // for j
		}     // for i

		fh->Close( );
		delete fh;
		return true;
	} // bool

	char* Data( uint32_t Row, uint32_t Column )
	{
		if ( Row > _Rows )
			return NULL;
		if ( Column > _Columns )
			return NULL;

		return _Data[ Row ][ Column ];
	}

	uint32_t Rows( )
	{
		return _Rows;
	}
	uint32_t Columns( )
	{
		return _Columns;
	}

	private:
	void CleanUp( )
	{
		if ( _Data != NULL )
		{
			for ( uint32_t i = 0; i < _Rows; i++ )
			{
				for ( uint32_t j = 0; j < _Columns; j++ )
				{
					delete[] _Data[ i ][ j ];
				}
				delete[] _Data[ i ];
			}
			delete[] _Data;
		}
		_Rows    = 0;
		_Columns = 0;
	}

	private:
	uint32_t _Rows;
	uint32_t _Columns;

	char*** _Data;
};
