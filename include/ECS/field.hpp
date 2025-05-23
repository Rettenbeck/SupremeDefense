#pragma once

#include <Util/basic.hpp>
#include <nlohmann/include.hpp>


namespace SupDef {

    template <typename T>
    struct IField {
        std::string name;
        virtual nlohmann::json getJson(const T* obj) const = 0;
        virtual void setJson(T* obj, const nlohmann::json& j) const = 0;
        virtual ~IField() = default;
    };

    template <typename T, typename FieldType>
    struct Field : public IField<T> {
        FieldType T::* memberPtr;
    
        Field(const std::string& name_, FieldType T::* ptr) : memberPtr(ptr) {
            this->name = name_;
        }
    
        nlohmann::json getJson(const T* obj) const override {
            assert(obj);
            return obj->*memberPtr;
        }
    
        void setJson(T* obj, const nlohmann::json& j) const override {
            assert(obj);
            obj->*memberPtr = j.get<FieldType>();
        }
    
        FieldType& getRef(T* obj) const {
            assert(obj);
            return obj->*memberPtr;
        }
    
        void setRef(T* obj, const FieldType& value) const {
            assert(obj);
            obj->*memberPtr = value;
        }
    };

    template <typename T, typename FieldType>
    std::unique_ptr<IField<T>> makeField(const std::string& name, FieldType T::* memberPtr) {
        return std::make_unique<Field<T, FieldType>>(name, memberPtr);
    }

    template <typename T, typename PointeeType>
    struct UniquePtrField : public IField<T> {
        std::unique_ptr<PointeeType> T::* memberPtr;
    
        UniquePtrField(const std::string& name_, std::unique_ptr<PointeeType> T::* ptr) : memberPtr(ptr) {
            this->name = name_;
        }
    
        nlohmann::json getJson(const T* obj) const override {
            assert(obj);
            const auto& ptr = obj->*memberPtr;
            if (!ptr) return nullptr;
    
            nlohmann::json j;
            // generic_to_json(j, ptr.get());
            ptr->to_json(j);
            return j;
        }
    
        void setJson(T* obj, const nlohmann::json& j) const override {
            assert(obj);
            if (j.is_null()) {
                obj->*memberPtr = nullptr;
            } else {
                obj->*memberPtr = std::make_unique<PointeeType>();
                auto& ptr = obj->*memberPtr;
                // generic_from_json(j, ptr.get());
                ptr->from_json(j);
            }
        }
    
        PointeeType* getRef(T* obj) const {
            assert(obj);
            return (obj->*memberPtr).get();
        }
    
        void setRef(T* obj, std::unique_ptr<PointeeType> value) const {
            assert(obj);
            obj->*memberPtr = std::move(value);
        }
    };

    template <typename T, typename PointeeType>
    std::unique_ptr<IField<T>> makeUniqueField(const std::string& name, std::unique_ptr<PointeeType> T::* memberPtr) {
        return std::make_unique<UniquePtrField<T, PointeeType>>(name, memberPtr);
    }

    template <typename T, typename ItemType>
    struct ListField : public IField<T> {
        std::vector<std::unique_ptr<ItemType>> T::* memberPtr;
    
        ListField(const std::string& name_, std::vector<std::unique_ptr<ItemType>> T::* ptr) : memberPtr(ptr) {
            this->name = name_;
        }
    
        nlohmann::json getJson(const T* obj) const override {
            assert(obj);
            const auto& vec = obj->*memberPtr;
            nlohmann::json j = nlohmann::json::array();
    
            for (const auto& item : vec) {
                if (item) {
                    nlohmann::json element;
                    // generic_to_json(element, item.get());
                    item->to_json(element);
                    j.push_back(element);
                } else {
                    j.push_back(nullptr);
                }
            }
    
            return j;
        }
    
