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
    bool containsKey(const TKey &key)
    {
        return map.find(key) != map.end();
    }

    /**
     * \brief Check if the map contains a value
     * \param value TValue to check
     * \return true if the map has at least a value
     */
    bool containsValue(const TValue &value)
    {
        for(const auto& it: map)
            if(it.second == value) return true;
        return false;
    }

    /**
     * \brief Convert the map to a string, for debug purposes
     */
    std::string ToString()
    {
        std::string str;

        for(auto it = map.cbegin(); it != map.cend(); it++)
            str += "Key : " + static_cast<Location>(it->first).ToString() + ", Value :" + static_cast<Location>(it->second).ToString() + "\n";
        
        return str;
    }

    /**
     * \brief Clear the map
     */
    void clear()
    {
        map.clear();
    }

    /**
     * \brief Insert an element in map
     */
    void insert(TKey key, TValue value)
    {
        map.insert({key, value});
    }

    /**
     * \brief Erase an element by key
     * \param key 
     */
    void erase(TKey key)
    {
        auto it = map.find(key);
        if(it != map.end())
            map.erase(it);
    }

    /**
    * \brief Swap value from oldKey to newKey, a default constructed value is created
    * by operator[] if the map does not contain newKey. Then erase the element referenced by oldKey
     * \param oldKey Element value to set in newKey element, and delete after operation
     * \param newKey Element receiving the oldKey value
     */
    void updateKey(TKey oldKey, TKey newKey)
    {
        auto it = map.find(oldKey);
        if (it != map.end()) {
            std::swap(map[newKey], it->second);
            map.erase(it);
        }
    }

    std::map< TKey, TValue > GetMap() { return map; }
    std::map< TKey, TValue >& GetRefMap() { return map; }
    
private:
    std::map< TKey, TValue > map;
};
