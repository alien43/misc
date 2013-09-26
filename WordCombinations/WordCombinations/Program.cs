using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WordCombinations
{
    class Program
    {
        static void Main(string[] args)
        {
            string path = "c:\\ys\\text\\";
            string xmlFileName = path + "wiki.xml";
            string outputFileName = path + "wiki.txt";

            XmlParserConfig xmlParserConfig = new XmlParserConfig();
            XmlParser xmlParser = new XmlParser(xmlParserConfig);
            xmlParser.ParseXml(xmlFileName, outputFileName);
        }
    }
}
