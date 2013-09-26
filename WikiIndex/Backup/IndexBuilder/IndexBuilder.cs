using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;

namespace IndexBuilder
{
    using Id = UInt32;
    using Offset = UInt32;

    struct TextOccurences
    {
        public Id id;
        public Offset[] offsets;

        public TextOccurences(Id id, Offset[] offsets)
        {
            this.id = id;
            this.offsets = offsets;
        }
    }

    struct Article
    {
        public string title;
        public string text;
    }
    
    class IndexBuilder
    {
        uint minWordLength_;
        string[] articleTitles_;
        Dictionary< string, List<TextOccurences> > occurences_;
        bool isBuilt_;
        const UInt32 CURRENT_VERSION = 1;

        public IndexBuilder(uint minWordLength)
        {
            minWordLength_ = minWordLength;
            isBuilt_ = false;
        }
        public void LoadXml(string fileName)
        {
            if (!File.Exists(fileName))
            {
                Console.Error.WriteLine("Could not find XML file ", fileName);
                return;
            }

            XmlTextReader reader = new XmlTextReader(fileName);
            reader.ReadToFollowing("page");
            Id currentId = 0;
            List<string> articlesTitlesList = new List<string>();
            occurences_ = new Dictionary<string, List<TextOccurences>>();
            while (!reader.EOF)
            {
                Article article = ReadArticle(reader);
                if (article.title == "")
                {
                    break;
                }
                articlesTitlesList.Add(article.title);
                ProcessText(article.text, currentId);
                ++currentId;
                if (currentId % 5000 == 0)
                {
                    Console.WriteLine("Processed " + currentId.ToString() + " articles");
                }
            }
            articleTitles_ = articlesTitlesList.ToArray();
            reader.Close();
            isBuilt_ = true;
        }
        public void Write(string indexFileName, string headerFileName,
            string documentListFileName)
        {
            if (!isBuilt_)
            {
                Console.Error.WriteLine("Index is not build, so it cannot be written");
                return;
            }
            Console.WriteLine();
            Console.WriteLine("Started writing files...");
            WriteIndex(indexFileName);
            WriteHeader(headerFileName);
            WriteDocumentList(documentListFileName);
            Console.WriteLine("Files written successfully!");
            
        }


        private Article ReadArticle(XmlTextReader reader)
        {
            Article article = new Article();
            reader.ReadToFollowing("title");
            article.title = reader.ReadString();
            reader.ReadToFollowing("text");
            article.text = reader.ReadString();
            return article;
        }
        private void ProcessText(string text, Id textId)
        {
            string[] words = Text.Parser.Parse(text);
            Dictionary< string, List<Offset> > newOccurences = new Dictionary< string, List<Offset> >();
            for (uint iWord = 0; iWord < words.Length; ++iWord)
            {
                string currentWord = processWord(words[iWord]);
                if (currentWord.Length < minWordLength_)
                {
                    continue;
                }
                if (!newOccurences.ContainsKey(currentWord))
                {
                    newOccurences.Add(currentWord, new List<Offset>());
                }
                newOccurences[currentWord].Add(iWord);
            }
            foreach (string word in newOccurences.Keys)
            {
                if (!occurences_.ContainsKey(word))
                {
                    occurences_.Add(word, new List<TextOccurences>());
                }
                occurences_[word].Add(new TextOccurences(textId, newOccurences[word].ToArray()));
            }
        }
        private string processWord(string word)
        {
            return word;
        }
        private void WriteIndex(string fileName)
        {
            FileStream fout;
            try
            {
                fout = new FileStream(fileName, FileMode.Create, FileAccess.Write);
            }
            catch
            {
                Console.Error.WriteLine("Could not write index file to ", fileName);
                return;
            }
            BinaryWriter writer = new BinaryWriter(fout);
            writer.Write(CURRENT_VERSION);
            foreach (List<TextOccurences> list in occurences_.Values)
            {
                writer.Write(list.Count);
                foreach (TextOccurences textOccurences in list)
                {
                    writer.Write(textOccurences.id);
                    writer.Write(textOccurences.offsets.Length);
                    foreach (Offset offset in textOccurences.offsets)
                    {
                        writer.Write(offset);
                    }
                }
            }
            fout.Close();
        }
        
        private void WriteHeader(string fileName)
        {
            FileStream fout;
            try
            {
                fout = new FileStream(fileName, FileMode.Create, FileAccess.Write);
            }
            catch
            {
                Console.Error.WriteLine("Could not write header file to ", fileName);
                return;
            }
            BinaryWriter writer = new BinaryWriter(fout);
            writer.Write(CURRENT_VERSION);
            writer.Write(occurences_.Keys.Count);
            ulong position = 0;
            foreach (string word in occurences_.Keys)
            {
                writer.Write(word);
                writer.Write(position);
                List<TextOccurences> list = occurences_[word];
                // list.count size
                position += sizeof(int);
                foreach (TextOccurences textOccurences in list)
                {
                    // textOccurences.id size
                    position += sizeof(Id);
                    // textOccurences.offsets.Length size
                    position += sizeof(int);
                    // textOccurences.offsets size
                    position += (uint)textOccurences.offsets.Length * sizeof(Offset);
                }
            }
            fout.Close();
        }
        private void WriteDocumentList(string fileName)
        {
            FileStream fout;
            try
            {
                fout = new FileStream(fileName, FileMode.Create, FileAccess.Write);
            }
            catch
            {
                Console.Error.WriteLine("Could not write document list file to ", fileName);
                return;
            }
            BinaryWriter writer = new BinaryWriter(fout);
            writer.Write(CURRENT_VERSION);
            writer.Write(articleTitles_.Length);
            foreach (string title in articleTitles_)
            {
                writer.Write(title);
            }
            fout.Close();
        }
    }


}
