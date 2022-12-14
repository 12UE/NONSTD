#pragma once
namespace nonstd {
    class Random {
    private:
        Random() = default;
        ULONG seed = -1;
    public:
        static Random& GetInstance() {
            static Random inst;
            return inst;
        }
        
        inline ULONG random() {
            return RtlRandomEx(&seed);
        }
        inline void srand(ULONG _seed) {
            seed = _seed;
        }
    };
    inline ULONG rand() {
        return Random::GetInstance().random();
    }
    inline void srand(ULONG _seed) {
        Random::GetInstance().srand(_seed);
    }
}