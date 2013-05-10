#ifndef __ROSE_SERVER__CRYPTION_ROUTINES__HEADER_FILE__
#define __ROSE_SERVER__CRYPTION_ROUTINES__HEADER_FILE__
//#define USE124
//-------------------------------------------------------------------------
// This structure contains generated tables, which are needed to
// encrypt and decrypt packets.
//-------------------------------------------------------------------------
struct CCryptTable
{
	unsigned**			Tables;							// Main encryption table
	unsigned short*		AddTable;						// Less important encryption table

	unsigned			EncryptionStartValue;			// Start value for sending clients	
};

//-------------------------------------------------------------------------
// This structure contains informations about current encrypt-values.
// Startvalues should be "EncryptionStartValue" from the CCryptTable for
// CurEncryptionValue and zero for CurAddValue.
// * This values changes every incoming encryption.
//-------------------------------------------------------------------------
struct CCryptStatus
{
	unsigned			CurEncryptionValue;				// Current encryption value
	unsigned			CurAddValue;					// Current additional value
};

//-------------------------------------------------------------------------
// Functions, used to generate and delete CryptTables
//-------------------------------------------------------------------------
void GenerateCryptTables			( CCryptTable* &CryptTables, unsigned ModValue );
void GenerateLoginServerCryptTables	( CCryptTable* &CryptTables );
void FreeCryptTables				( CCryptTable* &CryptTables );

//-------------------------------------------------------------------------
// Functions, used to decrypt/encrypt a buffer - serverside
//-------------------------------------------------------------------------
int  DecryptBufferHeader			( CCryptStatus *ri, CCryptTable *CryptTables, unsigned char *Buffer );
bool DecryptBufferData				( CCryptTable *CryptTables, unsigned char *Buffer );
void EncryptBuffer					( CCryptTable *CryptTables, unsigned char *Buffer );
bool CryptISCPak( unsigned char* pak ); // Raven0123
//-------------------------------------------------------------------------
#ifdef USE124
#define TABLE_SIZE	(256+8)

int buildChecksum(char* csum, char* path=0);
void buildCryptTable(char* crypttable, char* checksum, int checksumlen);
void cryptPacket(char *packet, char *crypttable);
#endif
#endif
