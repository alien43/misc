using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;
using System.IO;

namespace Text
{
    class TextParser
    {
        const string Letters = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
        const string Digits = "0123456789";
        const string AllChars = Letters + "-"; //+ Digits;
        const string Delimeters = ".,:;\"'/()";
        const string SpaceChars = " \t\n\r";

        static public void Parse(ref string text)
        {
            text = text.ToLower();
            RemoveDiacritics(ref text);
            ProcessHeaders(ref text);
            ProcessLinks(ref text);
            ProcessData(ref text);
            ProcessDecorations(ref text);
            ProcessSpecials(ref text);
            ProcessChars(ref text);            
        }

        static void ProcessLinks(ref string text)
        {
            StringBuilder builder = new StringBuilder();
            int cur = 0;
            while (true) {
                int atStart = text.IndexOf("[[", cur);
                if (atStart == -1) {
                    break;
                }
                int atEnd = text.IndexOf("]]", atStart);
                if (atEnd == -1) {
                    break;
                }
                builder.Append(text.Substring(cur, atStart - cur));
                string link = text.Substring(atStart + 2, atEnd - atStart - 2);
                int atPipe = link.LastIndexOf('|');
                if (!link.Contains(":"))
                {
                    builder.Append(link.Substring(atPipe + 1));
                }
                cur = atEnd + 2;
            }
            builder.Append(text.Substring(cur, text.Length - cur));
            text = builder.ToString();
        }

        static void ProcessData(ref string text)
        {
            StringBuilder builder = new StringBuilder();
            int cur = 0;
            int balance = 0;
            while (true)
            {
                if (cur + 1 >= text.Length) {
                    break;
                }
                if (text.Substring(cur, 2) == "{{")
                {
                    ++balance;
                    ++cur;
                }
                else if (text.Substring(cur, 2) == "}}")
                {
                    --balance;
                    ++cur;
                }
                ++cur;
                if (balance == 0)
                {
                    builder.Append(text[cur]);
                }
                if (balance < 0)
                {
                    // invalid wiki page, better leave it as it is
                    return;
                }
            }
                if (balance == 0 && text.Length > 0)
                {
                    builder.Append(text[text.Length - 1]);
                }
            text = builder.ToString();
        }

        static void ProcessHeaders(ref string text)
        {
            StringBuilder builder = new StringBuilder();
            StringReader reader = new StringReader(text);
            string[] badStarts = { "==", "[[категория:", "''см. также" };
            string line;
            while ((line = reader.ReadLine()) != null)
            {
                line = line.TrimStart(" ".ToCharArray());
                bool isGood = true;
                foreach (string badStart in badStarts)
                {
                    if (line.StartsWith(badStart))
                    {
                        isGood = false;
                    }
                }
                if (isGood)
                {
                    builder.AppendLine(line);
                }
            }
            text = builder.ToString();
        }

        static void RemoveDiacritics(ref string text)
        {
            // сохраним букву 'й'
            const char neverMetChar = (char)0;
            text = text.Replace('й', neverMetChar);
            string normalizedString = text.Normalize(NormalizationForm.FormD);
            StringBuilder stringBuilder = new StringBuilder();

            for (int i = 0; i < normalizedString.Length; i++)
            {
                Char c = normalizedString[i];
                if (CharUnicodeInfo.GetUnicodeCategory(c) != UnicodeCategory.NonSpacingMark)
                    stringBuilder.Append(c);
            }
            text = stringBuilder.ToString();
            text = text.Replace(neverMetChar, 'й');
        }

        static void ProcessSpecials(ref string text)
        {
            const char hyphen = '-';
            const char space = ' ';

            StringBuilder builder = new StringBuilder();
            int cur = 0;
            string toAppend;
            while (true)
            {
                int atHyphen = text.IndexOf(hyphen, cur);
                if (atHyphen == -1)
                {
                    break;
                }
                toAppend = "";
                if (atHyphen > cur)
                {
                    toAppend = text.Substring(cur, atHyphen - cur);
                }
                
                builder.Append(toAppend);
                if ((atHyphen > 0 && Letters.IndexOf(text[atHyphen - 1]) != -1) &&
                    (atHyphen + 1 < text.Length && Letters.IndexOf(text[atHyphen + 1]) != -1))
                {
                    builder.Append(hyphen);
                }
                else
                {
                    builder.Append(space);
                }
                cur = atHyphen + 1;
            }
            toAppend = text.Substring(cur, text.Length - cur);
            builder.Append(toAppend);
            text = builder.ToString();
        }

        enum CharType { Letter, Delimeter, Other };

        static void ProcessChars(ref string text)
        {
            const char space = ' ';
            const string delim = ". ";
            StringBuilder stringBuilder = new StringBuilder();
            CharType lastCharType = CharType.Other;
            for (int i = 0; i < text.Length; ++i)
            {
                char currentChar = text[i];
                if (AllChars.Contains(currentChar.ToString()))
                {
                    stringBuilder.Append(currentChar);
                    lastCharType = CharType.Letter;
                }
                else if (SpaceChars.Contains(currentChar.ToString()))
                {
                    if (lastCharType == CharType.Letter)
                    {
                        stringBuilder.Append(space);
                        lastCharType = CharType.Other;
                    }
                }
                else
                {
                    if (lastCharType != CharType.Delimeter)
                    {
                        if (lastCharType == CharType.Letter)
                        {
                            stringBuilder.Append(space);
                        }
                        stringBuilder.Append(delim);
                    }
                    lastCharType = CharType.Delimeter;
                }
#if false
                else if (Delimeters.Contains(currentChar.ToString()))
                {
                    if (lastCharType != CharType.Delimeter)
                    {
                        if (lastCharType == CharType.Letter)
                        {
                            stringBuilder.Append(space);
                        }
                        stringBuilder.Append(delim);
                    }
                    lastCharType = CharType.Delimeter;
                }
                else
                {
                    if (lastCharType == CharType.Letter)
                    {
                        stringBuilder.Append(space);
                        lastCharType = CharType.Other;
                    }
                }
#endif
            }
            text = stringBuilder.ToString();
        }

        static void ProcessDecorations(ref string text)
        {
            const string bold = "'''";
            const string italic = "''";
            text = text.Replace(bold, "");
            text = text.Replace(italic, "");
        }
    }
}
