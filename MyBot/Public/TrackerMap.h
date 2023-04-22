#pragma once
#include <map>

// Template map Class with usefull functions
template <class TKey, class TValue>
class TrackerMap
{
public:
    bool containsKey(const TKey &key)
    {
        return map.find(key) != map.end();
    }
    bool containsValue(const TValue &value)
    {
        for(const auto& it: map)
            if(it.second == value) return true;
        return false;
    }

    // Only to track locations
    std::string ToString()
    {
        std::string str;

        for(auto it = map.cbegin(); it != map.cend(); it++)
            str += "Key : " + static_cast<Location>(it->first).ToString() + ", Value :" + static_cast<Location>(it->second).ToString() + "\n";
        
        return str;
    }

    void clear()
    {
        map.clear();
    }
    void insert(TKey key, TValue value)
    {
        map.insert({key, value});
    }
private:
    std::map< TKey, TValue > map;
};
