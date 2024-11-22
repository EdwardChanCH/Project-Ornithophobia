/**
 * @file boilerplate_macros.h
 * @author Chun Ho Chan
 * @brief This header file contains all custom macros for GDExtension's boilerplate code.
 * The purpose is to minimize typos from copying these boilerplate code.
 * @version 1.0.0
 * @date 2024-11-10
 * 
 */

#ifndef BOILERPLATE_MACROS_H
#define BOILERPLATE_MACROS_H

/*--------------------------------------------------*/

// Put in header files. Declare functions needed to export instance variables.
#define _GDEXPORT /***********************************************/ \
protected:                                                          \
    void _get_property_list(List<PropertyInfo> *p_list) const;      \
    bool _get(const StringName &p_name, Variant &r_property) const; \
    bool _set(const StringName &p_name, const Variant &p_property);

/*--------------------------------------------------*/

// Start of _get_property_list().
#define _GDEXPORT_ADD_PREFIX(ClassName) \
void ClassName::_get_property_list(List<PropertyInfo> *p_list) const {

// Middle of _get_property_list(). Add a variable to the list of exported properties.
#define _GDEXPORT_ADD(PropertyInfo) \
p_list->push_back(PropertyInfo);

// End of _get_property_list().
#define _GDEXPORT_ADD_SUFFIX \
}

/*--------------------------------------------------*/

// Start of _get().
#define _GDEXPORT_GET_PREFIX(ClassName) \
bool ClassName::_get(const StringName &p_name, Variant &r_property) const {

// Middle of _get(). If the variable is exported, get its value and return true.
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
#define _GDEXPORT_SET_PREFIX(ClassName) \
bool ClassName::_set(const StringName &p_name, const Variant &p_property) {

// Middle of _set(). If the variable is exported, set its value and return true.
#define _GDEXPORT_SET(variable) /********/ \
if (p_name.nocasecmp_to(#variable) == 0) { \
    variable = p_property;                 \
    return true;                           \
}

// End of _set().
#define _GDEXPORT_SET_SUFFIX \
	return false;            \
}

/*--------------------------------------------------*/

#endif