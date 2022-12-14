#pragma once
namespace nonstd {
    template<typename Key, typename Value>
    class Map {
    public:
        Map() {
            // ʹ�� RtlInitializeGenericTable ��ʼ���ں˱�
            RtlInitializeGenericTable(
                &table_,
                (PRTL_GENERIC_COMPARE_ROUTINE)&Map::compareKeys,  // �Ƚϼ�ֵ�ĺ���
                (PRTL_GENERIC_ALLOCATE_ROUTINE)&Map::getKeyHash,  // ��ȡ��ֵ�Ĺ�ϣֵ�ĺ���
                nullptr,  // ��ʹ���Զ���ķ�����
                (PVOID)(sizeof(Key) + sizeof(Value))
            );
        }

        static int __stdcall compareKeys(const void* a, const void* b) {
            // �Ƚϼ�ֵ�ĺ���
            return memcmp(a, b, sizeof(Key));
        }

        static int getKeyHash(const void* a) {
            // ��ȡ��ֵ�Ĺ�ϣֵ�ĺ���
            return *static_cast<const Key*>(a);
        }
        void insert(const Key& key, const Value& value) {
            // ʹ�� RtlInsertElementGenericTable �����ֵ��
            RtlInsertElementGenericTable(
                &table_,
                (void*)&key,  // ��ֵ�ĵ�ַ
                sizeof(Key) + sizeof(Value),  // ��ֵ�Ĵ�С
                nullptr  // �Ƿ����ɹ��ı�־
            );
        }

        Value& operator[](const Key& key) {
            // ʹ�� RtlLookupElementGenericTable ���Ҽ�ֵ
            void* element = RtlLookupElementGenericTable(
                &table_,
                (void*)&key  // ��ֵ�ĵ�ַ
            );
            if (!element) {
                // ���û���ҵ���ֵ��������µļ�ֵ
                insert(key, Value{});
                element = RtlLookupElementGenericTable(&table_, (void*)&key);
            }
            // ���ؼ�ֵ��ֵ�ĵ�ַ
            return *reinterpret_cast<Value*>(reinterpret_cast<char*>(element) + sizeof(Key));
        }

    private:
        RTL_GENERIC_TABLE table_;  // �ں˱�
    };
}
