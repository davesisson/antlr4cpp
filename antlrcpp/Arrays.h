//
//  Arrays.h
//  antlrcpp
//
//  Created by Dan McLaughlin on 12/29/13.
//  Copyright (c) 2013 Dan McLaughlin. All rights reserved.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "StringBuilder.h"
#include <list>
#include <stdarg.h>
#include "Declarations.h"

/// <summary>
/// Converts List to string with given separator.
/// </summary>
/// <param name="list">The list.</param>
/// <param name="separator">The separator.</param>
/// <returns></returns>
class Arrays {
    public:

    static std::wstring ListToString(std::vector<std::wstring> list,std::wstring separator)
    {
    
        StringBuilder *sb = new StringBuilder();
        for (int i = 0; i < list.size(); i++)//(std::wstring s in list)
        {
            sb->append(list[i]);
			if (i + 1 < list.size()) sb->append(separator);
        }
        
        return sb->toString();
    }
#ifdef TODO  
    /// <summary>
    /// Strings to string list.
    /// </summary>
    /// <param name="items">The items.</param>
    /// <param name="separator">The separator.</param>
    /// <returns></returns>
    static std::list<std::wstring> StringToStringList(std::wstring items, char separator)
    {
        std::list<std::wstring> *list = new std::list<std::wstring>();
        std::wstring[] listItmes = items.Split(separator);
        foreach (std::wstring item in listItmes)
        {
            list.Add(item);
        }
        if (list.Count > 0) {
            return list;
        } else {
            return null;
        }
    }
    

#endif
    
    static std::vector<std::wstring> asList(const std::wstring *items)
    {
#ifdef TODO
        FIX ME
#endif
        
        std::vector<std::wstring> foo;
        return foo;//StringToStringList(items, "");
    }
    static std::vector<std::wstring> asList(int nArgs, ...)
    {
#ifdef TODO
        FIX ME
#endif
        
        std::vector<std::wstring> foo;
        return foo;//StringToStringList(items, "");
    }
    
    static bool equals(std::vector<int> a, std::vector<int> b)
    {
#ifdef TODO
        FIX ME
#endif
        return false;
    }
    
//    static bool equals(std::vector<PredictionContext*> a, std::vector<PredictionContext*> b)
//    {
//#ifdef TODO
//        FIX ME
//#endif
//        return false;
//    }
    
    static bool equals(void *a, void* b)
    {
#ifdef TODO
        FIX ME
#endif
        return false;
    }
    /// <summary>
    /// Strings to string list.
    /// </summary>
    /// <param name="items">The items.</param>
    /// <param name="separator">The separator.</param>
    /// <returns></returns>
    
    static std::list<std::wstring> copyOf(void * obj, int num)
    {
        std::list<std::wstring> foo;
        return foo;
    }

    static std::wstring copyOf(std::wstring obj, int num)
    {
        std::wstring foo;
        return foo;
    }
    
    static void arraycopy(void * arrayA, void * arrayB, int num)
    {

    }
};

