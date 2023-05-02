#pragma once
#include <map>

/**
 * \brief std::map template containing helpful functions
 * \tparam TKey Map key
 * \tparam TValue Map Value
 */
template <class TKey, class TValue>
class TrackerMap
{
public:
    /**
     * \brief Check if the map contains a key
     * \param key TKey to check
     * \return true if the map has the key
     */
    bool ContainsKey(const TKey &key)
    {
        return _map.find(key) != _map.end();
    }

    /**
     * \brief Check if the map contains a value
     * \param value TValue to check
     * \return true if the map has at least a value
     */
    bool ContainsValue(const TValue &value)
    {
        for(const auto& it: _map)
            if(it.second == value) return true;
        return false;
    }

    /**
     * \brief Convert the map to a string, for debug purposes
     */
    std::string ToString()
    {
        std::string str;

        for(auto it = _map.cbegin(); it != _map.cend(); it++)
            str += "Key : " + static_cast<Location>(it->first).ToString() + ", Value :" + static_cast<Location>(it->second).ToString() + "\n";
        
        return str;
    }

    /**
     * \brief Clear the map
     */
    void Clear()
    {
        _map.clear();
    }

    /**
     * \brief Insert an element in map
     */
    void Insert(TKey key, TValue value)
    {
        _map.insert({key, value});
    }

    /**
     * \brief Erase an element by key
     * \param key 
     */
    void Erase(TKey key)
    {
        auto it = _map.find(key);
        if(it != _map.end())
            _map.erase(it);
    }

    /**
    * \brief Swap value from oldKey to newKey, a default constructed value is created
    * by operator[] if the map does not contain newKey. Then erase the element referenced by oldKey
     * \param oldKey Element value to set in newKey element, and delete after operation
     * \param newKey Element receiving the oldKey value
     */
    void UpdateKey(TKey oldKey, TKey newKey)
    {
        auto it = _map.find(oldKey);
        if (it != _map.end()) {
            std::swap(_map[newKey], it->second);
            _map.erase(it);
        }
    }

    std::map< TKey, TValue > GetMap() { return _map; }
    std::map< TKey, TValue >& GetRefMap() { return _map; }
    
private:
    std::map< TKey, TValue > _map;
};
