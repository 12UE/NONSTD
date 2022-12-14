#pragma once
namespace nonstd {
    template<typename Key, typename Value>
    class Map {
    public:
        Map() {
            // 使用 RtlInitializeGenericTable 初始化内核表
            RtlInitializeGenericTable(
                &table_,
                (PRTL_GENERIC_COMPARE_ROUTINE)&Map::compareKeys,  // 比较键值的函数
                (PRTL_GENERIC_ALLOCATE_ROUTINE)&Map::getKeyHash,  // 获取键值的哈希值的函数
                nullptr,  // 不使用自定义的分配器
                (PVOID)(sizeof(Key) + sizeof(Value))
            );
        }

        static int __stdcall compareKeys(const void* a, const void* b) {
            // 比较键值的函数
            return memcmp(a, b, sizeof(Key));
        }

        static int getKeyHash(const void* a) {
            // 获取键值的哈希值的函数
            return *static_cast<const Key*>(a);
        }
        void insert(const Key& key, const Value& value) {
            // 使用 RtlInsertElementGenericTable 插入键值对
            RtlInsertElementGenericTable(
                &table_,
                (void*)&key,  // 键值的地址
                sizeof(Key) + sizeof(Value),  // 键值的大小
                nullptr  // 是否插入成功的标志
            );
        }

        Value& operator[](const Key& key) {
            // 使用 RtlLookupElementGenericTable 查找键值
            void* element = RtlLookupElementGenericTable(
                &table_,
                (void*)&key  // 键值的地址
            );
            if (!element) {
                // 如果没有找到键值，则插入新的键值
                insert(key, Value{});
                element = RtlLookupElementGenericTable(&table_, (void*)&key);
            }
            // 返回键值的值的地址
            return *reinterpret_cast<Value*>(reinterpret_cast<char*>(element) + sizeof(Key));
        }

    private:
        RTL_GENERIC_TABLE table_;  // 内核表
    };
}
