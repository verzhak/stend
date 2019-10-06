
#include "ui/gui/ui/script_highlighter.hpp"

QScriptHighlighter::QScriptHighlighter() :
	QSyntaxHighlighter((QTextDocument *) NULL) // Потому-что у базового класса перегруженный конструктор - и везде указатели
{
    HighlightingRule rule;
    QStringList keywordPatterns;

	keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    keywordPatterns << "\\bfunction\\b"     << "\\bend\\b"          << "\\bwhile\\b"
                    << "\\bfor\\b"          << "\\buntil\\b"        << "\\band\\b"
                    << "\\bbreak\\b"        << "\\bdo\\b"           << "\\belse\\b"
                    << "\\bbelseif\\b"      << "\\bfalse\\b"        << "\\bif\\b"
                    << "\\bin\\b"           << "\\blocal\\b"        << "\\bnil\\b"
                    << "\\bnot\\b"          << "\\bor\\b"           << "\\breturn\\b"
                    << "\\brepeat\\b"       << "\\bthen\\b"         << "\\btrue\\b"
                    << "\\btonumber\\b"     << "\\btostring\\b"     << "\\bprint\\b"
                    << "\\berror\\b"        << "\\bipairs\\b"       << "\\bpairs\\b"
                    << "\\bsetmetatable\\b" << "\\bgetmetatable\\b" << "\\btype\\b"
                    << "\\bselect\\b"       << "\\bloadstring\\b"   << "\\bloadfile\\b"
                    << "\\barg\\b"          << "\\bmath\\b"         << "\\bstring\\b"
                    << "\\bos\\b"           << "\\bio\\b"           << "\\belseif\\b";

	for(const QString & pattern : keywordPatterns)
    {
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
    }

	clrUmlauts = Qt::red;
	clrNumbers = Qt::magenta;
	clrSingleComment = Qt::darkGreen;
	clrMultiComment = Qt::darkGreen;
	clrDoubleQuote = Qt::darkGreen;
	clrSingeleQuote = Qt::darkGreen;
	clrFunction = Qt::darkCyan;

	// These characters aren't good in lua
	umlaut.setFontWeight(QFont::Bold);
	umlaut.setForeground(clrUmlauts);
	rule.pattern = QRegExp("[ÄÖÜßäöü]");
	rule.format = umlaut;
	highlightingRules.append(rule);

	// 0 1 2 3 4 5 6 7 8 9
	numbers.setForeground(clrNumbers);
	rule.pattern = QRegExp("[0-9]");
	rule.format = numbers;
	highlightingRules.append(rule);

	// Single line comment -> --
	singleLineCommentFormat.setForeground(clrSingleComment);
	rule.pattern = QRegExp("--+[^[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(clrMultiComment);

    // "Quotation"
	quotationFormat.setForeground(clrDoubleQuote);
	rule.pattern = QRegExp("\".*\"");
	rule.pattern = QRegExp( "(?:^|[^\\\\'])(\"(?:\\\\\"|\\\\(?!\")|[^\\\\\"^ä^ö^ü])*\")" );
	rule.pattern.setMinimal(true);
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	// 'Quotation'
	quotationFormat.setForeground(clrSingeleQuote);
	rule.pattern = QRegExp("\'.*\'");
	rule.pattern = QRegExp( "(?:^|[^\\\\\"])(\'(?:\\\\\'|\\\\(?!\')|[^\\\\\'])*\')" );
	rule.pattern.setMinimal(true);
	rule.format = quotationFormat;
	highlightingRules.append(rule);
	
	// function name()
	functionFormat.setFontItalic(true);
	functionFormat.setForeground(clrFunction);
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);
	
	// Multi Line Comment --[[ ]]
	commentStartExpression = QRegExp("--\\[\\["); // --[[
	commentEndExpression = QRegExp("\\]\\]"); // ]]
	
	rule.pattern.setMinimal(false);
}

void QScriptHighlighter::highlightBlock(const QString & txt)
{
	for(const HighlightingRule & rule : highlightingRules)
	{
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(txt);
        
		while(index >= 0)
		{
			const int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(txt, index + length);
        }
    }
	
	setCurrentBlockState(0);

    int startIndex = 0;

	if(previousBlockState() != 1)
		startIndex = commentStartExpression.indexIn(txt);
	
	while(startIndex >= 0)
	{
		int endIndex = commentEndExpression.indexIn(txt, startIndex);
		int commentLength;
		
		if(endIndex == -1)
		{
			setCurrentBlockState(1);
			commentLength = txt.length() - startIndex;
		}
		else
			commentLength = endIndex - startIndex + commentEndExpression.matchedLength();

		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = commentStartExpression.indexIn(txt, startIndex + commentLength);
    }
}

