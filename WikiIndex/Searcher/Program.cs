using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Searcher
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("Enter input index directory: ");
            string WorkingDirectory = Console.ReadLine();
            WorkingDirectory.TrimEnd('/');
            string indexPath = WorkingDirectory + "/index.dat";
            string headerPath = WorkingDirectory + "/header.dat";
            string documentsPath = WorkingDirectory + "/documents.dat";

            Console.WriteLine("Loading index searcher...");
            IndexSearcher searcher = new IndexSearcher();
            searcher.Load(headerPath, documentsPath);
            searcher.SetIndexFile(indexPath);
            Console.WriteLine("Loaded successfully, ready to process queries.");
            Console.Write("Enter number of matches to show: ");
            uint matchesCount = UInt32.Parse(Console.ReadLine());
            Console.WriteLine("Write \"q\" as a query to exit.");
            bool isFinished = false;
            do
            {
                Console.Write("Enter search query: ");
                string query = Console.ReadLine();
                if (query == "q")
                {
                    isFinished = true;
                }
                else
                {
                    string[] searchResults = searcher.Search(query, matchesCount);
                    foreach (string result in searchResults)
                    {
                        Console.WriteLine(result);
                    }
                    Console.WriteLine();
                }
            } while (!isFinished);
        }
    }
}
