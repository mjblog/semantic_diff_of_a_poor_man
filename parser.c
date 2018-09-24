#include <stdio.h>
#include "clang-c/Index.h"

enum CXChildVisitResult func_visitor (CXCursor cursor, CXCursor parent, CXClientData client_data)
{
	const char *cursorName = clang_getCString (clang_getCursorSpelling(cursor));
	if (clang_getCursorKind (cursor) == CXCursor_FunctionDecl 
		&& clang_isCursorDefinition (cursor))
	{
		printf("cur fun is %s \n" ,cursorName);
		CXSourceRange  range = clang_getCursorExtent (cursor);
		CXSourceLocation start = clang_getRangeStart (range);
		CXSourceLocation end = clang_getRangeEnd (range);
		CXFile  	file;
		unsigned  	line;
		unsigned  	column;
		unsigned  	offset;
		clang_getExpansionLocation(start, &file, &line, &column, &offset);
		printf("start at %s line %u:%u \n", clang_getCString (clang_getFileName (file)), 
				line, column);
		clang_getExpansionLocation(end, &file, &line, &column, &offset);
		printf("end at %s line %u:%u \n", clang_getCString (clang_getFileName (file)), 
				line, column);
		return CXChildVisit_Continue;
	}
//	else
//		printf("cur  is %s ;;\n" ,cursorName);
	return CXChildVisit_Recurse;
}


enum CXChildVisitResult func_visitor_for_awk (CXCursor cursor, CXCursor parent, CXClientData client_data)
{
	const char *cursorName = clang_getCString (clang_getCursorSpelling(cursor));
	if (clang_getCursorKind (cursor) == CXCursor_FunctionDecl 
		&& clang_isCursorDefinition (cursor))
	{
		printf("cur fun is %s " ,cursorName);
		CXSourceRange  range = clang_getCursorExtent (cursor);
		CXSourceLocation start = clang_getRangeStart (range);
		CXSourceLocation end = clang_getRangeEnd (range);
		CXFile  	file;
		unsigned  	line;
		unsigned  	column;
		unsigned  	offset;
		clang_getExpansionLocation(start, &file, &line, &column, &offset);
		printf("start at %s line %u:%u ", clang_getCString (clang_getFileName (file)), 
				line, column);
		clang_getExpansionLocation(end, &file, &line, &column, &offset);
		printf("end at %s line %u:%u \n", clang_getCString (clang_getFileName (file)), 
				line, column);
		return CXChildVisit_Continue;
	}
//	else
//		printf("cur  is %s ;;\n" ,cursorName);
	return CXChildVisit_Recurse;
}


int main(int argc, const char * argv[]) {

    // 一个编译单元，通常是一个文件
    CXTranslationUnit tu;
    // 一个 index 可以包含多个编译单元
    CXIndex index = clang_createIndex(1, 1);

    const char *filePath = argv[1];
    // 
    tu = clang_parseTranslationUnit(index, filePath, NULL, 0, NULL, 0, 0);
    if (!tu) 
	{
        printf("Couldn't create translation unit");
        return 1;
	}
    // 根 cursor
    CXCursor rootCursor = clang_getTranslationUnitCursor(tu);

    unsigned int numTokens;

    CXSourceRange range = clang_getCursorExtent(rootCursor);
	clang_visitChildren(rootCursor, func_visitor_for_awk, NULL);

    // 释放内存
    clang_disposeIndex(index);
    clang_disposeTranslationUnit(tu);

    
	return 0;
}
