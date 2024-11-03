// This file contains all the custom macros for GDExtension's boilerplate code. 
// The goal is to minimize typos from copying these boilerplate code. 

#ifndef BOILERPLATE_MACROS_H
#define BOILERPLATE_MACROS_H

/*--------------------------------------------------*/

// Declare functions needed to export instance variables. Used in header files. 
#define _GDEXPORT /***********************************************/ \
protected:                                                          \
    void _get_property_list(List<PropertyInfo> *p_list) const;      \
    bool _get(const StringName &p_name, Variant &r_property) const; \
    bool _set(const StringName &p_name, const Variant &p_property);

/*--------------------------------------------------*/

// Start of _get_property_list(). 
#define _GDEXPORT_ADD_PREFIX \
void GDExample1New::_get_property_list(List<PropertyInfo> *p_list) const {

// Add a variable to the list of exported properties. Used in _get_property_list(). 
#define _GDEXPORT_ADD(PropertyInfo) \
p_list->push_back(PropertyInfo);

// End of _get_property_list(). 
#define _GDEXPORT_ADD_SUFFIX \
}

/*--------------------------------------------------*/

// Start of _get(). 
#define _GDEXPORT_GET_PREFIX \
bool GDExample1New::_get(const StringName &p_name, Variant &r_property) const {

// If the variable is exported, get its value and return true. Used in _get(). 
#define _GDEXPORT_GET(variable) /********/ \
if (p_name.nocasecmp_to(#variable) == 0) { \
    r_property = variable;                 \
    return true;                           \
}

// End of _get(). 
#define _GDEXPORT_GET_SUFFIX \
	return false;            \
}

/*--------------------------------------------------*/

// Start of _set(). 
#define _GDEXPORT_SET_PREFIX \
bool GDExample1New::_set(const StringName &p_name, const Variant &p_property) {

// If the variable is exported, set its value and return true. Used in _set(). 
#define _GDEXPORT_SET(variable) /********/ \
if (p_name.nocasecmp_to(#variable) == 0) { \
    variable = p_property;                 \
    return true;                    \
}

// End of _set(). 
#define _GDEXPORT_SET_SUFFIX \
	return false;            \
}

/*--------------------------------------------------*/

#endif