        void setJson(T* obj, const nlohmann::json& j) const override {
            assert(obj);
            auto& vec = obj->*memberPtr;
            vec.clear();
    
            for (const auto& element : j) {
                if (element.is_null()) {
                    vec.push_back(nullptr);
                } else {
                    auto ptr = std::make_unique<ItemType>();
                    // generic_from_json(element, ptr.get());
                    ptr->from_json(element);
                    vec.push_back(std::move(ptr));
                }
            }
        }
    
        std::vector<std::unique_ptr<ItemType>>& getRef(T* obj) const {
            assert(obj);
            return obj->*memberPtr;
        }
    
        void setRef(T* obj, std::vector<std::unique_ptr<ItemType>> value) const {
            assert(obj);
            obj->*memberPtr = std::move(value);
        }
    };

    template <typename T, typename ItemType>
    std::unique_ptr<IField<T>> makeListField(const std::string& name, std::vector<std::unique_ptr<ItemType>> T::* memberPtr) {
        return std::make_unique<ListField<T, ItemType>>(name, memberPtr);
    }

    template <typename T, typename K, typename V>
    struct MapField : public IField<T> {
        std::unordered_map<K, std::unique_ptr<V>> T::* memberPtr;
    
        MapField(const std::string& name_, std::unordered_map<K, std::unique_ptr<V>> T::* ptr) : memberPtr(ptr) {
            this->name = name_;
        }
    
        nlohmann::json getJson(const T* obj) const override {
            assert(obj);
            const auto& map = obj->*memberPtr;
            nlohmann::json j = nlohmann::json::object();
    
            for (const auto& [key, value] : map) {
                std::stringstream ss; ss << key;
                if (value) {
                    nlohmann::json valJson;
                    generic_to_json(valJson, value.get());
                    j[ss.str()] = valJson;
                } else {
                    j[ss.str()] = nullptr;
                }
            }
    
            return j;
        }
    
        void setJson(T* obj, const nlohmann::json& j) const override {
            assert(obj);
            auto& map = obj->*memberPtr;
            map.clear();
    
            for (auto it = j.begin(); it != j.end(); ++it) {
                const std::string& keyStr = it.key();
                const auto& valueJson = it.value();
    
                K key{};
                std::istringstream(keyStr) >> key;
    
                if (valueJson.is_null()) {
                    map[key] = nullptr;
                } else {
                    auto ptr = std::make_unique<V>();
                    generic_from_json(valueJson, ptr.get());
                    map[key] = std::move(ptr);
                }
            }
        }
    
        std::unordered_map<K, std::unique_ptr<V>>& getRef(T* obj) const {
            assert(obj);
            return obj->*memberPtr;
        }
    
        void setRef(T* obj, std::unordered_map<K, std::unique_ptr<V>> value) const {
            assert(obj);
            obj->*memberPtr = std::move(value);
        }
    };

    template <typename T, typename K, typename V>
    std::unique_ptr<IField<T>> makeMapField(const std::string& name, std::unordered_map<K, std::unique_ptr<V>> T::* memberPtr) {
        return std::make_unique<MapField<T, K, V>>(name, memberPtr);
    }

    template <typename T>
    void generic_to_json(nlohmann::json& j, const T* object) {
        for (const auto& field : T::getFieldMap()) {
            j[field->name] = field->getJson(object);
        }
    }

    template <typename T>
    void generic_from_json(const nlohmann::json& j, T* object) {
        for (const auto& field : T::getFieldMap()) {
            if (j.contains(field->name)) {
                field->setJson(object, j[field->name]);
            }
        }
    }

    template <typename T, typename FieldType>
    FieldType& getFieldRef(const std::unique_ptr<IField<T>>& field, T* obj) {
        return static_cast<Field<T, FieldType>*>(field.get())->getRef(obj);
    }
    
    template <typename T, typename FieldType>
    void setFieldRef(const std::unique_ptr<IField<T>>& field, T* obj, const FieldType& value) {
        static_cast<Field<T, FieldType>*>(field.get())->setRef(obj, value);
    }

