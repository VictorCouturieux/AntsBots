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
    void erase(TKey key)
    {
        auto it = map.find(key);
        if(it != map.end())
            map.erase(it);
    }
    void updateKey(TKey oldKey, TKey newKey)
    {
        auto it = map.find(oldKey);
        if (it != map.end()) {
            // Swap value from oldKey to newKey, note that a default constructed value 
            // is created by operator[] if 'm' does not contain newKey.
            std::swap(map[newKey], it->second);
            // Erase old key-value from map
            map.erase(it);
        }
    }

    std::map< TKey, TValue > GetMap() { return map; }
    std::map< TKey, TValue >& GetRefMap() { return map; }

    template<typename T>
    void pop_front(std::vector<T>& vec)
    {
        vec.front() = std::move(vec.back());
        vec.pop_back();
    }
    
private:
    std::map< TKey, TValue > map;
};
