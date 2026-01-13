// Supported with union (c) 2018-2021 Union team
// FriedMeat - converts raw meat to cooked when creature dies from fire

#include "nlohmann/json.hpp"

namespace GOTHIC_ENGINE {

    struct MeatPair {
        char rawName[64];
        char cookedName[64];
    };

    class FriedMeatCore {
    public:
        static bool enabled;
        static MeatPair meatPairs[128];
        static int meatPairCount;
        static nlohmann::json jsonFile;
        
        static void Init();
        static void ReadOptions();
        static void LoadMeatConfig();
    };
}
