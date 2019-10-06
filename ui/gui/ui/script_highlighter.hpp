
/*
 * Special thanks for code to luaeditor (https://code.google.com/p/luaeditor)
 */

#ifndef SCRIPT_HIGHLIGHTER_HPP
#define SCRIPT_HIGHLIGHTER_HPP

#include "all.hpp"

class QScriptHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
	
    struct HighlightingRule
    {
		QRegExp pattern;
		QTextCharFormat format;
    };

	QVector<HighlightingRule> highlightingRules;
	QRegExp commentStartExpression, commentEndExpression;
    QTextCharFormat keywordFormat, singleLineCommentFormat, multiLineCommentFormat, quotationFormat, functionFormat, umlaut, numbers;

	protected:
		
		void highlightBlock(const QString & txt);

	public:
		
		QBrush clrUmlauts, clrNumbers, clrSingleComment, clrMultiComment, clrDoubleQuote, clrSingeleQuote, clrFunction;

		QScriptHighlighter();
};

#endif

