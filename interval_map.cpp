#include <map>
template<typename K, typename V>

class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
    void assign( K const& keyBegin, K const& keyEnd, V const& val ) {

        if (!(keyBegin < keyEnd)) return;

        std::pair<K,V> beginExtra;
        std::pair<K,V> endExtra;
        bool beginHasExtra = false;
        bool endHasExtra = false;

        typename std::map<K,V>::iterator itBegin;
        itBegin = m_map.lower_bound(keyBegin);
        if ( itBegin!=m_map.end() && keyBegin < itBegin->first ) {
            if (itBegin != m_map.begin()) {
                beginHasExtra = true;
                --itBegin;
                beginExtra = std::make_pair(itBegin->first, itBegin->second);
            }

        }

        typename std::map<K,V>::iterator itEnd;
        itEnd = m_map.lower_bound(keyEnd);
        if ( itEnd!=m_map.end() && keyEnd < itEnd->first ) {
            endHasExtra = true;
            typename std::map<K,V>::iterator extraIt = itEnd;
            --extraIt;
            endExtra = std::make_pair(keyEnd, extraIt->second);

        }


        bool insertMid = true;
        if (beginHasExtra) {
            if (beginExtra.second == val)
                insertMid = false;
        } else {
            if (itBegin != m_map.begin()) {
                typename std::map<K,V>::iterator beforeMid = itBegin;
                --beforeMid;
                if (beforeMid->second == val)
                    insertMid = false;
            }
        }


        if (endHasExtra) {
            if ( (insertMid && endExtra.second == val) || (!insertMid && endExtra.second == beginExtra.second) )
                endHasExtra = false;
        } else {
            if ( (insertMid && itEnd!=m_map.end() && itEnd->second == val) || (!insertMid && itEnd!=m_map.end() && itEnd->second == beginExtra.second) )
                itEnd = m_map.erase(itEnd);
        }

        itBegin = m_map.erase(itBegin, itEnd);
        if (beginHasExtra)
            itBegin = m_map.insert(itBegin, beginExtra);
        if (insertMid)
            itBegin = m_map.insert(itBegin, std::make_pair(keyBegin, val));
        if (endHasExtra)
            m_map.insert(itBegin, endExtra);
    }

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.