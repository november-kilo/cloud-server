#include <Array.h>

/*
 * NAME:	map_remove()
 * DESCRIPTION:	create a copy of mapping with specified key removed
 * RETURNS:	new mapping without the key
 */
mapping map_remove(mapping m, mixed key)
{
    mixed *indices, *values;
    int i, sz;
    mapping result;
    
    if (!m) {
        return ([ ]);
    }
    
    indices = map_indices(m);
    values = map_values(m);
    sz = sizeof(indices);
    
    result = ([ ]);
    
    for (i = 0; i < sz; i++) {
        if (indices[i] != key) {
            result[indices[i]] = values[i];
        }
    }
    
    return result;
}

private int member_array(mixed value, mixed *keys_to_remove) {
    Array array;
    int i;
    mixed element;

    array = new Array(keys_to_remove);
    ({ i, element }) = array->find_first(value);
    return i;
}

/*
 * NAME:	map_remove_keys()
 * DESCRIPTION:	create a copy of mapping with multiple keys removed
 * RETURNS:	new mapping without the specified keys
 */
mapping map_remove_keys(mapping m, mixed *keys_to_remove)
{
    mixed *indices, *values;
    int i, sz;
    mapping result;
    
    if (!m) {
        return ([ ]);
    }
    
    indices = map_indices(m);
    values = map_values(m);
    sz = sizeof(indices);
    
    result = ([ ]);
    
    for (i = 0; i < sz; i++) {
        if (member_array(indices[i], keys_to_remove) == -1) {
            result[indices[i]] = values[i];
        }
    }
    
    return result;
}

/*
 * NAME:	map_copy()
 * DESCRIPTION:	create a shallow copy of a mapping
 * RETURNS:	new mapping with same key-value pairs
 */
mapping map_copy(mapping m)
{
    mixed *indices, *values;
    int i, sz;
    mapping result;
    
    if (!m) {
        return ([ ]);
    }
    
    indices = map_indices(m);
    values = map_values(m);
    sz = sizeof(indices);
    
    result = ([ ]);
    
    for (i = 0; i < sz; i++) {
        result[indices[i]] = values[i];
    }
    
    return result;
}

/*
 * NAME:	map_select()
 * DESCRIPTION:	create a mapping with only specified keys
 * RETURNS:	new mapping with only the selected keys
 */
mapping map_select(mapping m, mixed *keys_to_keep)
{
    int i, sz;
    mapping result;
    
    if (!m) {
        return ([ ]);
    }
    
    sz = sizeof(keys_to_keep);
    result = ([ ]);
    
    for (i = 0; i < sz; i++) {
        if (m[keys_to_keep[i]]) {
            result[keys_to_keep[i]] = m[keys_to_keep[i]];
        }
    }
    
    return result;
}
