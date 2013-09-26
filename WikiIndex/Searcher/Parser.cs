using System;
using System.Collections.Generic;
using System.Text;

namespace Text
{
    class Parser
    {
        const string LowerAlphabet = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
        static public string[] Parse(string text)
        {
            text = text.ToLower();
            StringBuilder stringBuilder = new StringBuilder();
            bool wasLastCharSpace = true;
            for (int i = 0; i < text.Length; ++i)
            {
                char currentChar = text[i];
                if (LowerAlphabet.IndexOf(currentChar) != -1)
                {
                    stringBuilder.Append(currentChar);
                    wasLastCharSpace = false;
                }
                else if (currentChar != '́')
                {
                    if (!wasLastCharSpace)
                    {
                        stringBuilder.Append(' ');
                    }
                    wasLastCharSpace = true;
                }
            }
            return stringBuilder.ToString().Trim().Split(' ');
        }
    }
}
