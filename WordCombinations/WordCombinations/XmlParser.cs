using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;

namespace WordCombinations
{
    struct Article
    {
        public string title;
        public string text;
    }

    class XmlParserConfig
    {

    }

    class XmlParser
    {
        XmlParserConfig config_;
        const UInt32 CURRENT_VERSION = 1;

        public XmlParser(XmlParserConfig config)
        {
            config_ = config;
        }

        public void ParseXml(string xmlFileName, string outputFileName)
        {
            if (!File.Exists(xmlFileName))
            {
                Console.Error.WriteLine("Could not read XML file ", xmlFileName);
                return;
            }

            XmlTextReader reader = new XmlTextReader(xmlFileName);
            reader.ReadToFollowing("page");
            reader.ReadToFollowing("page");

            StreamWriter writer = new StreamWriter(outputFileName, false, Encoding.GetEncoding(1251));
            writer.WriteLine(CURRENT_VERSION);
            uint nArticles = 0;
            while (!reader.EOF)
            {
                Article article = ReadArticle(reader);
                if (article.text == null)
                {
                    break;
                }
                if (article.text.Contains("#REDIRECT"))
                {
                    continue;
                }
                ++nArticles;
                Text.TextParser.Parse(ref article.text);
                writer.WriteLine(article.title);
                writer.WriteLine(article.text);
            }
            writer.Close();
        }

        private Article ReadArticle(XmlTextReader reader)
        {
            Article article = new Article();
            try
            {
                reader.ReadToFollowing("title");
            }
            catch
            {
                article.title = "";
                return article;
            }
            article.title = reader.ReadString();
            if (article.title.Contains("Распределенные файловые системы"))
            {
                int k = 43;
            }
            reader.ReadToFollowing("text");
            article.text = reader.ReadString();

            return article;
        }
    }
}
