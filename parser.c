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
    // 需要分析混淆的 Objective C 源代码文件
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
    // 一个个经过词法分析以后得到的 token
    CXToken *tokens;
    unsigned int numTokens;
    CXCursor *cursors = 0;
    CXSourceRange range = clang_getCursorExtent(rootCursor);
	clang_visitChildren(rootCursor, func_visitor_for_awk, NULL);
#if 0
    // 获取所有的 token
    clang_tokenize(tu, range, &tokens, &numTokens);
    cursors = (CXCursor *)malloc(numTokens * sizeof(CXCursor));
    // 获取每个 token 对应的 cursor
    clang_annotateTokens(tu, tokens, numTokens, cursors);
    // 遍历 token
    for(int i=0; i < numTokens; i++) {
        CXToken token = tokens[i];
        CXCursor cursor = cursors[i];

        CXString tokenSpelling = clang_getTokenSpelling(tu, token);
        CXString cursorSpelling = clang_getCursorSpelling(cursor);
        const char *tokenName = clang_getCString(tokenSpelling);
		const char *cursorName = clang_getCString(cursorSpelling);
		if (clang_getCursorKind(cursor) == CXCursor_FunctionDecl)
		{
			printf("cur fun is %s ;; %s \n" , tokenName, cursorName);
		}


   //     if (CXToken_Literal == clang_getTokenKind(token))
        {

            printf("\t%d\t%s\n", cursor.kind, tokenName);
        }
        // 释放内存
        clang_disposeString(tokenSpelling);
        clang_disposeString(cursorSpelling);
    }
#endif
    // 释放内存
    clang_disposeTokens(tu, tokens, numTokens);
    clang_disposeIndex(index);
    clang_disposeTranslationUnit(tu);
    free(cursors);
    
	return 0;
}
