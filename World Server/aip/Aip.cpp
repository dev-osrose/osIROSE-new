// Props to ExJam for this code :D
#include "../WorldServer.h"

void CWorldServer::ReadAIP(strings path, dword index){	
	CRoseFile* fh = new CRoseFile(path, FM_READ | FM_BINARY);
	if(fh->IsOpen()) { // goto done;

//	Log(MSG_LOAD, "Loading %s                              ", path);

	dword BlockCount = fh->Get<dword>();
    dword unknown1 = fh->Get<dword>();
	dword unknown2 = fh->Get<dword>();
	dword titlestringlength = fh->Get<dword>();
	strings title=new char[titlestringlength+1];
	strings title2=new char[32+1];
	fh->Read(title, titlestringlength, 1);
	for(dword i = 0; i < BlockCount; i++)
    {
        titlestringlength = 32;
        fh->Read(title2, titlestringlength, 1);//on spawn-idle-attack-attacked-killed target-dead
		dword RecordCount = fh->Get<dword>();
		
    	for(dword j = 0; j < RecordCount; j++)
        {
            CAip* script = new CAip();
			fh->Read(title2, titlestringlength, 1);// 00 ==============================
			script->ConditionCount = fh->Get<dword>();
			if(script->ConditionCount > 0)
            {
				script->Conditions = new CAip::SAipDatum*[script->ConditionCount];
				for(dword k = 0; k < script->ConditionCount; k++)
                {
					CAip::SAipDatum* data = new CAip::SAipDatum();
					data->size = fh->Get<dword>();
					data->opcode = fh->Get<dword>() - 0x04000001;
					data->data = new byte[data->size - 8];
					fh->Read(data->data, data->size - 8, 1);
					script->Conditions[k] = data;
				}
			}
            else
            {
				script->Conditions = NULL;
			}
			script->ActionCount = fh->Get<dword>();

			if(script->ActionCount > 0)
            {
				script->Actions = new CAip::SAipDatum*[script->ActionCount];
				for(dword k = 0; k < script->ActionCount; k++)
                {
					CAip::SAipDatum* data = new CAip::SAipDatum();
					data->size = fh->Get<dword>();
					data->opcode = fh->Get<dword>() - 0x0B000001;
					data->data = new byte[data->size - 8];
					fh->Read(data->data, data->size - 8, 1);
					script->Actions[k] = data;
				}
			}
            else
            {
				script->Actions = NULL;
			}
			script->AipID=((index*0x10000)+(i*0x100)+j);
			AipList.push_back( script );
		}
	}
}else
     Log( MSG_ERROR, "AIP File: '%s'", path );
     
    fh->Close();
	delete fh;
}

void CWorldServer::LoadAipData(){
	CStrStb* stbAip = new CStrStb("3DDATA\\STB\\FILE_AI.STB");
	
	for(dword i = 1; i < stbAip->Rows(); i++){
		if(stbAip->Data(i, 0)){
			GServer->ReadAIP(stbAip->Data(i, 0),i);
		}
	}
//Log(MSG_INFO, "%i aip entrys",AipList.size());
	Log(MSG_INFO, "Finished loading aip data                              ");

	aiCondFunc[0] = &F_AI_COND_000;
	aiCondFunc[1] = &F_AI_COND_001;
	aiCondFunc[2] = &F_AI_COND_002;
	aiCondFunc[3] = &F_AI_COND_003;
	aiCondFunc[4] = &F_AI_COND_004;
	aiCondFunc[5] = &F_AI_COND_005;
	aiCondFunc[6] = &F_AI_COND_006;
	aiCondFunc[7] = &F_AI_COND_007;
	aiCondFunc[8] = &F_AI_COND_008;
	aiCondFunc[9] = &F_AI_COND_009;
	aiCondFunc[10] = &F_AI_COND_010;
	aiCondFunc[11] = &F_AI_COND_011;
	aiCondFunc[12] = &F_AI_COND_012;
	aiCondFunc[13] = &F_AI_COND_013;
	aiCondFunc[14] = &F_AI_COND_014;
	aiCondFunc[15] = &F_AI_COND_015;
	aiCondFunc[16] = &F_AI_COND_016;
	aiCondFunc[17] = &F_AI_COND_017;
	aiCondFunc[18] = &F_AI_COND_018;
	aiCondFunc[19] = &F_AI_COND_019;
	aiCondFunc[20] = &F_AI_COND_020;
	aiCondFunc[21] = &F_AI_COND_021;
	aiCondFunc[22] = &F_AI_COND_022;
	aiCondFunc[23] = &F_AI_COND_023;
	aiCondFunc[24] = &F_AI_COND_024;
	aiCondFunc[25] = &F_AI_COND_025;
	aiCondFunc[26] = &F_AI_COND_026;
	aiCondFunc[27] = &F_AI_COND_027;
	aiCondFunc[28] = &F_AI_COND_028;
	aiCondFunc[29] = &F_AI_COND_029;
	aiCondFunc[30] = &F_AI_COND_030;

	aiActFunc[0] = &F_AI_ACT_000;
	aiActFunc[1] = &F_AI_ACT_001;
	aiActFunc[2] = &F_AI_ACT_002;
	aiActFunc[3] = &F_AI_ACT_003;
	aiActFunc[4] = &F_AI_ACT_004;
	aiActFunc[5] = &F_AI_ACT_005;
	aiActFunc[6] = &F_AI_ACT_006;
	aiActFunc[7] = &F_AI_ACT_007;
	aiActFunc[8] = &F_AI_ACT_008;
	aiActFunc[9] = &F_AI_ACT_009;
	aiActFunc[10] = &F_AI_ACT_010;
	aiActFunc[11] = &F_AI_ACT_011;
	aiActFunc[12] = &F_AI_ACT_012;
	aiActFunc[13] = &F_AI_ACT_013;
	aiActFunc[14] = &F_AI_ACT_014;
	aiActFunc[15] = &F_AI_ACT_015;
	aiActFunc[16] = &F_AI_ACT_016;
	aiActFunc[17] = &F_AI_ACT_017;
	aiActFunc[18] = &F_AI_ACT_018;
	aiActFunc[19] = &F_AI_ACT_019;
	aiActFunc[20] = &F_AI_ACT_020;
	aiActFunc[21] = &F_AI_ACT_021;
	aiActFunc[22] = &F_AI_ACT_022;
	aiActFunc[23] = &F_AI_ACT_023;
	aiActFunc[24] = &F_AI_ACT_024;
	aiActFunc[25] = &F_AI_ACT_025;
	aiActFunc[26] = &F_AI_ACT_026;
	aiActFunc[27] = &F_AI_ACT_027;
	aiActFunc[28] = &F_AI_ACT_028;
	aiActFunc[29] = &F_AI_ACT_029;
	aiActFunc[30] = &F_AI_ACT_030;
	aiActFunc[31] = &F_AI_ACT_031;
	aiActFunc[32] = &F_AI_ACT_032;
	aiActFunc[33] = &F_AI_ACT_033;
	aiActFunc[34] = &F_AI_ACT_034;
	aiActFunc[35] = &F_AI_ACT_035;
	aiActFunc[36] = &F_AI_ACT_036;
	aiActFunc[37] = &F_AI_ACT_037;

	delete stbAip;
}