    #define REFLECT_BEGIN \
    static std::vector<std::unique_ptr<IField<ThisType>>> getFieldMap() { \
        std::vector<std::unique_ptr<IField<ThisType>>> fields;
    
    #define REFLECT_FIELD(name) \
        fields.push_back(std::make_unique<Field<ThisType, decltype(ThisType::name)>>(#name, &ThisType::name));

    #define REFLECT_UNIQUE(name, pointeeType) \
        fields.push_back(makeUniqueField<ThisType, pointeeType>(#name, &ThisType::name));

    #define REFLECT_LIST_UNIQUE(name, itemType) \
        fields.push_back(makeListField<ThisType, itemType>(#name, &ThisType::name));

    #define REFLECT_MAP_UNIQUE(name, keyType, valueType) \
        fields.push_back(makeMapField<ThisType, keyType, valueType>(#name, &ThisType::name));

    #define REFLECT_END \
        return fields; \
    }



    #define DEFINE_COMPONENT_BEGIN(CLASS_NAME, COMPONENT_NAME)                 \
    class CLASS_NAME : public Component {                                      \
    public:                                                                    \
        using ThisType = CLASS_NAME;                                           \
        std::string getTypeName() const override {                             \
            return COMPONENT_NAME;                                             \
        }                                                                      \
        void addToRegistry() {                                                 \
            ComponentRegistry::registerComponent(getTypeName(), []()           \
                { return std::make_unique<ThisType>(); });                     \
        }                                                                      \
        CLASS_NAME() { addToRegistry(); }                                      \
        void to_json(json& j) const override {                                 \
            generic_to_json(j, this);                                          \
        }                                                                      \
        void from_json(const json& j) override {                               \
            generic_from_json(j, this);                                        \
        }

    
    #define DEFINE_EMPTY_COMPONENT(CLASS_NAME, COMPONENT_NAME) \
    DEFINE_COMPONENT_BEGIN(CLASS_NAME, COMPONENT_NAME) \
        REFLECT_BEGIN \
        REFLECT_END \
    DEFINE_COMPONENT_END
    

    #define DEFINE_COMPONENT_NOJSON_BEGIN(CLASS_NAME, COMPONENT_NAME)          \
    class CLASS_NAME : public Component {                                      \
    public:                                                                    \
        using ThisType = CLASS_NAME;                                           \
        std::string getTypeName() const override {                             \
            return COMPONENT_NAME;                                             \
        }                                                                      \
        void addToRegistry() {                                                 \
            ComponentRegistry::registerComponent(getTypeName(), []()           \
                { return std::make_unique<ThisType>(); });                     \
        }                                                                      \
        CLASS_NAME() { addToRegistry(); }


    #define DEFINE_SCLASS_BEGIN(CLASS_NAME)                                    \
    class CLASS_NAME {                                                         \
    public:                                                                    \
        using ThisType = CLASS_NAME;                                           \
        CLASS_NAME() {}                                                        \
        void to_json(json& j) const {                                          \
            generic_to_json(j, this);                                          \
        }                                                                      \
        void from_json(const json& j) {                                        \
            generic_from_json(j, this);                                        \
        }


    #define DEFINE_SCLASS_NOJSON_BEGIN(CLASS_NAME)                             \
    class CLASS_NAME {                                                         \
    public:                                                                    \
        using ThisType = CLASS_NAME;                                           \
        CLASS_NAME() { addToRegistry(); }
    
    

    #define DEFINE_COMPONENT_END \
    };

    #define DEFINE_SCLASS_END \
    }; \

    
    #define DEFINE_UNIQUE(CLASS, UNIQUE) \
    using UNIQUE = std::unique_ptr<CLASS>;

    #define DEFINE_LIST(CLASS, LIST) \
    using LIST = std::vector<CLASS>;

    #define DEFINE_UNIQUE_AND_LIST(CLASS, UNIQUE, LIST) \
    DEFINE_UNIQUE(CLASS, UNIQUE) \
    DEFINE_LIST(UNIQUE, LIST)

    #define BOOL_TO_INT(BOOL) \
    ((BOOL) ? 1 : 0)

    #define STR_TO_BOOL(STR) \
    ((STR == '1') ? true : false)

}
