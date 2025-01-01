/**
 * SPDX-FileCopyrightText: 2021-2023 EasyCoding Team and contributors
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
*/

/** @file app/newprofiledialog/profilesyntaxhighlighter.cpp

*/

#include "profilesyntaxhighlighter.h"

ProfileHighlighter::ProfileHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    singleLineCommentFormat.setForeground(Qt::green);
    rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    confSectionFormat.setForeground(Qt::cyan);
    rule.pattern = QRegularExpression(QStringLiteral("\\[.*\\]"));
    rule.format = confSectionFormat;
    highlightingRules.append(rule);

    confValueFormat.setForeground(Qt::yellow);
    rule.pattern = QRegularExpression(QStringLiteral("(?<=\\=).*[^\n]"));
    rule.format = confValueFormat;
    highlightingRules.append(rule);
}

ProfileHighlighter::~ProfileHighlighter()
{

}

void ProfileHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : std::as_const(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
