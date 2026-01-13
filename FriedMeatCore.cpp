// Supported with union (c) 2018-2021 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
    
    bool FriedMeatCore::enabled = true;
    MeatPair FriedMeatCore::meatPairs[128] = {};
    int FriedMeatCore::meatPairCount = 0;
    nlohmann::json FriedMeatCore::jsonFile;

    void FriedMeatCore::Init() {
        ReadOptions();
        LoadMeatConfig();
    }

    void FriedMeatCore::ReadOptions() {
        if (!zoptions) return;
        enabled = zoptions->ReadBool("FRIED_MEAT", "Enabled", true);
    }

    void FriedMeatCore::LoadMeatConfig() {
        meatPairCount = 0;
        
        zoptions->ChangeDir(DIR_SYSTEM);
        zFILE_VDFS* file = new zFILE_VDFS("FriedMeat.json");
        
        if (!file->Exists()) {
            delete file;
            return;
        }
        
        file->Open(false);
        
        zSTRING line, buffer;
        do {
            file->Read(line);
            buffer += line;
        } while (!file->Eof());
        
        file->Close();
        delete file;
        
        try {
            jsonFile = nlohmann::json::parse(buffer.ToChar());
            
            for (auto& category : jsonFile.items()) {
                if (!category.value().is_array()) continue;
                
                for (auto& pair : category.value()) {
                    if (meatPairCount >= 128) break;
                    
                    if (!pair["raw"].is_string() || !pair["cooked"].is_string()) continue;
                    
                    std::string rawStr = pair["raw"].get<std::string>();
                    std::string cookedStr = pair["cooked"].get<std::string>();
                    
                    strncpy(meatPairs[meatPairCount].rawName, rawStr.c_str(), 63);
                    meatPairs[meatPairCount].rawName[63] = 0;
                    strncpy(meatPairs[meatPairCount].cookedName, cookedStr.c_str(), 63);
                    meatPairs[meatPairCount].cookedName[63] = 0;
                    
                    meatPairCount++;
                }
                
                if (meatPairCount >= 128) break;
            }
        }
        catch (...) {
        }
    }
}
