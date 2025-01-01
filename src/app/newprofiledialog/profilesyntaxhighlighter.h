/**
 * SPDX-FileCopyrightText: 2021-2023 EasyCoding Team and contributors
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef PROFILESYNTAXHIGHLIGHTER_H
#define PROFILESYNTAXHIGHLIGHTER_H

/** @file app/newprofiledialog/profilesyntaxhighlighter.h

*/

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class ProfileHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    ProfileHighlighter(QTextDocument *parent = nullptr);
    virtual ~ProfileHighlighter();

protected:
    virtual void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;

    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat confSectionFormat;
    QTextCharFormat confValueFormat;
};



#endif
