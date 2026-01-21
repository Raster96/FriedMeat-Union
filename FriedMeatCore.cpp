// Supported with union (c) 2018-2021 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
    
    bool FriedMeatCore::enabled = true;
    bool FriedMeatCore::checkSpells = true;
    MeatPair FriedMeatCore::meatPairs[128] = {};
    int FriedMeatCore::meatPairCount = 0;
    int FriedMeatCore::fireSpellIds[32] = {};
    int FriedMeatCore::fireSpellCount = 0;
    nlohmann::json FriedMeatCore::jsonFile;

    void FriedMeatCore::Init() {
        ReadOptions();
        LoadMeatConfig();
        LoadFireSpells();
    }

    void FriedMeatCore::ReadOptions() {
        if (!zoptions) return;
        enabled = zoptions->ReadBool("FRIED_MEAT", "Enabled", true);
        checkSpells = zoptions->ReadBool("FRIED_MEAT", "CheckSpells", true);
    }

    bool FriedMeatCore::IsFireSpell(unsigned long spellID) {
        if (spellID == 0) return false;
        
        for (int i = 0; i < fireSpellCount; i++) {
            if (fireSpellIds[i] == (int)spellID) return true;
        }
        return false;
    }

    void FriedMeatCore::LoadFireSpells() {
        fireSpellCount = 0;
        
        if (jsonFile.is_null()) return;
        
        try {
            if (!jsonFile.contains("fireSpells")) return;
            
#if ENGINE == Engine_G1 || ENGINE == Engine_G1A
            const char* spellKey = "gothic1";
#else
            const char* spellKey = "gothic2";
#endif
            
            if (!jsonFile["fireSpells"].contains(spellKey)) return;
            
            auto& spells = jsonFile["fireSpells"][spellKey];
            if (!spells.is_array()) return;
            
            for (auto& spell : spells) {
                if (fireSpellCount >= 32) break;
                
                if (!spell.contains("id") || !spell["id"].is_number_integer()) continue;
                
                fireSpellIds[fireSpellCount] = spell["id"].get<int>();
                fireSpellCount++;
            }
        }
        catch (...) {
        }
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
            
            if (!jsonFile.contains("meatPairs")) return;
            
            for (auto& category : jsonFile["meatPairs"].items()) {
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
