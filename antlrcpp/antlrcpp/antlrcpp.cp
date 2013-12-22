/*
 *  antlrcpp.cp
 *  antlrcpp
 *
 *  Created by Dan McLaughlin on 12/22/13.
 *  Copyright (c) 2013 Dan McLaughlin. All rights reserved.
 *
 */

#include <iostream>
#include "antlrcpp.h"
#include "antlrcppPriv.h"

void antlrcpp::HelloWorld(const char * s)
{
	 antlrcppPriv *theObj = new antlrcppPriv;
	 theObj->HelloWorldPriv(s);
	 delete theObj;
};

void antlrcppPriv::HelloWorldPriv(const char * s) 
{
	std::cout << s << std::endl;
};

