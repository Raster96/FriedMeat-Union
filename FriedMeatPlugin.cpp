// FriedMeat Plugin for Union SDK
// FriedMeat - converts raw meat to cooked when creature dies from fire
// Union SOURCE file

namespace GOTHIC_ENGINE {

    bool IsWorldReady() {
        if (!ogame) return false;
        if (ogame->inScriptStartup || ogame->inLoadSaveGame || ogame->inLevelChange) return false;
        return true;
    }

    bool IsFireEffect(const zSTRING& name) {
        const char* str = name.ToChar();
        if (!str) return false;
        
        char upper[256];
        strncpy(upper, str, 255);
        upper[255] = 0;
        _strupr(upper);
        
        return strstr(upper, "FIRE") != nullptr || 
               strstr(upper, "FIR_") != nullptr ||
               strstr(upper, "HUMANBURN") != nullptr ||
               strstr(upper, "BURN") != nullptr;
    }

    void ReplaceMeatWithFried(oCNpc* npc) {
        if (!IsWorldReady()) return;
        if (!npc || !FriedMeatCore::enabled) return;
        if (!parser) return;
        
        if (npc->IsHuman()) return;
        
        oCNpcInventory* inv = &npc->inventory2;
        if (!inv) return;
        
        for (int i = 0; i < FriedMeatCore::meatPairCount; i++) {
            zSTRING rawName = FriedMeatCore::meatPairs[i].rawName;
            rawName.Upper();
            
            int rawIndex = parser->GetIndex(rawName);
            if (rawIndex < 0) continue;
            
            oCItem* rawMeat = inv->IsIn(rawIndex, 1);
            if (!rawMeat) continue;
            
            int amount = rawMeat->amount;
            inv->Remove(rawMeat, amount);
            
            zSTRING cookedName = FriedMeatCore::meatPairs[i].cookedName;
            cookedName.Upper();
            int cookedIndex = parser->GetIndex(cookedName);
            
            if (cookedIndex >= 0) {
                npc->CreateItems(cookedIndex, amount);
            }
        }
    }

    HOOK Hook_oCNpc_OnDamage PATCH(&oCNpc::OnDamage, &oCNpc::OnDamage_Union);

    void oCNpc::OnDamage_Union(oCNpc::oSDamageDescriptor& desc) {
        if (IsWorldReady() && FriedMeatCore::enabled) {
            bool isFireDamage = false;
            
            if (desc.pVisualFX) {
                zSTRING fxName = desc.pVisualFX->visName_S;
                if (IsFireEffect(fxName)) {
                    isFireDamage = true;
                }
            }
            
            if (desc.pFXHit) {
                zSTRING fxName = desc.pFXHit->visName_S;
                if (IsFireEffect(fxName)) {
                    isFireDamage = true;
                }
            }
            
            if (isFireDamage) {
                ReplaceMeatWithFried(this);
            }
        }
        
        THISCALL(Hook_oCNpc_OnDamage)(desc);
    }

    void Game_Entry() {
    }
    
    void Game_Init() {
        FriedMeatCore::Init();
    }

    void Game_Exit() {
    }

    void Game_PreLoop() {
    }

    void Game_Loop() {
    }

    void Game_PostLoop() {
    }

    void Game_MenuLoop() {
    }

    TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

    void Game_SaveBegin() {
    }

    void Game_SaveEnd() {
    }

    void LoadBegin() {
    }

    void LoadEnd() {
    }

    void Game_LoadBegin_NewGame() {
        LoadBegin();
    }

    void Game_LoadEnd_NewGame() {
        LoadEnd();
    }

    void Game_LoadBegin_SaveGame() {
        LoadBegin();
    }

    void Game_LoadEnd_SaveGame() {
        LoadEnd();
    }

    void Game_LoadBegin_ChangeLevel() {
        LoadBegin();
    }

    void Game_LoadEnd_ChangeLevel() {
        LoadEnd();
    }

    void Game_LoadBegin_Trigger() {
    }
    
    void Game_LoadEnd_Trigger() {
    }
    
    void Game_Pause() {
    }
    
    void Game_Unpause() {
    }
    
    void Game_DefineExternals() {
    }

    void Game_ApplyOptions() {
        FriedMeatCore::ReadOptions();
    }

#define AppDefault True
    CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
        Enabled( AppDefault ) Game_Entry,
        Enabled( AppDefault ) Game_Init,
        Enabled( AppDefault ) Game_Exit,
        Enabled( AppDefault ) Game_PreLoop,
        Enabled( AppDefault ) Game_Loop,
        Enabled( AppDefault ) Game_PostLoop,
        Enabled( AppDefault ) Game_MenuLoop,
        Enabled( AppDefault ) Game_SaveBegin,
        Enabled( AppDefault ) Game_SaveEnd,
        Enabled( AppDefault ) Game_LoadBegin_NewGame,
        Enabled( AppDefault ) Game_LoadEnd_NewGame,
        Enabled( AppDefault ) Game_LoadBegin_SaveGame,
        Enabled( AppDefault ) Game_LoadEnd_SaveGame,
        Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
        Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
        Enabled( AppDefault ) Game_LoadBegin_Trigger,
        Enabled( AppDefault ) Game_LoadEnd_Trigger,
        Enabled( AppDefault ) Game_Pause,
        Enabled( AppDefault ) Game_Unpause,
        Enabled( AppDefault ) Game_DefineExternals,
        Enabled( AppDefault ) Game_ApplyOptions
    );
}
