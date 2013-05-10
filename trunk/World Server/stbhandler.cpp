// Written by Brett19
#include "worldserver.h"

// -----------------------------------------------------------------------------------------
// STB reading shit
// -----------------------------------------------------------------------------------------
int STBStoreData( char* filename, CSTBData* data )
{
	char tmpfield[512];
	unsigned dataoffset = 0;
	unsigned short fieldlen = 0;

    FILE* fh = fopen( filename, "rb" );
	if (  fh== 0 ) {
		Log( MSG_ERROR, "Could not load STB '%s'", filename );
		return 1;
	}else{
		// Read the header
		fseek( fh, 4, SEEK_SET );
		fread( &dataoffset, 4, 1, fh );
		fread( &data->rowcount, 4, 1, fh );
		fread( &data->fieldcount, 4, 1, fh );
		data->rowcount--;
		data->fieldcount--;
		// Read the data
		fseek( fh, dataoffset, SEEK_SET );
		int* tmp = new int[data->rowcount*data->fieldcount];
		data->rows = new int*[data->rowcount];
		for( unsigned i = 0; i < data->rowcount; i++ ) data->rows[i] = &tmp[i*data->fieldcount];
		for( unsigned j = 0; j < data->rowcount*data->fieldcount; j++ ) {
			fread( &fieldlen, 2, 1, fh );
			fread( tmpfield, 1, fieldlen, fh );
			tmpfield[fieldlen]=0;
			tmp[j] = atoi( tmpfield ); 
		}
		fclose( fh );
	}

	return 0;
}

void STBFreeData( CSTBData* data )
{
	delete [] data->rows[0];
	delete [] data->rows;
}
