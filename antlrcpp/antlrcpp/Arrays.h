//
//  Arrays.h
//  antlrcpp
//
//  Created by Dan McLaughlin on 12/29/13.
//  Copyright (c) 2013 Dan McLaughlin. All rights reserved.
//

#ifndef __antlrcpp__Arrays__
#define __antlrcpp__Arrays__

#include <iostream>
#include <string>
#include <vector>
#include "StringBuilder.h"
#include <list>

/// <summary>
/// Converts List to string with given separator.
/// </summary>
/// <param name="list">The list.</param>
/// <param name="separator">The separator.</param>
/// <returns></returns>
class Arrays {
    public:
#ifdef TODO
    static std::wstring ListToString(std::list<std::wstring> list,std::wstring separator)
    {
    
        StringBuilder *sb = new StringBuilder();
        for (int i = 0; i < list->length; i++)//(std::wstring s in list)
        {
            sb.Append(string.Format("{0}{1}", s, separator));
        }
        string returnString = std::wstring.Empty; ;
        //Remove the last separator from the list
        if (sb.Length > 0)
        {
            returnString = sb.Remove(
                                     sb.ToString().LastIndexOf(separator),
                                     separator.Length).ToString();
        }
        return returnString;
    }
    
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
        std::vector<std::wstring> foo;
        return foo;//StringToStringList(items, "");
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
};
#endif /* defined(__antlrcpp__Arrays__) */
