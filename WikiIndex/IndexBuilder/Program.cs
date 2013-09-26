using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;


namespace IndexBuilder
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.Write("Enter output index directory: ");
            string WorkingDirectory = Console.ReadLine();
            WorkingDirectory.TrimEnd('/');
            if (!Directory.Exists(WorkingDirectory))
            {
                Directory.CreateDirectory(WorkingDirectory);
            }
            string indexPath = WorkingDirectory + "/index.dat";
            string headerPath = WorkingDirectory + "/header.dat";
            string documentsPath = WorkingDirectory + "/documents.dat";

            Console.Write("Enter minimum word length: ");
            uint minWordLength = UInt32.Parse(Console.ReadLine());
            IndexBuilder builder = new IndexBuilder(minWordLength);
            Console.Write("Enter articles XML path: ");
            string xmlPath = Console.ReadLine();
            builder.LoadXml(xmlPath);
            builder.Write(indexPath, headerPath, documentsPath);
        }
    }
}
