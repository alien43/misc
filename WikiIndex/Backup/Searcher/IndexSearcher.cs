using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Searcher
{
    using Id = UInt32;
    using Offset = UInt32;

    class ResultIdSorter : IComparer<Id>
    {
        Dictionary<Id, uint>[] wordsStats_;

        public ResultIdSorter(Dictionary<Id, uint>[] wordsStats)
        {
            wordsStats_ = wordsStats;
        }
        public int Compare(Id id1, Id id2)
        {
            return CountStats(id2).CompareTo(CountStats(id1));
        }

        private uint CountStats(Id id)
        {
            uint result = 0;
            foreach (Dictionary<Id, uint> wordStats in wordsStats_)
            {
                if (wordStats.ContainsKey(id))
                {
                    result += wordStats[id];
                }
            }
            return result;
        }
    }

    class IndexSearcher
    {
        const string COMMON_PREFIX = "http://ru.wikipedia.org/wiki/";
        UInt32 version_;
        string[] articleTitles_;
        Dictionary<string, ulong> positions_;
        bool isLoaded_;
        string indexFileName_;

        public IndexSearcher()
        {
            isLoaded_ = false;
        }

        public void Load(string headerFileName, string documentListFileName)
        {
            ReadHeader(headerFileName);
            ReadDocumentList(documentListFileName);
            isLoaded_ = true;
        }
        public void SetIndexFile(string fileName)
        {
            indexFileName_ = fileName;
        }
        public string[] Search(string query, uint count)
        {
            if (!isLoaded_)
            {
                Console.Error.WriteLine("IndexSearcher was not loaded, you cannot use it for searching");
                return new string[0];
            }
            string[] words = Text.Parser.Parse(query);
            if (words.Length == 0)
            {
                return new string[0];
            }

            Dictionary<Id, uint>[] wordsStats = new Dictionary<Id, uint>[words.Length];
            for (int iWord = 0; iWord < words.Length; ++iWord)
            {
                wordsStats[iWord] = SearchWord(words[iWord]);
            }
            HashSet<Id> idSet = new HashSet<Id>(wordsStats[0].Keys);
            for (int iWord = 1; iWord < words.Length; ++iWord)
            {
                idSet.IntersectWith(wordsStats[iWord].Keys);
            }
            List<Id> idList = new List<Id>(idSet);
            idList.Sort(new ResultIdSorter(wordsStats));

            if (idList.Count > count)
            {
                idList.RemoveRange((int)count, idList.Count - (int)count);
            }
            string[] result = new string[idList.Count];
            for (int i = 0; i < idList.Count; ++i)
            {
                Id id = idList[i];
                result[i] = COMMON_PREFIX + articleTitles_[id];
            }
            return result;
        }

        private void ReadHeader(string fileName)
        {
            FileStream fin;
            try
            {
                fin = new FileStream(fileName, FileMode.Open, FileAccess.Read);
            }
            catch
            {
                Console.Error.WriteLine("Could not open header ", fileName);
                return;
            }
            BinaryReader reader = new BinaryReader(fin);
            version_ = reader.ReadUInt32();
            if (version_ == 1)
            {
                positions_ = new Dictionary<string, ulong>();
                int wordsCount = reader.ReadInt32();
                for (int i = 0; i < wordsCount; ++i)
                {
                    string word = reader.ReadString();
                    ulong position = reader.ReadUInt64();
                    positions_[word] = position;
                }
            }
            fin.Close();
        }
        private void ReadDocumentList(string fileName)
        {
            FileStream fin;
            try
            {
                fin = new FileStream(fileName, FileMode.Open, FileAccess.Read);
            }
            catch
            {
                Console.Error.WriteLine("Could not open document list ", fileName);
                return;
            }
            BinaryReader reader = new BinaryReader(fin);
            version_ = reader.ReadUInt32();
            if (version_ == 1)
            {
                int length = reader.ReadInt32();
                articleTitles_ = new string[length];
                for (int i = 0; i < length; ++i)
                {
                    articleTitles_[i] = reader.ReadString();
                }
            }
            fin.Close();
        }
        private Dictionary<Id, uint> SearchWord(string word)
        {
            Dictionary<Id, uint> result = new Dictionary<Id, uint>();
            if (!positions_.ContainsKey(word))
            {
                return result;
            }
            ulong position = positions_[word];
            FileStream fin;
            try
            {
                fin = new FileStream(indexFileName_, FileMode.Open, FileAccess.Read);
            }
            catch
            {
                Console.Error.WriteLine("Could not open index file ", indexFileName_);
                return result;
            }
            BinaryReader reader = new BinaryReader(fin);
            reader.ReadUInt32();
            fin.Seek((long)position, SeekOrigin.Current);
            int count = reader.ReadInt32();
            for (int iText = 0; iText < count; ++iText)
            {
                Id id = reader.ReadUInt32();
                int length = reader.ReadInt32();
                for (int iOffset = 0; iOffset < length; ++iOffset)
                {
                    reader.ReadUInt32();
                }
                result.Add(id, (uint)length);
            }
            fin.Close();
            return result;
        }
    }
}
