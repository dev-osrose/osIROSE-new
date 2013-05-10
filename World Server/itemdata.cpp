// Added By Raven0123

#include "datatypes.h"

unsigned CItem::GetVisInfo( )
{
	if ( count == 0 ) Clear( );
	return ( ( refine >> 4 ) << 20 ) | ( socketed << 19 ) | ( stats << 10 ) | itemnum;
}

unsigned short CItem::GetPakHeader( )
{
	if ( count == 0 ) Clear( );
	return ( ( itemnum << 5 ) | ( itemtype ) );
}

unsigned CItem::GetPakData( )
{
	unsigned retval = 0;
	if ( count == 0 ) Clear( );
	if ( itemtype >= 10 && itemtype <= 13 || itemtype == 0x1f ) {
		retval = count;
	} else {
		retval |= (refine>>4) << 28;
		retval |= (appraised&1) << 27;
		retval |= (socketed&1) << 26;
		retval |= (lifespan*10) << 16;
		retval |= durability << 9;
		retval |= stats;
	}
	return retval;
}

void CItem::Clear( )
{
	itemnum = 0;
	itemtype = 0;
	count = 0;
	refine = 0;
	appraised = 0;
	socketed = 0;
	lifespan = 0;
	durability = 0;
	stats = 0;
}
